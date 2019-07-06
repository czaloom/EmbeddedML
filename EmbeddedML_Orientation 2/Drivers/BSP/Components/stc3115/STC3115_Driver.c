/**
  ******************************************************************************
  * @file    STC3115_Driver.c
  * @author  Central Labs
  * @version V1.0.0
  * @date    07-October-2014
  * @brief   STC3115 Driver Component Function Vector
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "STC3115_Driver.h"
#include "STC3115_Battery.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup STC3115
  * @brief      Provides a set of functions to drive STMicroelectronics STC3115 Gas Gauge device
  * @{
  */

/** @defgroup STC3115_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @defgroup STC3115_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup STC3115_Private_FunctionPrototypes
  * @{
  */
static DrvStatusTypeDef STC3115_Init( DrvContextTypeDef* );

static DrvStatusTypeDef STC3115_DeInit( DrvContextTypeDef *handle );
//static DrvStatusTypeDef STC3115_Sensor_Enable( DrvContextTypeDef *handle );
//static DrvStatusTypeDef STC3115_Sensor_Disable( DrvContextTypeDef *handle );
static DrvStatusTypeDef STC3115_Get_WhoAmI( DrvContextTypeDef *handle, uint8_t *who_am_i );

static DrvStatusTypeDef STC3115_Task( DrvContextTypeDef* , uint8_t *vm_mode);
static DrvStatusTypeDef STC3115_Reset( DrvContextTypeDef* );
static DrvStatusTypeDef STC3115_Stop( DrvContextTypeDef* );
static DrvStatusTypeDef STC3115_GetSOC( DrvContextTypeDef*, uint32_t*  );
static DrvStatusTypeDef STC3115_GetOCV( DrvContextTypeDef*, uint32_t*  );
static DrvStatusTypeDef STC3115_GetCurrent( DrvContextTypeDef*, int32_t*  );
static DrvStatusTypeDef STC3115_GetTemperature( DrvContextTypeDef*, int32_t*  );
static DrvStatusTypeDef STC3115_GetVoltage( DrvContextTypeDef*, uint32_t*  );
static DrvStatusTypeDef STC3115_GetChargeValue( DrvContextTypeDef*, uint32_t*  );
static DrvStatusTypeDef STC3115_GetPresence( DrvContextTypeDef*, uint32_t*  );
static DrvStatusTypeDef STC3115_GetAlarmStatus( DrvContextTypeDef*, uint32_t*  );

extern uint8_t Sensor_IO_Write( void *handle, uint8_t WriteAddr, uint8_t *pBuffer, uint16_t nBytesToWrite );
extern uint8_t Sensor_IO_Read( void *handle, uint8_t ReadAddr, uint8_t *pBuffer, uint16_t nBytesToRead );

static int STC3115_Read(DrvContextTypeDef* handle, uint8_t RegAddress, uint8_t *data, uint16_t NumByteToRead);
static int STC3115_Write(DrvContextTypeDef* handle, uint8_t RegAddress, uint8_t *data, uint16_t NumByteToWrite);
static int STC3115_ReadWord(DrvContextTypeDef* handle, uint8_t RegAddress, int* value);
static int STC3115_WriteWord(DrvContextTypeDef* handle, uint8_t RegAddress, int value);
static int STC3115_Status( DrvContextTypeDef* handle );
static int STC3115_ReadRamData(DrvContextTypeDef* handle, unsigned char *RamData);
static int STC3115_WriteRamData(DrvContextTypeDef* handle, unsigned char *RamData);
static int STC3115_CalcRamCRC8(unsigned char *data, int n);
static int STC3115_UpdateRamCRC(DrvContextTypeDef* handle);
static void STC3115_InitRamData(DrvContextTypeDef* handle, STC3115_ConfigData_TypeDef *ConfigData);
static int STC3115_Restore(DrvContextTypeDef* handle, STC3115_ConfigData_TypeDef *ConfigData);
static int STC3115_Powerdown(DrvContextTypeDef* handle);
static int STC3115_conv(short value, unsigned short factor);
static int STC3115_Startup(DrvContextTypeDef* handle, STC3115_ConfigData_TypeDef *ConfigData);
static int STC3115_ReadBatteryData(DrvContextTypeDef* handle, STC3115_BatteryData_TypeDef *BatteryData);

static DrvStatusTypeDef STC3115_SetParam(DrvContextTypeDef* handle, STC3115_ConfigData_TypeDef *ConfigData);

#define GG_Delay HAL_Delay
extern void HAL_Delay(int delay);

/**
* @}
*/

