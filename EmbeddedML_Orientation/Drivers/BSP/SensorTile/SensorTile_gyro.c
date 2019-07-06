/**
 ******************************************************************************
 * @file   SensorTile_gyro.c
* @author  Central Labs
* @version V1.2.0
 * @date    10-Nov-2016
* @brief   This file provides the Gyroscope driver for the Sensor Tile
*          board
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
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
#include "SensorTile_gyro.h"



/** @addtogroup BSP 
 * @{
 */

/** @addtogroup SENSORTILE 
 * @{
 */

/** @addtogroup SENSORTILE_GYRO 
* @brief This file provides a set of firmware functions to manage MEMS Gyroscope

 * @{
 */

/** @addtogroup SENSORTILE_GYRO_Private_Variables SENSORTILE_GYRO Private variables
 * @{
 */

static DrvContextTypeDef GYRO_SensorHandle[ GYRO_SENSORS_MAX_NUM ];
static GYRO_Data_t GYRO_Data[ GYRO_SENSORS_MAX_NUM ]; // Gyroscope - all.
static LSM6DSM_G_Data_t LSM6DSM_G_0_Data; // Gyroscope - sensor 0.
/**
 * @}
 */

/** @addtogroup SENSORTILE_GYRO_Private_FunctionPrototypes SENSORTILE_GYRO Private function prototypes
 * @{
 */
static DrvStatusTypeDef BSP_LSM6DSM_GYRO_Init( void **handle );
/**
 * @}
 */

/** @addtogroup SENSORTILE_GYRO_Exported_Functions SENSORTILE_GYRO Exported functions
 * @{
 */

/**
 * @brief Initialize a gyroscope sensor
 * @param id the gyroscope sensor identifier
 * @param handle the device handle
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
DrvStatusTypeDef BSP_GYRO_Init( GYRO_ID_t id, void **handle )
{

  *handle = NULL;
  
  switch(id)
  {
    case GYRO_SENSORS_AUTO:
    default:
    {
      if( BSP_LSM6DSM_GYRO_Init(handle) == COMPONENT_ERROR )
      {
        return COMPONENT_ERROR;
      }
      break;
    }
    case LSM6DSM_G_0:
    {
      if( BSP_LSM6DSM_GYRO_Init(handle) == COMPONENT_ERROR )
      {
        return COMPONENT_ERROR;
      }
      break;
    }
  }
  
  return COMPONENT_OK;
}



static DrvStatusTypeDef BSP_LSM6DSM_GYRO_Init( void **handle )
{
  GYRO_Drv_t *driver = NULL;
  uint8_t data = 0x0C;
  
  if(GYRO_SensorHandle[ LSM6DSM_G_0 ].isInitialized == 1)
  {
    /* We have reached the max num of instance for this component */
    return COMPONENT_ERROR;
  }
  
  if ( Sensor_IO_SPI_Init() == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }
  
  /* Setup sensor handle. */
  /* Gyroscope - sensor 0 */
  GYRO_SensorHandle[ LSM6DSM_G_0 ].who_am_i      = LSM6DSM_ACC_GYRO_WHO_AM_I;
  GYRO_SensorHandle[ LSM6DSM_G_0 ].ifType        = 1; // SPI interface
  GYRO_SensorHandle[ LSM6DSM_G_0 ].address       = LSM6DSM_ACC_GYRO_I2C_ADDRESS_HIGH;
  GYRO_SensorHandle[ LSM6DSM_G_0 ].spiDevice     = LSM6DSM;
  GYRO_SensorHandle[ LSM6DSM_G_0 ].instance      = LSM6DSM_G_0;
  GYRO_SensorHandle[ LSM6DSM_G_0 ].isInitialized = 0;
  GYRO_SensorHandle[ LSM6DSM_G_0 ].isEnabled     = 0;
  GYRO_SensorHandle[ LSM6DSM_G_0 ].isCombo       = 1;
  GYRO_SensorHandle[ LSM6DSM_G_0 ].pData         = ( void * )&GYRO_Data[ LSM6DSM_G_0 ];
  GYRO_SensorHandle[ LSM6DSM_G_0 ].pVTable       = ( void * )&LSM6DSM_G_Drv;
  GYRO_SensorHandle[ LSM6DSM_G_0 ].pExtVTable    = 0;

  LSM6DSM_G_0_Data.comboData = &LSM6DSM_Combo_Data[0];
  GYRO_Data[ LSM6DSM_G_0 ].pComponentData = ( void * )&LSM6DSM_G_0_Data;
  GYRO_Data[ LSM6DSM_G_0 ].pExtData       = 0;
  
  *handle = (void *)&GYRO_SensorHandle[ LSM6DSM_G_0 ];
  
  Sensor_IO_SPI_CS_Init(*handle);
  
  if(LSM6DSM_Combo_Data[0].isAccInitialized == 0)
  { 
    // SPI Serial Interface Mode selection --> 3Wires
    if( Sensor_IO_Write(*handle, LSM6DSM_ACC_GYRO_CTRL3_C, &data, 1) )
    {
      return COMPONENT_ERROR;
    }
  }
  
  driver = ( GYRO_Drv_t * )((DrvContextTypeDef *)(*handle))->pVTable;
  
  if ( driver->Init == NULL )
  {
    memset((*handle), 0, sizeof(DrvContextTypeDef));
    *handle = NULL;
    return COMPONENT_ERROR;
  }
  
  if ( driver->Init( (DrvContextTypeDef *)(*handle) ) == COMPONENT_ERROR )
  {
    memset((*handle), 0, sizeof(DrvContextTypeDef));
    *handle = NULL;
    return COMPONENT_ERROR;
  }
  
  /* Disable I2C interface */
  if ( LSM6DSM_ACC_GYRO_W_I2C_DISABLE( *handle, LSM6DSM_ACC_GYRO_I2C_DISABLE_SPI_ONLY ) == MEMS_ERROR )
  {
    return COMPONENT_ERROR;
  }
  
  return COMPONENT_OK;
}