/** @defgroup STC3115_Private_Variables
  * @{
  */
GG_Drv_t   STC3115_Drv =
{
  .Init = STC3115_Init,
  
  .DeInit = STC3115_DeInit,
  .Sensor_Enable = 0,
  .Sensor_Disable = 0,
  .Get_WhoAmI = STC3115_Get_WhoAmI,
  
  .Task = STC3115_Task,
  .Reset = STC3115_Reset,
  .Stop = STC3115_Stop,
  
  .GetSOC = STC3115_GetSOC,
  .GetOCV = STC3115_GetOCV,
  .GetCurrent = STC3115_GetCurrent,
  .GetTemperature = STC3115_GetTemperature,
  .GetVoltage = STC3115_GetVoltage,
  .GetChargeValue = STC3115_GetChargeValue,
  .GetPresence = STC3115_GetPresence,
  .GetAlarmStatus = STC3115_GetAlarmStatus,
  .GetITState = 0,
  
  .AlarmSetVoltageThreshold = 0,
  .AlarmSetSOCThreshold = 0,
  
  .GetIT = 0,
  .SetIT = 0,
  .StopIT = 0,
  .ClearIT = 0,
};

/**
* @}
*/


/** @defgroup STC3115_Private_Functions
  * @{
  */

/**
 * @brief Initializes STC3115 Gas Gauge. Hides low level device and battery details
 * @param None
 * @retval None
 */
static DrvStatusTypeDef STC3115_Init( DrvContextTypeDef* handle )
{
  int32_t res, loop;
  int32_t OCVOffset[16] = OCV_OFFSET_TAB;
  int vcount = 0;
  uint8_t res8;
  
  STC3115_ConfigData_TypeDef *ConfigData = &(( STC3115_Data_t * )(( GG_Data_t * )handle->pData)->pComponentData)->configData;
  STC3115_BatteryData_TypeDef *BatteryData = &(( STC3115_Data_t * )(( GG_Data_t * )handle->pData)->pComponentData)->batteryData;
  RAMData_TypeDef *RAMData = &(( STC3115_Data_t * )(( GG_Data_t * )handle->pData)->pComponentData)->ramData;
  
  /*** Fill configuration structure parameters ***/
  ConfigData->Vmode = VMODE;
  
  if(RSENSE != 0)
  {
    ConfigData->Rsense = RSENSE;
  }
  else
  {
    ConfigData->Rsense = 10; // default value to avoid division by 0
  }
  
  ConfigData->CC_cnf = (CAPACITY * ConfigData->Rsense * 250 + 6194) / 12389;
  
  if(RINT != 0)
  {
    ConfigData->VM_cnf = (CAPACITY * RINT * 50 + 24444) / 48889;
  }
  else
  {
    ConfigData->VM_cnf = (CAPACITY * 200 * 50 + 24444) / 48889; // default value
  }
  
  for(loop = 0; loop < 16; loop++)
  {
    if(OCVOffset[loop] > 127)
    {
      OCVOffset[loop] = 127;
    }
    if(OCVOffset[loop] < -127)
    {
      OCVOffset[loop] = -127;
    }
    ConfigData->OCVOffset[loop] = OCVOffset[loop];
  }
  
  ConfigData->Cnom = CAPACITY;
  ConfigData->RelaxCurrent = CAPACITY / 20;
  
  ConfigData->Alm_SOC = ALM_SOC;
  ConfigData->Alm_Vbat = ALM_VBAT;
  
  /*** Initialize Gas Gauge system ***/
  
  /* check STC3115 status */
  res = STC3115_Status(handle);
  if(res < 0)
  {
    return COMPONENT_ERROR;
  }
    
  /*Battery presence status init*/
  BatteryData->Presence = 1;
  
  /* check RAM data validity */
  STC3115_ReadRamData(handle, RAMData->db);
  
  STC3115_Read(handle, STC3115_REG_MODE, &res8, 1);
  res8 &= STC3115_GG_RUN;
  if(res8 != 0)   /* GasGauge is running */
  {
    STC3115_Read(handle, STC3115_REG_CTRL, &res8, 1);
    res8 &= (STC3115_BATFAIL | STC3115_PORDET);
    if(res8 == 0)   /* no GG reset has occurred */
    {
      if((RAMData->reg.TstWord == RAM_TSTWORD) && (STC3115_CalcRamCRC8(RAMData->db, RAM_SIZE) == 0))  /* RAM data is consistent */
      {
        if(RAMData->reg.STC3115_Status == STC3115_RUNNING)   /* last saved RAM state was for a valid measurement */
        {
          /* Recover from last SOC */
          res = STC3115_Restore(handle, ConfigData);
          if(res < 0)
            return COMPONENT_ERROR;
        }
      }
    }
  }
  
  /* Startup from known state */
  STC3115_Reset(handle);
  STC3115_InitRamData(handle, ConfigData);
  
  RAMData->reg.STC3115_Status = STC3115_INIT;
  STC3115_UpdateRamCRC(handle);
  STC3115_WriteRamData(handle, RAMData->db);
  
  /* Configure & Handle delays and Resets */
  STC3115_SetParam(handle, ConfigData);
  
  STC3115_ReadWord(handle, STC3115_REG_COUNTER, &vcount);
  while(vcount <= VCOUNT)
  {
    STC3115_Read(handle, STC3115_REG_MODE, &res8, 1);
    res8 &= STC3115_GG_RUN;
    while(res8 == 0)
    {
      STC3115_SetParam(handle, ConfigData);
      GG_Delay(200);
      STC3115_Read(handle, STC3115_REG_MODE, &res8, 1);
      res8 &= STC3115_GG_RUN;
    } 
    GG_Delay(200);
    STC3115_ReadWord(handle, STC3115_REG_COUNTER, &vcount);
  }
  
  handle->isInitialized = 1;
  return COMPONENT_OK;
}


/**
 * @brief Deinitialize the STC3115 sensor
 * @param handle the device handle
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
static DrvStatusTypeDef STC3115_DeInit( DrvContextTypeDef *handle )
{
  handle->isInitialized = 0;

  return COMPONENT_OK;
}


/**
 * @brief Enable the  STC3115 sensor
 * @param handle the device handle
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
//static DrvStatusTypeDef STC3115_Sensor_Enable( DrvContextTypeDef *handle )
//{
//  return COMPONENT_NOT_IMPLEMENTED;
//}


/**
 * @brief Disable the STC3115 sensor
 * @param handle the device handle
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
//static DrvStatusTypeDef STC3115_Sensor_Disable( DrvContextTypeDef *handle )
//{
//  return COMPONENT_NOT_IMPLEMENTED;
//}

/**
 * @brief Get the WHO_AM_I ID of the STC3115 sensor
 * @param handle the device handle
 * @param who_am_i pointer to the value of WHO_AM_I register
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
static DrvStatusTypeDef STC3115_Get_WhoAmI( DrvContextTypeDef *handle, uint8_t *who_am_i )
{
  if (STC3115_Read(handle, STC3115_REG_ID, who_am_i, 1) == 0)
  {
    return COMPONENT_OK;
  }
  else
  {
    return COMPONENT_ERROR;
  }
}


/**
 * @brief Task function for STC3115 Gas Gauge. Hides low level device and battery details
 * @param None
 * @retval Same as GasGauge_Task
 */