/**
 * @brief Deinitialize a gyroscope sensor
 * @param handle the device handle
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
DrvStatusTypeDef BSP_GYRO_DeInit( void **handle )
{

  DrvContextTypeDef *ctx = (DrvContextTypeDef *)(*handle);
  GYRO_Drv_t *driver = NULL;
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( GYRO_Drv_t * )ctx->pVTable;
  
  if ( driver->DeInit == NULL )
  {
    return COMPONENT_ERROR;
  }
  
  if ( driver->DeInit( ctx ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }
  
  memset(ctx, 0, sizeof(DrvContextTypeDef));
  
  *handle = NULL;
  
  return COMPONENT_OK;
}



/**
 * @brief Enable gyroscope sensor
 * @param handle the device handle
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
DrvStatusTypeDef BSP_GYRO_Sensor_Enable( void *handle )
{

  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  GYRO_Drv_t *driver = NULL;
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( GYRO_Drv_t * )ctx->pVTable;
  
  if ( driver->Sensor_Enable == NULL )
  {
    return COMPONENT_ERROR;
  }
  
  if ( driver->Sensor_Enable( ctx ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }
  
  return COMPONENT_OK;
}



/**
 * @brief Disable gyroscope sensor
 * @param handle the device handle
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
DrvStatusTypeDef BSP_GYRO_Sensor_Disable( void *handle )
{

  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  GYRO_Drv_t *driver = NULL;
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( GYRO_Drv_t * )ctx->pVTable;
  
  if ( driver->Sensor_Disable == NULL )
  {
    return COMPONENT_ERROR;
  }
  
  if ( driver->Sensor_Disable( ctx ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }
  
  return COMPONENT_OK;
}


/**
 * @brief Check if the gyroscope sensor is initialized
 * @param handle the device handle
 * @param status the pointer to the initialization status
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
DrvStatusTypeDef BSP_GYRO_IsInitialized( void *handle, uint8_t *status )
{
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  if ( status == NULL )
  {
    return COMPONENT_ERROR;
  }
  
  *status = ctx->isInitialized;
  
  return COMPONENT_OK;
}


/**
 * @brief Check if the gyroscope sensor is enabled
 * @param handle the device handle
 * @param status the pointer to the enable status
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
DrvStatusTypeDef BSP_GYRO_IsEnabled( void *handle, uint8_t *status )
{
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  if ( status == NULL )
  {
    return COMPONENT_ERROR;
  }
  
  *status = ctx->isEnabled;
  
  return COMPONENT_OK;
}


/**
 * @brief Check if the gyroscope sensor is combo
 * @param handle the device handle
 * @param status the pointer to the combo status
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
DrvStatusTypeDef BSP_GYRO_IsCombo( void *handle, uint8_t *status )
{
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  if ( status == NULL )
  {
    return COMPONENT_ERROR;
  }
  
  *status = ctx->isCombo;
  
  return COMPONENT_OK;
}


/**
 * @brief Get the gyroscope sensor instance
 * @param handle the device handle
 * @param instance the pointer to the device instance
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
DrvStatusTypeDef BSP_GYRO_Get_Instance( void *handle, uint8_t *instance )
{
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  if ( instance == NULL )
  {
    return COMPONENT_ERROR;
  }
  
  *instance = ctx->instance;
  
  return COMPONENT_OK;
}



/**
 * @brief Get the WHO_AM_I ID of the gyroscope sensor
 * @param handle the device handle
 * @param who_am_i pointer to the value of WHO_AM_I register
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
DrvStatusTypeDef BSP_GYRO_Get_WhoAmI( void *handle, uint8_t *who_am_i )
{

  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  GYRO_Drv_t *driver = NULL;
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( GYRO_Drv_t * )ctx->pVTable;
  
  if ( who_am_i == NULL )
  {
    return COMPONENT_ERROR;
  }
  if ( driver->Get_WhoAmI == NULL )
  {
    return COMPONENT_ERROR;
  }
  if ( driver->Get_WhoAmI( ctx, who_am_i ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }
  
  return COMPONENT_OK;
}



/**
 * @brief Check the WHO_AM_I ID of the gyroscope sensor
 * @param handle the device handle
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
DrvStatusTypeDef BSP_GYRO_Check_WhoAmI( void *handle )
{

  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  GYRO_Drv_t *driver = NULL;
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( GYRO_Drv_t * )ctx->pVTable;
  
  if ( driver->Check_WhoAmI == NULL )
  {
    return COMPONENT_ERROR;
  }
  if ( driver->Check_WhoAmI( ctx ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }
  
  return COMPONENT_OK;
}



/**
 * @brief Get the gyroscope sensor axes
 * @param handle the device handle
 * @param angular_velocity pointer where the values of the axes are written [mdps]
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
DrvStatusTypeDef BSP_GYRO_Get_Axes( void *handle, SensorAxes_t *angular_velocity )
{

  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  GYRO_Drv_t *driver = NULL;
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( GYRO_Drv_t * )ctx->pVTable;
  
  if ( angular_velocity == NULL )
  {
    return COMPONENT_ERROR;
  }
  if ( driver->Get_Axes == NULL )
  {
    return COMPONENT_ERROR;
  }
  if ( driver->Get_Axes( ctx, angular_velocity ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }
  
  return COMPONENT_OK;
}



/**
 * @brief Get the gyroscope sensor raw axes
 * @param handle the device handle
 * @param value pointer where the raw values of the axes are written
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
DrvStatusTypeDef BSP_GYRO_Get_AxesRaw( void *handle, SensorAxesRaw_t *value )
{

  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  GYRO_Drv_t *driver = NULL;
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( GYRO_Drv_t * )ctx->pVTable;
  
  if ( value == NULL )
  {
    return COMPONENT_ERROR;
  }
  
  if ( driver->Get_AxesRaw == NULL )
  {
    return COMPONENT_ERROR;
  }
  
  if ( driver->Get_AxesRaw( ctx, value ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }
  
  return COMPONENT_OK;
}


/**
 * @brief Get the gyroscope sensor sensitivity
 * @param handle the device handle
 * @param sensitivity pointer where the sensitivity value is written [mdps/LSB]
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
DrvStatusTypeDef BSP_GYRO_Get_Sensitivity( void *handle, float *sensitivity )
{

  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  GYRO_Drv_t *driver = NULL;
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( GYRO_Drv_t * )ctx->pVTable;
  
  if ( sensitivity == NULL )
  {
    return COMPONENT_ERROR;
  }
  if ( driver->Get_Sensitivity == NULL )
  {
    return COMPONENT_ERROR;
  }
  if ( driver->Get_Sensitivity( ctx, sensitivity ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }
  
  return COMPONENT_OK;
}



/**
 * @brief Get the gyroscope sensor output data rate
 * @param handle the device handle
 * @param odr pointer where the output data rate is written
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
DrvStatusTypeDef BSP_GYRO_Get_ODR( void *handle, float *odr )
{

  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  GYRO_Drv_t *driver = NULL;
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( GYRO_Drv_t * )ctx->pVTable;
  
  if ( odr == NULL )
  {
    return COMPONENT_ERROR;
  }
  if ( driver->Get_ODR == NULL )
  {
    return COMPONENT_ERROR;
  }
  if ( driver->Get_ODR( ctx, odr ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }
  
  return COMPONENT_OK;
}



/**
 * @brief Set the gyroscope sensor output data rate
 * @param handle the device handle
 * @param odr the functional output data rate to be set
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
DrvStatusTypeDef BSP_GYRO_Set_ODR( void *handle, SensorOdr_t odr )
{

  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  GYRO_Drv_t *driver = NULL;
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( GYRO_Drv_t * )ctx->pVTable;
  
  if ( driver->Set_ODR == NULL )
  {
    return COMPONENT_ERROR;
  }
  if ( driver->Set_ODR( ctx, odr ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }
  
  return COMPONENT_OK;
}



/**
 * @brief Set the gyroscope sensor output data rate
 * @param handle the device handle
 * @param odr the output data rate value to be set
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
DrvStatusTypeDef BSP_GYRO_Set_ODR_Value( void *handle, float odr )
{

  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  GYRO_Drv_t *driver = NULL;
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( GYRO_Drv_t * )ctx->pVTable;
  
  if ( driver->Set_ODR_Value == NULL )
  {
    return COMPONENT_ERROR;
  }
  if ( driver->Set_ODR_Value( ctx, odr ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }
  
  return COMPONENT_OK;
}



/**
 * @brief Get the gyroscope sensor full scale
 * @param handle the device handle
 * @param fullScale pointer where the full scale is written
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
DrvStatusTypeDef BSP_GYRO_Get_FS( void *handle, float *fullScale )
{

  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  GYRO_Drv_t *driver = NULL;
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( GYRO_Drv_t * )ctx->pVTable;
  
  if ( fullScale == NULL )
  {
    return COMPONENT_ERROR;
  }
  if ( driver->Get_FS == NULL )
  {
    return COMPONENT_ERROR;
  }
  if ( driver->Get_FS( ctx, fullScale ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }
  
  return COMPONENT_OK;
}



/**
 * @brief Set the gyroscope sensor full scale
 * @param handle the device handle
 * @param fullScale the functional full scale to be set
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
DrvStatusTypeDef BSP_GYRO_Set_FS( void *handle, SensorFs_t fullScale )
{

  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  GYRO_Drv_t *driver = NULL;
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( GYRO_Drv_t * )ctx->pVTable;
  
  if ( driver->Set_FS == NULL )
  {
    return COMPONENT_ERROR;
  }
  if ( driver->Set_FS( ctx, fullScale ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }
  
  return COMPONENT_OK;
}



/**
 * @brief Set the gyroscope sensor full scale
 * @param handle the device handle
 * @param fullScale the full scale value to be set
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
DrvStatusTypeDef BSP_GYRO_Set_FS_Value( void *handle, float fullScale )
{

  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  GYRO_Drv_t *driver = NULL;
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( GYRO_Drv_t * )ctx->pVTable;
  
  if ( driver->Set_FS_Value == NULL )
  {
    return COMPONENT_ERROR;
  }
  if ( driver->Set_FS_Value( ctx, fullScale ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }
  
  return COMPONENT_OK;
}



/**
 * @brief Get the gyroscope sensor axes status
 * @param handle the device handle
 * @param xyz_enabled the pointer to the axes enabled/disabled status
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
DrvStatusTypeDef BSP_GYRO_Get_Axes_Status( void *handle, uint8_t *xyz_enabled )
{

  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  GYRO_Drv_t *driver = NULL;
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( GYRO_Drv_t * )ctx->pVTable;
  
  if ( xyz_enabled == NULL )
  {
    return COMPONENT_ERROR;
  }
  if ( driver->Get_Axes_Status == NULL )
  {
    return COMPONENT_ERROR;
  }
  if ( driver->Get_Axes_Status( ctx, xyz_enabled ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }
  
  return COMPONENT_OK;
}


/**
 * @brief Read the data from register
 * @param handle the device handle
 * @param reg register address
 * @param data register data
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
DrvStatusTypeDef BSP_GYRO_Read_Reg( void *handle, uint8_t reg, uint8_t *data )
{

  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  GYRO_Drv_t *driver = NULL;

  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }

  driver = ( GYRO_Drv_t * )ctx->pVTable;

  if(data == NULL)
  {
    return COMPONENT_ERROR;
  }

  if ( driver->Read_Reg == NULL )
  {
    return COMPONENT_ERROR;
  }

  if ( driver->Read_Reg( ctx, reg, data ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }

  return COMPONENT_OK;
}


/**
 * @brief Write the data to register
 * @param handle the device handle
 * @param reg register address
 * @param data register data
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
DrvStatusTypeDef BSP_GYRO_Write_Reg( void *handle, uint8_t reg, uint8_t data )
{

  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  GYRO_Drv_t *driver = NULL;

  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }

  driver = ( GYRO_Drv_t * )ctx->pVTable;

  if ( driver->Write_Reg == NULL )
  {
    return COMPONENT_ERROR;
  }

  if ( driver->Write_Reg( ctx, reg, data ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }

  return COMPONENT_OK;
}


/**
 * @brief Get gyroscope data ready status
 * @param handle the device handle
 * @param status the data ready status
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
DrvStatusTypeDef BSP_GYRO_Get_DRDY_Status( void *handle, uint8_t *status )
{

  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  GYRO_Drv_t *driver = NULL;

  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }

  driver = ( GYRO_Drv_t * )ctx->pVTable;

  if ( driver->Get_DRDY_Status == NULL )
  {
    return COMPONENT_ERROR;
  }

  if ( driver->Get_DRDY_Status( ctx, status ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }

  return COMPONENT_OK;
}


/**
 * @brief Set the enabled/disabled status of the gyroscope sensor axes
 * @param handle the device handle
 * @param enable_xyz vector of the axes enabled/disabled status
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
DrvStatusTypeDef BSP_GYRO_Set_Axes_Status( void *handle, uint8_t *enable_xyz )
{

  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  GYRO_Drv_t *driver = NULL;
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( GYRO_Drv_t * )ctx->pVTable;
  
  if ( enable_xyz == NULL )
  {
    return COMPONENT_ERROR;
  }
  if ( driver->Set_Axes_Status == NULL )
  {
    return COMPONENT_ERROR;
  }
  if ( driver->Set_Axes_Status( ctx, enable_xyz ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }
  
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