static DrvStatusTypeDef STC3115_Task( DrvContextTypeDef* handle, uint8_t *vm_mode)
{
  int res;

  STC3115_ConfigData_TypeDef *ConfigData = &(( STC3115_Data_t * )(( GG_Data_t * )handle->pData)->pComponentData)->configData;
  STC3115_BatteryData_TypeDef *BatteryData = &(( STC3115_Data_t * )(( GG_Data_t * )handle->pData)->pComponentData)->batteryData;
  RAMData_TypeDef *RAMData = &(( STC3115_Data_t * )(( GG_Data_t * )handle->pData)->pComponentData)->ramData;
  
  /* ----------------------------- System state verification ---------------------------- */
  /*Read STC3115 status registers */
  res = STC3115_Status(handle);
  
  if (res < 0) return(COMPONENT_ERROR); /* return if I2C error or STC3115 not responding */
  BatteryData->status = res;
  
  /* check STC3115 RAM status (battery has not been changed) */
  STC3115_ReadRamData(handle, RAMData->db);
  if ( (RAMData->reg.TstWord != RAM_TSTWORD) || (STC3115_CalcRamCRC8(RAMData->db, RAM_SIZE) != 0) )
  {
    /* if RAM non ok, reset it and set init state */
    STC3115_InitRamData(handle, ConfigData);
    RAMData->reg.STC3115_Status = STC3115_INIT;
  }
  
  /* check battery presence status */
  if ((BatteryData->status & (STC3115_BATFAIL << 8)) != 0)
  {
    /*Battery disconnection has been detected     */
    
    /*BATD pin level is over 1.61 or Vcc is below 2.7V  */
    BatteryData->Presence = 0;
    
    /*HW and SW state machine reset*/
    STC3115_Reset(handle);
    
    return (COMPONENT_ERROR);
  }
  
  /* check STC3115 running mode*/
  if ((BatteryData->status & STC3115_GG_RUN) == 0)
  {
    if(RAMData->reg.STC3115_Status == STC3115_RUNNING)
    {
      STC3115_Restore(handle, ConfigData);  /* if RUNNING state, restore STC3115*/
    }
    else
    {
      STC3115_Startup(handle, ConfigData);  /* if INIT state, initialize STC3115*/
    }
    
    RAMData->reg.STC3115_Status = STC3115_INIT;
  }
  
  /* --------------------------------- Read battery data ------------------------------- */
  
  res = STC3115_ReadBatteryData(handle, BatteryData);
  if (res != 0) return(COMPONENT_ERROR); /* abort in case of I2C failure */
  
  
  /* ------------------------------- battery data report ------------------------------- */
  /* check INIT state */
  if (RAMData->reg.STC3115_Status == STC3115_INIT)
  {
    /* INIT state, wait for current & temperature value available: */
    if (BatteryData->ConvCounter > VCOUNT)
    {
      RAMData->reg.STC3115_Status = STC3115_RUNNING;
      /*Battery is connected*/
      BatteryData->Presence = 1;
    }
  }
  
  if (RAMData->reg.STC3115_Status != STC3115_RUNNING)   /* not running : data partially availalble*/
  {
    BatteryData->ChargeValue = ConfigData->Cnom * BatteryData->SOC / MAX_SOC;
    BatteryData->Current = 0;
    BatteryData->Temperature = 250;
    BatteryData->RemTime = -1;
  }
  else /*STC3115 running */
  {
  
    /* ---------- process SW algorithms -------- */
    
    /*early empty compensation*/
    if (BatteryData->Voltage < APP_CUTOFF_VOLTAGE)
      BatteryData->SOC = 0;
    else if (BatteryData->Voltage < (APP_CUTOFF_VOLTAGE + 200))
      BatteryData->SOC = BatteryData->SOC * (BatteryData->Voltage - APP_CUTOFF_VOLTAGE) / 200;
      
    /* Battery charge value calculation */
    
    BatteryData->ChargeValue = ConfigData->Cnom * BatteryData->SOC / MAX_SOC;
    
    if ((BatteryData->status & STC3115_VMODE) == 0) /* mixed mode only*/
    {
    
      /*Lately fully compensation*/
      if ((BatteryData->status & STC3115_VMODE) == 0) /*running in mixed mode*/
      {
      
        if(BatteryData->Current > APP_EOC_CURRENT && BatteryData->SOC > 990)
        {
          BatteryData->SOC = 990;
          STC3115_WriteWord(handle, STC3115_REG_SOC, 50688);  /* 99% */
        }
      }
      
      /*Remaining time calculation*/
      if(BatteryData->Current < 0)
      {
      
        BatteryData->RemTime = (BatteryData->RemTime * 4 + BatteryData->ChargeValue / BatteryData->Current * 60 ) / 5;
        if( BatteryData->RemTime  < 0)
          BatteryData->RemTime = -1; /* means no estimated time available */
      }
      else
        BatteryData->RemTime = -1; /* means no estimated time available */
        
    }
    else /* voltage mode only */
    {
      BatteryData->Current = 0;
      BatteryData->RemTime = -1;
    }
    
    //SOC min/max clamping
    if(BatteryData->SOC > 1000) BatteryData->SOC = MAX_SOC;
    if(BatteryData->SOC < 0) BatteryData->SOC = 0;
    
  }
  /* save SOC */
  RAMData->reg.HRSOC = BatteryData->HRSOC;
  RAMData->reg.SOC = (BatteryData->SOC + 5) / 10;
  STC3115_UpdateRamCRC(handle);
  STC3115_WriteRamData(handle, RAMData->db);
  
  if (RAMData->reg.STC3115_Status == STC3115_RUNNING)
  {
    if((BatteryData->status & (STC3115_GG_VM << 8)) == 0)
      *vm_mode=MIXED_MODE; //return(2); // Coulomb counter mode
    else
      *vm_mode=VM_MODE; //return(1); // Voltage mode
  }
  return COMPONENT_OK;
}

/**
 * @brief Resets Gas Gauge
 * @param None
 * @retval None
 */
static DrvStatusTypeDef STC3115_Reset(DrvContextTypeDef* handle)
{
  int res;
  RAMData_TypeDef *RAMData = &(( STC3115_Data_t * )(( GG_Data_t * )handle->pData)->pComponentData)->ramData;
  
  /* reset RAM */
  RAMData->reg.TstWord = 0;
  RAMData->reg.STC3115_Status = 0;
  res = STC3115_WriteRamData(handle, RAMData->db);
  if(res != OK) return COMPONENT_ERROR;
  /* reset STC3115*/
  uint8_t data = STC3115_PORDET;
  res = STC3115_Write(handle, STC3115_REG_CTRL, &data, 1);  /*   set soft POR */
  
  return COMPONENT_OK;
}

/**
 * @brief Stops Gas Gauge
 * @param None
 * @retval Same as GasGauge_Stop
 */
static DrvStatusTypeDef STC3115_Stop(DrvContextTypeDef* handle)
{
  int res;
  RAMData_TypeDef *RAMData = &(( STC3115_Data_t * )(( GG_Data_t * )handle->pData)->pComponentData)->ramData;
  
  /*Save context in RAM*/
  STC3115_ReadRamData(handle, RAMData->db);
  RAMData->reg.STC3115_Status = STC3115_POWERDN;
  /* update the crc */
  STC3115_UpdateRamCRC(handle);
  STC3115_WriteRamData(handle, RAMData->db);
  
  /*STC3115 Power down*/
  res = STC3115_Powerdown(handle);
  if (res != 0) return (COMPONENT_ERROR); /* error */
  
  return(COMPONENT_OK);
}

/**
 * @brief Returns battery State of Charge
 * @param None
 * @retval Battery State of Charge value
 */
static DrvStatusTypeDef STC3115_GetSOC( DrvContextTypeDef* handle, uint32_t* SOC)
{
  *SOC = ((( STC3115_Data_t * )(( GG_Data_t * )handle->pData)->pComponentData)->batteryData.SOC + 5) / 10;
  return COMPONENT_OK;
}

/**
 * @brief Returns battery Open circuit Voltage
 * @param None
 * @retval Open circuit Voltage value
 */
static DrvStatusTypeDef STC3115_GetOCV(DrvContextTypeDef* handle, uint32_t* OCV)
{
  *OCV = (( STC3115_Data_t * )(( GG_Data_t * )handle->pData)->pComponentData)->batteryData.OCV;
  return COMPONENT_OK;
}

/**
 * @brief Returns absorbed current
 * @param None
 * @retval Current value
 */
static DrvStatusTypeDef STC3115_GetCurrent(DrvContextTypeDef* handle, int32_t* Current)
{
  *Current = (( STC3115_Data_t * )(( GG_Data_t * )handle->pData)->pComponentData)->batteryData.Current;
  return COMPONENT_OK;
}

/**
 * @brief Returns temperature value
 * @param None
 * @retval Temperature value
 */
static DrvStatusTypeDef STC3115_GetTemperature(DrvContextTypeDef* handle, int32_t* Temperature)
{
  *Temperature = (( STC3115_Data_t * )(( GG_Data_t * )handle->pData)->pComponentData)->batteryData.Temperature;
  return COMPONENT_OK;
}

/**
 * @brief Returns battery voltage
 * @param None
 * @retval Battery voltage value
 */
static DrvStatusTypeDef STC3115_GetVoltage(DrvContextTypeDef* handle, uint32_t* Voltage)
{
  *Voltage = (( STC3115_Data_t * )(( GG_Data_t * )handle->pData)->pComponentData)->batteryData.Voltage;
  return COMPONENT_OK;
}

/**
 * @brief Returns remaining battery charge value
 * @param None
 * @retval battery charge value
 */
static DrvStatusTypeDef STC3115_GetChargeValue(DrvContextTypeDef* handle, uint32_t* ChargeValue)
{
  *ChargeValue = (( STC3115_Data_t * )(( GG_Data_t * )handle->pData)->pComponentData)->batteryData.ChargeValue;
  return COMPONENT_OK;
}

/**
 * @brief Tells whether battery is present or not
 * @param None
 * @retval 0 if battery is not present, 1 if present
 */
static DrvStatusTypeDef STC3115_GetPresence(DrvContextTypeDef* handle, uint32_t* Presence)
{
  *Presence = (( STC3115_Data_t * )(( GG_Data_t * )handle->pData)->pComponentData)->batteryData.Presence;
  return COMPONENT_OK;
}

/**
 * @brief Returns Alarm bits status
 * @param None
 * @retval STC3115 Alarm bits status
 */
static DrvStatusTypeDef STC3115_GetAlarmStatus(DrvContextTypeDef* handle, uint32_t* status)
{
  *status = ((( STC3115_Data_t * )(( GG_Data_t * )handle->pData)->pComponentData)->batteryData.status >> 13) & 0x3;
  return COMPONENT_OK;
}

/**
 * @brief Read the STC3115 status
 * @param None
 * @retval status word (REG_MODE / REG_CTRL), -1 if error
 */
static int STC3115_Status( DrvContextTypeDef* handle )
{
  int value;
  uint8_t data;
  
  /* First, check the presence of the STC3115 by reading first byte of dev. ID */
  if(STC3115_Read(handle, STC3115_REG_ID, &data, 1) == -1)
    return -1;
  if (data != STC3115_ID)
    return -1;
    
  /* read REG_MODE and REG_CTRL */
  STC3115_ReadWord(handle, STC3115_REG_MODE, &value);
  value &= 0x7f7f;
  
  return value;
}

/**
 * @brief Calculate the CRC8
 * @param pointer to byte array,
 * @param n: number of bytes
 * @retval CRC value
 */
static int STC3115_CalcRamCRC8(unsigned char *data, int n)
{
  int crc = 0; /* initial value */
  int i, j;
  
  for (i = 0; i < n; i++)
  {
    crc ^= data[i];
    for (j = 0; j < 8; j++)
    {
      crc <<= 1;
      if (crc & 0x100)  crc ^= 7;
    }
  }
  return(crc & 255);
}


/**
 * @brief calculate the RAM CRC
 * @param None
 * @retval CRC value
 */
static int STC3115_UpdateRamCRC( DrvContextTypeDef* handle )
{
  int res;
  RAMData_TypeDef *RAMData = &(( STC3115_Data_t * )(( GG_Data_t * )handle->pData)->pComponentData)->ramData;
  
  res = STC3115_CalcRamCRC8(RAMData->db, RAM_SIZE - 1);
  RAMData->db[RAM_SIZE - 1] = res; /* last byte holds the CRC */
  return(res);
}


/**
 * @brief Initialize the STC3115 RAM registers with valid test word and CRC
 * @param ConfigData Pointer to STC3115_ConfigData_TypeDef structure
 * @retval None
 */
static void STC3115_InitRamData(DrvContextTypeDef* handle, STC3115_ConfigData_TypeDef *ConfigData)
{
  int index;
  RAMData_TypeDef *RAMData = &(( STC3115_Data_t * )(( GG_Data_t * )handle->pData)->pComponentData)->ramData;
  
  //Set full RAM tab to 0
  for (index = 0; index < RAM_SIZE; index++)
  {
    RAMData->db[index] = 0;
  }
  //Fill RAM regs
  RAMData->reg.TstWord = RAM_TSTWORD; /* Fixed word to check RAM integrity */
  RAMData->reg.CC_cnf = ConfigData->CC_cnf;
  RAMData->reg.VM_cnf = ConfigData->VM_cnf;
  /* update the crc */
  STC3115_UpdateRamCRC(handle);
}


/**
 * @brief Utility function to write the RAM data into STC3115
 * @param RamData Pointer to read STC3115 RAM data array from
 * @retval I2C error code
 */
static int STC3115_WriteRamData(DrvContextTypeDef* handle, unsigned char *RamData)
{
  if (Sensor_IO_Write(handle, STC3115_REG_RAM, RamData, RAM_SIZE))
  {
    return -1;
  }
  else
  {
    return 0;
  }
}


/**
 * @brief utility function to read the RAM data from STC3115
 * @param RamData Pointer to store STC3115 RAM data array to
 * @retval I2C error code
 */
static int STC3115_ReadRamData(DrvContextTypeDef* handle, unsigned char *RamData)
{
  if (Sensor_IO_Read(handle, STC3115_REG_RAM, RamData, RAM_SIZE))
  {
    return -1;
  }
  else
  {
    return 0;
  }
}

/**
 * @brief Utility function to read the battery data from STC3115 to be called every 5s or so
 * @param Pointer to STC3115_BatteryData_TypeDef structure
 * @retval error status (OK, !OK)
 */
static int STC3115_ReadBatteryData(DrvContextTypeDef* handle, STC3115_BatteryData_TypeDef *BatteryData)
{
  unsigned char data[16];
  int res;
  int value;
  
  //STC3115_Read(DrvContextTypeDef* handle, uint8_t RegAddress, uint8_t *data, uint16_t NumByteToWrite)
  /* read STC3115 registers 0 to 14 */
  res = STC3115_Read(handle, 0, data, 15);
  
  if (res < 0) return(res); /* read failed */
  
  /* fill the battery status data */
  /* SOC */
  value = data[3];
  value = (value << 8) + data[2];
  BatteryData->HRSOC = value;     /* result in 1/512% */
  BatteryData->SOC = (value * 10 + 256) / 512; /* result in 0.1% */
  
  /* conversion counter */
  value = data[5];
  value = (value << 8) + data[4];
  BatteryData->ConvCounter = value;
  
  /* current */
  value = data[7];
  value = (value << 8) + data[6];
  value &= 0x3fff;   /* mask unused bits */
  if (value >= 0x2000) value = value - 0x4000; /* convert to signed value */
  BatteryData->Current = STC3115_conv(value, CurrentFactor / RSENSE); /* result in mA */
  
  /* voltage */
  value = data[9];
  value = (value << 8) + data[8];
  value &= 0x0fff; /* mask unused bits */
  if (value >= 0x0800) value -= 0x1000; /* convert to signed value */
  value = STC3115_conv(value, VoltageFactor); /* result in mV */
  BatteryData->Voltage = value;  /* result in mV */
  
  /* temperature */
  value = data[10];
  if (value >= 0x80) value -= 0x100; /* convert to signed value */
  BatteryData->Temperature = value * 10; /* result in 0.1°C */
  
  /* OCV */
  value = data[14];
  value = (value << 8) + data[13];
  value &= 0x3fff; /* mask unused bits */
  if (value >= 0x02000) value -= 0x4000; /* convert to signed value */
  value = STC3115_conv(value, VoltageFactor);
  value = (value + 2) / 4; /* divide by 4 with rounding */
  BatteryData->OCV = value;  /* result in mV */
  
  return(OK);
}

/**
 * @brief Restore STC3115 state
 * @param Pointer to STC3115 Configuration Data structure
 * @retval Always 0
 */
static int STC3115_Restore(DrvContextTypeDef* handle, STC3115_ConfigData_TypeDef *ConfigData)
{
  int res;
  RAMData_TypeDef *RAMData = &(( STC3115_Data_t * )(( GG_Data_t * )handle->pData)->pComponentData)->ramData;
  
  /* check STC310x status */
  res = STC3115_Status(handle);
  if (res < 0) return(res);
  
  STC3115_SetParam(handle, ConfigData);  /* set STC3115 parameters  */
  
  /* restore SOC from RAM data */
  res = 0;
  
  if (RAMData->reg.STC3115_Status == STC3115_RUNNING)
  {
    if (RAMData->reg.HRSOC != 0)
    {
      res = STC3115_WriteWord(handle, STC3115_REG_SOC, RAMData->reg.HRSOC);  /* restore HRSOC */
    }
  }
  return(res);
}

/**
 * @brief Stop the STC3115 at application power down
 * @param None
 * @retval Ok if success, error code otherwise
 */
static int STC3115_Powerdown(DrvContextTypeDef* handle)
{
  int res;
  uint8_t data;
  
  /* write 0x01 into the REG_CTRL to release IO0 pin open, */
  data = 0x01;
  STC3115_Write(handle, STC3115_REG_CTRL, &data, 1);
  
  /* write 0 into the REG_MODE register to put the STC3115 in standby mode */
  data = 0x00;
  res = STC3115_Write(handle, STC3115_REG_MODE, &data, 1);
  if (res!= OK) return (res);
  
  return (OK);
}

/**
 * @brief Convert a raw 16-bit value from STC3115 registers into user units (mA, mAh, mV, °C)
 * @param value 
 * @param factor 
 * @retval Converted value (result = value * factor / 4096)
 */
static int STC3115_conv(short value, unsigned short factor)
{
  int v;
  
  v = ( (long) value * factor ) >> 11;
  v = (v+1)/2;
  
  return (v);
}

/**
 * @brief Initialize and start the STC3115 at application startup
 * @param Pointer to STC3115 Configuration Data structure
 * @retval 0 if ok, -1 if error
 */
static int STC3115_Startup(DrvContextTypeDef* handle, STC3115_ConfigData_TypeDef *ConfigData)
{
  int res;
  int ocv;
  
  /* check STC310x status */
  res = STC3115_Status(handle);
  if (res < 0) return(res);
  
  /* read OCV */
  STC3115_ReadWord(handle, STC3115_REG_OCV, &ocv);
  
  STC3115_SetParam(handle, ConfigData);  /* set STC3115 parameters  */
  
  /* rewrite ocv to start SOC with updated OCV curve */
  STC3115_WriteWord(handle, STC3115_REG_OCV, ocv);
  
  return 0;
}

/**
 * @brief Utility function to read the value stored in one register
 * @param  RegAddress STC3115 register to read
 * @param  value pointer to integer where to store data
 * @retval Error code: 0 ok, -1 error
 */
static int STC3115_Read(DrvContextTypeDef* handle, uint8_t RegAddress, uint8_t *data, uint16_t NumByteToRead)
{
  if (Sensor_IO_Read(handle, RegAddress, data, NumByteToRead))
  {
    return -1;
  }
  else
  {
    return 0;
  }
}

/**
 * @brief utility function to write a 8-bit value into a register
 * @param RegAddress STC3115 register
 * @param value value to write
 * @retval Error code: 0 ok, -1 error
 */
static int STC3115_Write(DrvContextTypeDef* handle, uint8_t RegAddress, uint8_t *data, uint16_t NumByteToWrite)
{
  if (Sensor_IO_Write(handle, RegAddress, data, NumByteToWrite))
  {
    return -1;
  }
  else
  {
    return 0;
  }
}

/**
 * @brief Utility function to read the value stored in one register pair
 * @param RegAddress STC3115 register
 * @param value pointer to integer value where to store data
 * @retval Error code: 0 ok, -1 error
 */
static int STC3115_ReadWord(DrvContextTypeDef* handle, uint8_t RegAddress, int* value)
{
  uint16_t tmp;
  
  if (Sensor_IO_Read(handle, RegAddress, (uint8_t*)&tmp, 2))
  {
    return -1;
  }
  else
  {
    *value = tmp & 0xffff;
    return 0;
  }
}


/**
 * @brief Utility function to write a 16-bit value into a register pair
 * @param RegAddress STC3115 register
 * @param value value to write
 * @retval Error code: 0 ok, -1 error
 */
static int STC3115_WriteWord(DrvContextTypeDef* handle, uint8_t RegAddress, int value)
{
  uint16_t tmp = value & 0xffff;
  
  if (Sensor_IO_Write(handle, RegAddress, (uint8_t*)&tmp, 2))
  {
    return -1;
  }
  else
  {
    return 0;
  }
}

/**
 * @brief Initialize the STC3115 parameters
 * @param ConfigData Pointer to STC3115 Configuration Data structure
 * @retval None
 */
static DrvStatusTypeDef STC3115_SetParam(DrvContextTypeDef* handle, STC3115_ConfigData_TypeDef *ConfigData)
{
  uint8_t data;
  RAMData_TypeDef *RAMData = &(( STC3115_Data_t * )(( GG_Data_t * )handle->pData)->pComponentData)->ramData;
  
  /* set GG_RUN=0 before changing algo parameters */
  data = STC3115_VMODE;
  STC3115_Write(handle, STC3115_REG_MODE, &data, 1);
  
  /* init OCV curve */
  STC3115_Write(handle, STC3115_REG_OCVTAB, (unsigned char *) ConfigData->OCVOffset, OCVTAB_SIZE);
  
  /* set alm level if different from default */
  if (ConfigData->Alm_SOC != 0 )
  {
    data = ConfigData->Alm_SOC * 2;
    STC3115_Write(handle, STC3115_REG_ALARM_SOC, &data, 1);
  }
  if (ConfigData->Alm_Vbat != 0 )
  {
    data = ((long)(ConfigData->Alm_Vbat << 9) / VoltageFactor); /* LSB=8*2.2mV */
    STC3115_Write(handle, STC3115_REG_ALARM_VOLTAGE, &data, 1);
  }
  
  /* relaxation timer */
  if (ConfigData->Rsense != 0 )
  {
    data = ((long)(ConfigData->RelaxCurrent << 9) / (CurrentFactor / ConfigData->Rsense));  /* LSB=8*5.88uV */
    STC3115_Write(handle, STC3115_REG_CURRENT_THRES, &data, 1);
  }
  
  /* set parameters if different from default, only if a restart is done (battery change) */
  if (RAMData->reg.CC_cnf != 0 ) STC3115_WriteWord(handle, STC3115_REG_CC_CNF, RAMData->reg.CC_cnf);
  if (RAMData->reg.VM_cnf != 0 ) STC3115_WriteWord(handle, STC3115_REG_VM_CNF, RAMData->reg.VM_cnf);
  
  data = 0x03;
  STC3115_Write(handle, STC3115_REG_CTRL, &data, 1); /*   clear PORDET, BATFAIL, free ALM pin, reset conv counter */
  
  data = STC3115_GG_RUN | (STC3115_VMODE * ConfigData->Vmode) | (STC3115_ALM_ENA * ALM_EN);
  STC3115_Write(handle, STC3115_REG_MODE, &data, 1);  /*   set GG_RUN=1, set mode, set alm enable */
  
  return COMPONENT_OK;
}

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

