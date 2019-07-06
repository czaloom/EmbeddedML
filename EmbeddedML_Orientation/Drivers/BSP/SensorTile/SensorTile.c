/**
******************************************************************************
* @file    SensorTile.c
* @author  Central Labs
* @version V1.2.0
* @date    10-Nov-2016
* @brief   This file provides low level functionalities for Sensor Tile board
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
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
#include "SensorTile.h"


/** @addtogroup BSP
* @{
*/ 

/** @addtogroup SENSORTILE
* @{
*/

  /** @addtogroup SENSORTILE_LOW_LEVEL
  * @brief This file provides a set of low level firmware functions 
  * @{
  */

/** @defgroup SENSORTILE_LOW_LEVEL_Private_TypesDefinitions SENSORTILE_LOW_LEVEL Private Typedef
* @{
*/

/**
* @}
*/

/** @defgroup SENSORTILE_LOW_LEVEL__Private_Defines SENSORTILE_LOW_LEVEL Private Defines
* @{
*/

//#define SYNCHRO_WAIT(NB)       for(int i=0; i<NB; i++){__asm("dsb\n");}
//#define SYNCHRO_SPI_DELAY     (1)

/**
* @brief SensorTile BSP Driver version number V1.0.0
*/
#define __SensorTile_BSP_VERSION_MAIN   (0x01) /*!< [31:24] main version */
#define __SensorTile_BSP_VERSION_SUB1   (0x00) /*!< [23:16] sub1 version */
#define __SensorTile_BSP_VERSION_SUB2   (0x00) /*!< [15:8]  sub2 version */
#define __SensorTile_BSP_VERSION_RC     (0x00) /*!< [7:0]  release candidate */
#define __SensorTile_BSP_VERSION         ((__SensorTile_BSP_VERSION_MAIN << 24)\
|(__SensorTile_BSP_VERSION_SUB1 << 16)\
  |(__SensorTile_BSP_VERSION_SUB2 << 8 )\
    |(__SensorTile_BSP_VERSION_RC))


/**
* @}
*/

/** @defgroup SENSORTILE_LOW_LEVEL_Private_Variables SENSORTILE_LOW_LEVEL Private Variables 
* @{
*/
static uint32_t I2C_SENSORTILE_Timeout = SENSORTILE_I2C_ONBOARD_SENSORS_TIMEOUT_MAX;    /*<! Value of Timeout when I2C communication fails */
static I2C_HandleTypeDef I2C_SENSORTILE_Handle;
static SPI_HandleTypeDef SPI_Sensor_Handle;
static SPI_HandleTypeDef SPI_SD_Handle;

GPIO_TypeDef* GPIO_PORT[LEDn] = {LED1_GPIO_PORT, LEDSWD_GPIO_PORT};

const uint32_t GPIO_PIN[LEDn] = {LED1_PIN, LEDSWD_PIN};

uint32_t SpixTimeout = SENSORTILE_SD_SPI_TIMEOUT_MAX;        /*<! Value of Timeout when SPI communication fails */

/**
* @}
*/


/** @defgroup SENSORTILE_LOW_LEVEL_Private_Functions SENSORTILE_LOW_LEVEL Private Functions
* @{
*/ 
static void I2C_SENSORTILE_MspInit( void );
static void I2C_SENSORTILE_Error( uint8_t Addr );
static uint8_t I2C_SENSORTILE_ReadData( uint8_t Addr, uint8_t Reg, uint8_t* pBuffer, uint16_t Size );
static uint8_t I2C_SENSORTILE_WriteData( uint8_t Addr, uint8_t Reg, uint8_t* pBuffer, uint16_t Size );
static uint8_t I2C_SENSORTILE_Init( void );

static void               SD_IO_SPI_Init(void);/*high speed*/
static void               SD_IO_SPI_Init_LS(void); /*low speed*/

static void               SD_IO_SPI_Write(uint8_t Value);
static uint32_t           SD_IO_SPI_Read(void);
static void               SD_IO_SPI_Error (void);
static void               SD_IO_SPI_MspInit(SPI_HandleTypeDef *hspi);

void SPI_Read(SPI_HandleTypeDef* xSpiHandle, uint8_t *val);
void SPI_Read_nBytes(SPI_HandleTypeDef* xSpiHandle, uint8_t *val, uint16_t nBytesToRead );
void SPI_Write(SPI_HandleTypeDef* xSpiHandle, uint8_t val);

/* Link functions for SD Card peripheral over SPI */
void                      SD_IO_Init(void);
void                      SD_IO_Init_LS(void);/*low speed*/

HAL_StatusTypeDef         SD_IO_WriteCmd(uint8_t Cmd, uint32_t Arg, uint8_t Crc, uint8_t Response);
uint8_t                   SD_IO_WriteCmd_wResp(uint8_t Cmd, uint32_t Arg, uint8_t Crc);
HAL_StatusTypeDef         SD_IO_WaitResponse(uint8_t Response);
void                      SD_IO_WriteDummy(void);
void                      SD_IO_WriteByte(uint8_t Data);
uint8_t                   SD_IO_ReadByte(void);

/**
* @}
*/



/** @defgroup SENSORTILE_LOW_LEVEL_Exported_Functions SENSORTILE_LOW_LEVEL Exported Functions
  * @{
  */

/**
* @brief  This method returns the STM32446E EVAL BSP Driver revision
* @param  None
* @retval version: 0xXYZR (8bits for each decimal, R for RC)
*/
uint32_t BSP_GetVersion(void)
{
  return __SensorTile_BSP_VERSION;
}


/**
* @brief  Configures LEDs.
* @param  Led: LED to be configured. 
*          This parameter can be one of the following values:
*            @arg  LED1
* @retval None
*/
void BSP_LED_Init(Led_TypeDef Led)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* Enable VddIO2 for GPIOG  */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_EnableVddIO2();

  /* Enable the GPIO_LED clock */
  LEDx_GPIO_CLK_ENABLE(Led);
  
  /* Configure the GPIO_LED pin */
  GPIO_InitStruct.Pin = GPIO_PIN[Led];
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  
  HAL_GPIO_Init(GPIO_PORT[Led], &GPIO_InitStruct);
}


/**
* @brief  DeInit LEDs.
* @param  Led: LED to be configured. 
*          This parameter can be one of the following values:
*            @arg  LED1
*            @arg  LED2
*            @arg  LED3
*            @arg  LED4
* @note Led DeInit does not disable the GPIO clock nor disable the Mfx 
* @retval None
*/
void BSP_LED_DeInit(Led_TypeDef Led)
{
  
}

/**
* @brief  Turns selected LED On.
* @param  Led: LED to be set on 
*          This parameter can be one of the following values:
*            @arg  LED1
*            @arg  LED2
*            @arg  LED3
*            @arg  LED4
* @retval None
*/
void BSP_LED_On(Led_TypeDef Led)
{
  if(Led == LED1)
  {
    HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_SET);
  }
  else if (Led == LEDSWD)
  {
    HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET);
  }
}

/**
* @brief  Turns selected LED Off. 
* @param  Led: LED to be set off
*          This parameter can be one of the following values:
*            @arg  LED1
*            @arg  LED2
*            @arg  LED3
*            @arg  LED4
* @retval None
*/
void BSP_LED_Off(Led_TypeDef Led)
{
  if(Led == LED1)
  {
    HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET);
  }
  else if (Led == LEDSWD)
  {
    HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_SET);
  }
}

/**
* @brief  Toggles the selected LED.
* @param  Led: LED to be toggled
*          This parameter can be one of the following values:
*            @arg  LED1
*            @arg  LED2
*            @arg  LED3
*            @arg  LED4
* @retval None
*/
void BSP_LED_Toggle(Led_TypeDef Led)
{
  HAL_GPIO_TogglePin(GPIO_PORT[Led], GPIO_PIN[Led]);
}

/**
 * @brief  Configures sensor SPI interface.
 * @param  None
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
DrvStatusTypeDef Sensor_IO_I2C_Init( void )
{
  if ( I2C_SENSORTILE_Init() )
  {
    return COMPONENT_ERROR;
  }
  else
  {
    return COMPONENT_OK;
  }
}


/**
 * @brief  Configures sensor SPI interface.
 * @param  None
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
DrvStatusTypeDef Sensor_IO_SPI_Init( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  if(HAL_SPI_GetState( &SPI_Sensor_Handle) == HAL_SPI_STATE_RESET )
  {
    SENSORTILE_SENSORS_SPI_CLK_ENABLE();
    SENSORTILE_SENSORS_SPI_GPIO_CLK_ENABLE();
    
    GPIO_InitStruct.Pin = SENSORTILE_SENSORS_SPI_MOSI_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(SENSORTILE_SENSORS_SPI_Port, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = SENSORTILE_SENSORS_SPI_SCK_Pin;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(SENSORTILE_SENSORS_SPI_Port, &GPIO_InitStruct);
    
    SPI_Sensor_Handle.Instance = SENSORTILE_SENSORS_SPI;
    SPI_Sensor_Handle.Init.Mode = SPI_MODE_MASTER;
    SPI_Sensor_Handle.Init.Direction = SPI_DIRECTION_1LINE;
    SPI_Sensor_Handle.Init.DataSize = SPI_DATASIZE_8BIT;
    SPI_Sensor_Handle.Init.CLKPolarity = SPI_POLARITY_HIGH;
    SPI_Sensor_Handle.Init.CLKPhase = SPI_PHASE_2EDGE;
    SPI_Sensor_Handle.Init.NSS = SPI_NSS_SOFT;
    SPI_Sensor_Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16; // 5 MHz
//    SPI_Sensor_Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32; // 2.5MHz
    SPI_Sensor_Handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
    SPI_Sensor_Handle.Init.TIMode = SPI_TIMODE_DISABLED;
    SPI_Sensor_Handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
    SPI_Sensor_Handle.Init.CRCPolynomial = 7;
    SPI_Sensor_Handle.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
    SPI_Sensor_Handle.Init.NSSPMode = SPI_NSS_PULSE_DISABLED;
    HAL_SPI_Init(&SPI_Sensor_Handle);
    
    SPI_1LINE_TX(&SPI_Sensor_Handle);
    __HAL_SPI_ENABLE(&SPI_Sensor_Handle);
  }  
  return COMPONENT_OK;
}

uint8_t Sensor_IO_SPI_CS_Init_All(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* Set all the pins before init to avoid glitch */
  HAL_GPIO_WritePin(SENSORTILE_LSM6DSM_SPI_CS_Port, SENSORTILE_LSM6DSM_SPI_CS_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(SENSORTILE_LSM303AGR_X_SPI_CS_Port, SENSORTILE_LSM303AGR_X_SPI_CS_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(SENSORTILE_LSM303AGR_M_SPI_CS_Port, SENSORTILE_LSM303AGR_M_SPI_CS_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(SENSORTILE_LPS22HB_SPI_CS_Port, SENSORTILE_LPS22HB_SPI_CS_Pin, GPIO_PIN_SET);
  
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  
  SENSORTILE_LSM6DSM_SPI_CS_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Pin = SENSORTILE_LSM6DSM_SPI_CS_Pin;
  HAL_GPIO_Init(SENSORTILE_LSM6DSM_SPI_CS_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(SENSORTILE_LSM6DSM_SPI_CS_Port, SENSORTILE_LSM6DSM_SPI_CS_Pin, GPIO_PIN_SET);
  
  SENSORTILE_LSM303AGR_X_SPI_CS_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Pin = SENSORTILE_LSM303AGR_X_SPI_CS_Pin;
  HAL_GPIO_Init(SENSORTILE_LSM303AGR_X_SPI_CS_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(SENSORTILE_LSM303AGR_X_SPI_CS_Port, SENSORTILE_LSM303AGR_X_SPI_CS_Pin, GPIO_PIN_SET);
  
  SENSORTILE_LSM303AGR_M_SPI_CS_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Pin = SENSORTILE_LSM303AGR_M_SPI_CS_Pin;
  HAL_GPIO_Init(SENSORTILE_LSM303AGR_M_SPI_CS_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(SENSORTILE_LSM303AGR_M_SPI_CS_Port, SENSORTILE_LSM303AGR_M_SPI_CS_Pin, GPIO_PIN_SET);
  
  SENSORTILE_LPS22HB_SPI_CS_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Pin = SENSORTILE_LPS22HB_SPI_CS_Pin;
  HAL_GPIO_Init(SENSORTILE_LPS22HB_SPI_CS_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(SENSORTILE_LPS22HB_SPI_CS_Port, SENSORTILE_LPS22HB_SPI_CS_Pin, GPIO_PIN_SET);

  return COMPONENT_OK;
}

uint8_t Sensor_IO_SPI_CS_Init(void *handle)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  
  switch(ctx->spiDevice)
  {
  case LSM6DSM:
    SENSORTILE_LSM6DSM_SPI_CS_GPIO_CLK_ENABLE();
    GPIO_InitStruct.Pin = SENSORTILE_LSM6DSM_SPI_CS_Pin;
    /* Set the pin before init to avoid glitch */
    HAL_GPIO_WritePin(SENSORTILE_LSM6DSM_SPI_CS_Port, SENSORTILE_LSM6DSM_SPI_CS_Pin, GPIO_PIN_SET);
    HAL_GPIO_Init(SENSORTILE_LSM6DSM_SPI_CS_Port, &GPIO_InitStruct);
    break;
  case LSM303AGR_X:
    SENSORTILE_LSM303AGR_X_SPI_CS_GPIO_CLK_ENABLE();
    GPIO_InitStruct.Pin = SENSORTILE_LSM303AGR_X_SPI_CS_Pin;
    /* Set the pin before init to avoid glitch */
    HAL_GPIO_WritePin(SENSORTILE_LSM303AGR_X_SPI_CS_Port, SENSORTILE_LSM303AGR_X_SPI_CS_Pin, GPIO_PIN_SET);
    HAL_GPIO_Init(SENSORTILE_LSM303AGR_X_SPI_CS_Port, &GPIO_InitStruct);
    break;
  case LSM303AGR_M:
    SENSORTILE_LSM303AGR_M_SPI_CS_GPIO_CLK_ENABLE();
    GPIO_InitStruct.Pin = SENSORTILE_LSM303AGR_M_SPI_CS_Pin;
    /* Set the pin before init to avoid glitch */
    HAL_GPIO_WritePin(SENSORTILE_LSM303AGR_M_SPI_CS_Port, SENSORTILE_LSM303AGR_M_SPI_CS_Pin, GPIO_PIN_SET);
    HAL_GPIO_Init(SENSORTILE_LSM303AGR_M_SPI_CS_Port, &GPIO_InitStruct);
    break;
  case LPS22HB:
    SENSORTILE_LPS22HB_SPI_CS_GPIO_CLK_ENABLE();
    GPIO_InitStruct.Pin = SENSORTILE_LPS22HB_SPI_CS_Pin;
    /* Set the pin before init to avoid glitch */
    HAL_GPIO_WritePin(SENSORTILE_LPS22HB_SPI_CS_Port, SENSORTILE_LPS22HB_SPI_CS_Pin, GPIO_PIN_SET);
    HAL_GPIO_Init(SENSORTILE_LPS22HB_SPI_CS_Port, &GPIO_InitStruct);
    break;
  default:
    return COMPONENT_NOT_IMPLEMENTED;
  }
  return COMPONENT_OK;
}

/**
 * @brief  Writes a buffer to the sensor
 * @param  handle instance handle
 * @param  WriteAddr specifies the internal sensor address register to be written to
 * @param  pBuffer pointer to data buffer
 * @param  nBytesToWrite number of bytes to be written
 * @retval 0 in case of success
 * @retval 1 in case of failure
 */
uint8_t Sensor_IO_Write( void *handle, uint8_t WriteAddr, uint8_t *pBuffer, uint16_t nBytesToWrite )
{
  
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  
  if(ctx->ifType == 0)
  {
    
    if ( nBytesToWrite > 1 ) 
    if (ctx->who_am_i == HTS221_WHO_AM_I_VAL)
        WriteAddr |= 0x80;  /* Enable I2C multi-bytes Write */
    
    return Sensor_IO_I2C_Write( handle, WriteAddr, pBuffer, nBytesToWrite );
  }
  
  if(ctx->ifType == 1)
  {
    if ( nBytesToWrite > 1 ) 
    {
      switch(ctx->who_am_i)
        {
          case LSM303AGR_ACC_WHO_AM_I: WriteAddr |= 0x40; break; /* Enable I2C multi-bytes Write */
          case LSM303AGR_MAG_WHO_AM_I: break;
          default:;
        }
    }
   return Sensor_IO_SPI_Write( handle, WriteAddr, pBuffer, nBytesToWrite );
  }
  
  return COMPONENT_ERROR;
}


/**
 * @brief  Writes a buffer to the sensor by I2C
 * @param  handle instance handle
 * @param  WriteAddr specifies the internal sensor address register to be written to
 * @param  pBuffer pointer to data buffer
 * @param  nBytesToWrite number of bytes to be written
 * @retval 0 in case of success
 * @retval 1 in case of failure
 */
uint8_t Sensor_IO_I2C_Write( void *handle, uint8_t WriteAddr, uint8_t *pBuffer, uint16_t nBytesToWrite )
{
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;

  
  return I2C_SENSORTILE_WriteData( ctx->address, WriteAddr, pBuffer, nBytesToWrite );
}


/**
 * @brief  Reads from the sensor to a buffer
 * @param  handle instance handle
 * @param  ReadAddr specifies the internal sensor address register to be read from
 * @param  pBuffer pointer to data buffer
 * @param  nBytesToRead number of bytes to be read
 * @retval 0 in case of success
 * @retval 1 in case of failure
 */
uint8_t Sensor_IO_Read( void *handle, uint8_t ReadAddr, uint8_t *pBuffer, uint16_t nBytesToRead )
{

  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  
  if(ctx->ifType == 0)
  {
     
  if ( nBytesToRead > 1 ) 
    if (ctx->who_am_i == HTS221_WHO_AM_I_VAL)
        ReadAddr |= 0x80;  /* Enable I2C multi-bytes Write */
  
    return Sensor_IO_I2C_Read( handle, ReadAddr, pBuffer, nBytesToRead );
  }
  
  if(ctx->ifType == 1 )
  {
    if ( nBytesToRead > 1 ) {
      switch(ctx->who_am_i)
        {
          case LSM303AGR_ACC_WHO_AM_I: ReadAddr |= 0x40; break; /* Enable I2C multi-bytes Write */
          case LSM303AGR_MAG_WHO_AM_I: break;
          default:;
        }
    }
   return Sensor_IO_SPI_Read( handle, ReadAddr, pBuffer, nBytesToRead );
  }
  
  return COMPONENT_ERROR;
}


/**
 * @brief  Reads from the sensor to a buffer by I2C
 * @param  handle instance handle
 * @param  ReadAddr specifies the internal sensor address register to be read from
 * @param  pBuffer pointer to data buffer
 * @param  nBytesToRead number of bytes to be read
 * @retval 0 in case of success
 * @retval 1 in case of failure
 */
uint8_t Sensor_IO_I2C_Read( void *handle, uint8_t ReadAddr, uint8_t *pBuffer, uint16_t nBytesToRead )
{
  
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  

  return I2C_SENSORTILE_ReadData( ctx->address, ReadAddr, pBuffer, nBytesToRead );
}


/**
 * @brief  Writes a buffer to the sensor
 * @param  handle instance handle
 * @param  WriteAddr specifies the internal sensor address register to be written to
 * @param  pBuffer pointer to data buffer
 * @param  nBytesToWrite number of bytes to be written
 * @retval 0 in case of success
 * @retval 1 in case of failure
 */
uint8_t Sensor_IO_SPI_Write( void *handle, uint8_t WriteAddr, uint8_t *pBuffer, uint16_t nBytesToWrite )
{
  uint8_t i;
  
// Select the correct device
  Sensor_IO_SPI_CS_Enable(handle);
  
  SPI_Write(&SPI_Sensor_Handle, WriteAddr);

  for(i=0;i<nBytesToWrite;i++)
  {
    SPI_Write(&SPI_Sensor_Handle, pBuffer[i]);
  }
// Deselect the device
  Sensor_IO_SPI_CS_Disable(handle);
  
  return COMPONENT_OK;
}

/**
 * @brief  Reads a from the sensor to buffer
 * @param  handle instance handle
 * @param  ReadAddr specifies the internal sensor address register to be read from
 * @param  pBuffer pointer to data buffer
 * @param  nBytesToRead number of bytes to be read
 * @retval 0 in case of success
 * @retval 1 in case of failure
 */
uint8_t Sensor_IO_SPI_Read( void *handle, uint8_t ReadAddr, uint8_t *pBuffer, uint16_t nBytesToRead )
{
  /* Select the correct device */
  Sensor_IO_SPI_CS_Enable(handle);
  
  /* Write Reg Address */
  SPI_Write(&SPI_Sensor_Handle, ReadAddr | 0x80);

  /* Disable the SPI and change the data line to input */
  __HAL_SPI_DISABLE(&SPI_Sensor_Handle);
  SPI_1LINE_RX(&SPI_Sensor_Handle);

  /* Check if we need to read one byte or more */
  if(nBytesToRead > 1U)
  {
    SPI_Read_nBytes(&SPI_Sensor_Handle, pBuffer, nBytesToRead);
  }
  else
  {
    SPI_Read(&SPI_Sensor_Handle, pBuffer);
  }
  
  /* Deselect the device */
  Sensor_IO_SPI_CS_Disable(handle);  
  
  /* Change the data line to output and enable the SPI */
  SPI_1LINE_TX(&SPI_Sensor_Handle);
  __HAL_SPI_ENABLE(&SPI_Sensor_Handle);
  
  return COMPONENT_OK;
}


uint8_t Sensor_IO_SPI_CS_Enable(void *handle)
{
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  
  switch(ctx->spiDevice)
  {
  case LSM6DSM:
    HAL_GPIO_WritePin(SENSORTILE_LSM6DSM_SPI_CS_Port, SENSORTILE_LSM6DSM_SPI_CS_Pin, GPIO_PIN_RESET);
    break;
  case LSM303AGR_X:
    HAL_GPIO_WritePin(SENSORTILE_LSM303AGR_X_SPI_CS_Port, SENSORTILE_LSM303AGR_X_SPI_CS_Pin, GPIO_PIN_RESET);
    break;
  case LSM303AGR_M:
    HAL_GPIO_WritePin(SENSORTILE_LSM303AGR_M_SPI_CS_Port, SENSORTILE_LSM303AGR_M_SPI_CS_Pin, GPIO_PIN_RESET);
    break;
  case LPS22HB:
    HAL_GPIO_WritePin(SENSORTILE_LPS22HB_SPI_CS_Port, SENSORTILE_LPS22HB_SPI_CS_Pin, GPIO_PIN_RESET);
    break;
  }
  return COMPONENT_OK;
}

uint8_t Sensor_IO_SPI_CS_Disable(void *handle)
{
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  
  switch(ctx->spiDevice)
  {
  case LSM6DSM:
    HAL_GPIO_WritePin(SENSORTILE_LSM6DSM_SPI_CS_Port, SENSORTILE_LSM6DSM_SPI_CS_Pin, GPIO_PIN_SET);
    break;
  case LSM303AGR_X:
    HAL_GPIO_WritePin(SENSORTILE_LSM303AGR_X_SPI_CS_Port, SENSORTILE_LSM303AGR_X_SPI_CS_Pin, GPIO_PIN_SET);
    break;
  case LSM303AGR_M:
    HAL_GPIO_WritePin(SENSORTILE_LSM303AGR_M_SPI_CS_Port, SENSORTILE_LSM303AGR_M_SPI_CS_Pin, GPIO_PIN_SET);
    break;
  case LPS22HB:
    HAL_GPIO_WritePin(SENSORTILE_LPS22HB_SPI_CS_Port, SENSORTILE_LPS22HB_SPI_CS_Pin, GPIO_PIN_SET);
    break;
  }
  return COMPONENT_OK;
}

/**
 * @brief  Configures sensor interrupts interface for LSM6DSM sensor.
 * @param  None
 * @retval COMPONENT_OK in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
DrvStatusTypeDef LSM6DSM_Sensor_IO_ITConfig( void )
{

  /* At the moment this feature is only implemented for LSM6DSM */
  GPIO_InitTypeDef GPIO_InitStructureInt2;
  
  /* Enable INT2 GPIO clock */
  LSM6DSM_INT2_GPIO_CLK_ENABLE();
  
  /* Configure GPIO PINs to detect Interrupts */
  GPIO_InitStructureInt2.Pin = LSM6DSM_INT2_PIN;
  GPIO_InitStructureInt2.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStructureInt2.Speed = GPIO_SPEED_FAST;
  GPIO_InitStructureInt2.Pull  = GPIO_NOPULL;
  HAL_GPIO_Init(LSM6DSM_INT2_GPIO_PORT, &GPIO_InitStructureInt2);
  
  /* Enable and set EXTI Interrupt priority */
  HAL_NVIC_SetPriority(LSM6DSM_INT2_EXTI_IRQn, 0x00, 0x00);
  HAL_NVIC_EnableIRQ(LSM6DSM_INT2_EXTI_IRQn);
  
  return COMPONENT_OK;
}

//#if defined(__ICCARM__)
//#pragma optimize=none
//#endif

/**
 * @brief  This function reads a single byte on SPI 3-wire.
 * @param  xSpiHandle : SPI Handler.
 * @param  val : value.
 * @retval None
 */
void SPI_Read(SPI_HandleTypeDef* xSpiHandle, uint8_t *val)
{
  /* In master RX mode the clock is automaticaly generated on the SPI enable.
  So to guarantee the clock generation for only one data, the clock must be
  disabled after the first bit and before the latest bit */
  /* Interrupts should be disabled during this operation */
  
  __disable_irq();
  
  __HAL_SPI_ENABLE(xSpiHandle);
  __asm("dsb\n");
  __asm("dsb\n");
  __HAL_SPI_DISABLE(xSpiHandle);
  
  __enable_irq();
  
  while ((xSpiHandle->Instance->SR & SPI_FLAG_RXNE) != SPI_FLAG_RXNE);
  /* read the received data */
  *val = *(__IO uint8_t *) &xSpiHandle->Instance->DR;
  while ((xSpiHandle->Instance->SR & SPI_FLAG_BSY) == SPI_FLAG_BSY);
}

/**
 * @brief  This function reads multiple bytes on SPI 3-wire.
 * @param  xSpiHandle: SPI Handler.
 * @param  val: value.
 * @param  nBytesToRead: number of bytes to read.
 * @retval None
 */
void SPI_Read_nBytes(SPI_HandleTypeDef* xSpiHandle, uint8_t *val, uint16_t nBytesToRead)
{
  /* Interrupts should be disabled during this operation */
  __disable_irq();
  __HAL_SPI_ENABLE(xSpiHandle);
  
  /* Transfer loop */
  while (nBytesToRead > 1U)
  {
    /* Check the RXNE flag */
    if (xSpiHandle->Instance->SR & SPI_FLAG_RXNE)
    {
      /* read the received data */
      *val = *(__IO uint8_t *) &xSpiHandle->Instance->DR;
      val += sizeof(uint8_t);
      nBytesToRead--;
    }
  }
  /* In master RX mode the clock is automaticaly generated on the SPI enable.
  So to guarantee the clock generation for only one data, the clock must be
  disabled after the first bit and before the latest bit of the last Byte received */
  /* __DSB instruction are inserted to garantee that clock is Disabled in the right timeframe */
  
  __DSB();
  __DSB();
  __HAL_SPI_DISABLE(xSpiHandle);
  
  __enable_irq();
  
  while ((xSpiHandle->Instance->SR & SPI_FLAG_RXNE) != SPI_FLAG_RXNE);
  /* read the received data */
  *val = *(__IO uint8_t *) &xSpiHandle->Instance->DR;
  while ((xSpiHandle->Instance->SR & SPI_FLAG_BSY) == SPI_FLAG_BSY);
}

/**
 * @brief  This function writes a single byte on SPI 3-wire.
 * @param  xSpiHandle: SPI Handler.
 * @param  val: value.
 * @retval None
 */
void SPI_Write(SPI_HandleTypeDef* xSpiHandle, uint8_t val)
{
  /* check TXE flag */
  while ((xSpiHandle->Instance->SR & SPI_FLAG_TXE) != SPI_FLAG_TXE);
  
  /* Write the data */
  *((__IO uint8_t*) &xSpiHandle->Instance->DR) = val;
  
  /* Wait BSY flag */
  while ((xSpiHandle->Instance->SR & SPI_FLAG_FTLVL) != SPI_FTLVL_EMPTY);
  while ((xSpiHandle->Instance->SR & SPI_FLAG_BSY) == SPI_FLAG_BSY);
}


/******************************* I2C Routines *********************************/

/**
 * @brief  Configures I2C interface.
 * @param  None
 * @retval 0 in case of success
 * @retval 1 in case of failure
 */
static uint8_t I2C_SENSORTILE_Init( void )
{
  if(HAL_I2C_GetState( &I2C_SENSORTILE_Handle) == HAL_I2C_STATE_RESET )
  {  
    /* I2C_SENSORTILE peripheral configuration */
    I2C_SENSORTILE_Handle.Init.Timing = I2C_SENSORTILE_TIMING_100KHZ;   /* 100KHz */
    I2C_SENSORTILE_Handle.Init.OwnAddress1    = 0x33;
    I2C_SENSORTILE_Handle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    I2C_SENSORTILE_Handle.Instance            = SENSORTILE_I2C_ONBOARD_SENSORS;
    
    /* Init the I2C */
    I2C_SENSORTILE_MspInit();
    HAL_I2C_Init( &I2C_SENSORTILE_Handle );
  }
  
  if( HAL_I2C_GetState( &I2C_SENSORTILE_Handle) == HAL_I2C_STATE_READY )
  {
    return 0;
  }
  else
  {
    return 1;
  }
}



/**
 * @brief  Write data to the register of the device through BUS
 * @param  Addr Device address on BUS
 * @param  Reg The target register address to be written
 * @param  pBuffer The data to be written
 * @param  Size Number of bytes to be written
 * @retval 0 in case of success
 * @retval 1 in case of failure
 */
static uint8_t I2C_SENSORTILE_WriteData( uint8_t Addr, uint8_t Reg, uint8_t* pBuffer, uint16_t Size )
{

  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_I2C_Mem_Write( &I2C_SENSORTILE_Handle, Addr, ( uint16_t )Reg, I2C_MEMADD_SIZE_8BIT, pBuffer, Size, I2C_SENSORTILE_Timeout );
                              
  /* Check the communication status */
  if( status != HAL_OK )
  {
  
    /* Execute user timeout callback */
    I2C_SENSORTILE_Error( Addr );
    return 1;
  }
  else
  {
    return 0;
  }
}


/**
 * @brief  Read a register of the device through BUS
 * @param  Addr Device address on BUS
 * @param  Reg The target register address to read
 * @param  pBuffer The data to be read
 * @param  Size Number of bytes to be read
 * @retval 0 in case of success
 * @retval 1 in case of failure
 */
static uint8_t I2C_SENSORTILE_ReadData( uint8_t Addr, uint8_t Reg, uint8_t* pBuffer, uint16_t Size )
{
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_I2C_Mem_Read( &I2C_SENSORTILE_Handle, Addr, ( uint16_t )Reg, I2C_MEMADD_SIZE_8BIT, pBuffer, Size, I2C_SENSORTILE_Timeout );
                             
  /* Check the communication status */
  if( status != HAL_OK )
  {
    /* Execute user timeout callback */
    I2C_SENSORTILE_Error( Addr );
    return 1;
  }
  else
  {
    return 0;
  }
}


/**
 * @brief  Manages error callback by re-initializing I2C
 * @param  Addr I2C Address
 * @retval None
 */
static void I2C_SENSORTILE_Error( uint8_t Addr )
{
  /* De-initialize the I2C comunication bus */
  HAL_I2C_DeInit( &I2C_SENSORTILE_Handle );
  
  /* Re-Initiaize the I2C comunication bus */
  I2C_SENSORTILE_Init();
}


/**
 * @brief I2C MSP Initialization
 * @param None
 * @retval None
 */
static void I2C_SENSORTILE_MspInit( void )
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  RCC_PeriphCLKInitTypeDef  RCC_PeriphCLKInitStruct;
  
  /*##-1- Configure the I2C clock source. The clock is derived from the SYSCLK #*/
  RCC_PeriphCLKInitStruct.PeriphClockSelection = SENSORTILE_I2C_ONBOARD_SENSORS_RCC_PERIPHCLK;
  RCC_PeriphCLKInitStruct.I2c3ClockSelection = SENSORTILE_I2C_ONBOARD_SENSORS_I2CCLKSOURCE;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);
  
  /* Enable I2C GPIO clocks */
  SENSORTILE_I2C_ONBOARD_SENSORS_SCL_SDA_GPIO_CLK_ENABLE();
  
  /* I2C_SENSORTILE SCL and SDA pins configuration -------------------------------------*/
  GPIO_InitStruct.Pin        = SENSORTILE_I2C_ONBOARD_SENSORS_SCL_PIN | SENSORTILE_I2C_ONBOARD_SENSORS_SDA_PIN;
  GPIO_InitStruct.Mode       = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Speed      = GPIO_SPEED_FAST;
  GPIO_InitStruct.Pull       = GPIO_PULLUP;
  GPIO_InitStruct.Alternate  = SENSORTILE_I2C_ONBOARD_SENSORS_SCL_SDA_AF;
  
  HAL_GPIO_Init( SENSORTILE_I2C_ONBOARD_SENSORS_SCL_SDA_GPIO_PORT, &GPIO_InitStruct );
  
  /* Enable the I2C_SENSORTILE peripheral clock */
  SENSORTILE_I2C_ONBOARD_SENSORS_CLK_ENABLE();
  
  /* Force the I2C peripheral clock reset */
  SENSORTILE_I2C_ONBOARD_SENSORS_FORCE_RESET();
  
  /* Release the I2C peripheral clock reset */
  SENSORTILE_I2C_ONBOARD_SENSORS_RELEASE_RESET();
  
  /* Enable and set I2C_SENSORTILE Interrupt to the highest priority */
  HAL_NVIC_SetPriority(SENSORTILE_I2C_ONBOARD_SENSORS_EV_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(SENSORTILE_I2C_ONBOARD_SENSORS_EV_IRQn);
  
  /* Enable and set I2C_SENSORTILE Interrupt to the highest priority */
  HAL_NVIC_SetPriority(SENSORTILE_I2C_ONBOARD_SENSORS_ER_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(SENSORTILE_I2C_ONBOARD_SENSORS_ER_IRQn);
}


/******************************* SPI Routines**********************************/
/**
  * @brief  Initializes SPI MSP.
  * @param  None
  * @retval None
  */
static void SD_IO_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef  GPIO_InitStructure;  
  
  /* Enable VddIO2 for GPIOG  */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_EnableVddIO2();
  
  /*** Configure the GPIOs ***/  
  /* Enable GPIO clock */
  SENSORTILE_SD_SPI_SCK_GPIO_CLK_ENABLE();
  SENSORTILE_SD_SPI_MISO_MOSI_GPIO_CLK_ENABLE();
  
  /* configure SPI SCK */
  GPIO_InitStructure.Pin = SENSORTILE_SD_SPI_SCK_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull  = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStructure.Alternate = SENSORTILE_SD_SPI_SCK_AF;
  HAL_GPIO_Init(SENSORTILE_SD_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* configure SPI MISO and MOSI */
  GPIO_InitStructure.Pin = (SENSORTILE_SD_SPI_MISO_PIN | SENSORTILE_SD_SPI_MOSI_PIN);
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull  = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStructure.Alternate = SENSORTILE_SD_SPI_MISO_MOSI_AF;
  HAL_GPIO_Init(SENSORTILE_SD_SPI_MISO_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /*** Configure the SPI peripheral ***/ 
  /* Enable SPI clock */
  SENSORTILE_SD_SPI_CLK_ENABLE();
}

/**
* @brief  Initializes SPI HAL. Low baundrate for initializazion phase.
* @param  None
* @retval None
*/
static void SD_IO_SPI_Init_LS(void)
{
  if(HAL_SPI_GetState(&SPI_SD_Handle) == HAL_SPI_STATE_RESET)
  {
    SPI_SD_Handle.Instance = SENSORTILE_SD_SPI;
    SPI_SD_Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;    /* SPI baudrate is PCLK2/SPI_BaudRatePrescaler */
    SPI_SD_Handle.Init.Direction = SPI_DIRECTION_2LINES;
    SPI_SD_Handle.Init.CLKPhase = SPI_PHASE_2EDGE;
    SPI_SD_Handle.Init.CLKPolarity = SPI_POLARITY_HIGH;
    SPI_SD_Handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
    SPI_SD_Handle.Init.CRCPolynomial = 7;
    SPI_SD_Handle.Init.DataSize = SPI_DATASIZE_8BIT;
    SPI_SD_Handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
    SPI_SD_Handle.Init.NSS = SPI_NSS_SOFT;
    SPI_SD_Handle.Init.TIMode = SPI_TIMODE_DISABLED;
    SPI_SD_Handle.Init.Mode = SPI_MODE_MASTER;
    
    SD_IO_SPI_MspInit(&SPI_SD_Handle);
    HAL_SPI_Init(&SPI_SD_Handle);
  }
}
/**
* @brief  Initializes SPI HAL. High baundrate
* @param  None
* @retval None
*/
static void SD_IO_SPI_Init(void)
{
  if(HAL_SPI_GetState(&SPI_SD_Handle) == HAL_SPI_STATE_RESET)
  {
    SPI_SD_Handle.Instance = SENSORTILE_SD_SPI;
    SPI_SD_Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;    /* SPI baudrate is PCLK2/SPI_BaudRatePrescaler */
    SPI_SD_Handle.Init.Direction = SPI_DIRECTION_2LINES;
    SPI_SD_Handle.Init.CLKPhase = SPI_PHASE_2EDGE;
    SPI_SD_Handle.Init.CLKPolarity = SPI_POLARITY_HIGH;
    SPI_SD_Handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
    SPI_SD_Handle.Init.CRCPolynomial = 7;
    SPI_SD_Handle.Init.DataSize = SPI_DATASIZE_8BIT;
    SPI_SD_Handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
    SPI_SD_Handle.Init.NSS = SPI_NSS_SOFT;
    SPI_SD_Handle.Init.TIMode = SPI_TIMODE_DISABLED;
    SPI_SD_Handle.Init.Mode = SPI_MODE_MASTER;
    
    SD_IO_SPI_MspInit(&SPI_SD_Handle);
    HAL_SPI_Init(&SPI_SD_Handle);
  }
}

/**
  * @brief SPI Read 4 bytes from device
  * @param None
  * @retval Read data
*/
static uint32_t SD_IO_SPI_Read(void)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t readvalue = 0;
  uint32_t writevalue = 0xFFFFFFFF;
  
  status = HAL_SPI_TransmitReceive(&SPI_SD_Handle, (uint8_t*) &writevalue, (uint8_t*) &readvalue, 1, SpixTimeout);
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SD_IO_SPI_Error();
  }

  return readvalue;
}

/**
  * @brief SPI Write a byte to device
  * @param Value: value to be written
  * @retval None
  */
static void SD_IO_SPI_Write(uint8_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_SPI_Transmit(&SPI_SD_Handle, (uint8_t*) &Value, 1, SpixTimeout);

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SD_IO_SPI_Error();
  }
}

/**
  * @brief SPI error treatment function
  * @param None
  * @retval None
  */
static void SD_IO_SPI_Error (void)
{
  /* De-initialize the SPI communication BUS */
  HAL_SPI_DeInit(&SPI_SD_Handle);
  
  /* Re- Initiaize the SPI communication BUS */
  SD_IO_SPI_Init();
}

/******************************** LINK SD Card ********************************/

/**
  * @brief  Initializes the SD Card and put it into StandBy State (Ready for 
  *         data transfer).
  * @param  None
  * @retval None
  */
void SD_IO_Init(void)
{
  uint8_t counter;

  /* SD SPI Config */
  SD_IO_CS_Init();
  
  /* SD SPI Config */
  SD_IO_SPI_Init();
  
  SENSORTILE_SD_CS_HIGH();
  
  /* Send dummy byte 0xFF, 10 times with CS high */
  /* Rise CS and MOSI for 80 clocks cycles */
  for (counter = 0; counter <= 9; counter++)
  {
    /* Send dummy byte 0xFF */
    SD_IO_WriteByte(SENSORTILE_SD_DUMMY_BYTE);
  }
}

/**
  * @brief  Initializes the SD Card and put it into StandBy State (Ready for 
  *         data transfer). Low baundrate
  * @param  None
  * @retval None
  */
void SD_IO_Init_LS()
{ 
  uint8_t counter;

  /* SD SPI Config */
  SD_IO_CS_Init();
  
  /* SD SPI Config */
  SD_IO_SPI_Init_LS();
  
  SENSORTILE_SD_CS_HIGH();
  
  /* Send dummy byte 0xFF, 10 times with CS high */
  /* Rise CS and MOSI for 80 clocks cycles */
  for (counter = 0; counter <= 9; counter++)
  {
    /* Send dummy byte 0xFF */
    SD_IO_WriteByte(SENSORTILE_SD_DUMMY_BYTE);
  }
}

void SD_IO_CS_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* SD_CS_GPIO and SD_DETECT_GPIO Periph clock enable */
  SENSORTILE_SD_CS_GPIO_CLK_ENABLE();

  /* Configure SD_CS_PIN pin: SD Card CS pin */
  GPIO_InitStructure.Pin = SENSORTILE_SD_CS_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(SENSORTILE_SD_CS_GPIO_PORT, &GPIO_InitStructure);
  
  /* SD chip select high */
//  SENSORTILE_SD_CS_HIGH();
}

void SD_IO_CS_DeInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Configure SD_CS_PIN pin: SD Card CS pin */
  GPIO_InitStructure.Pin = SENSORTILE_SD_CS_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(SENSORTILE_SD_CS_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  Writes a byte on the SD.
  * @param  Data: byte to send.
  * @retval None
  */
void SD_IO_WriteByte(uint8_t Data)
{
  /* Send the byte */
  SD_IO_SPI_Write(Data);
}

/**
  * @brief  Reads a byte from the SD.
  * @param  None
  * @retval The received byte.
  */
uint8_t SD_IO_ReadByte(void)
{
  uint8_t data = 0;
  
  /* Get the received data */
  data = SD_IO_SPI_Read();

  /* Return the shifted data */
  return data;
}


/**
  * @brief  Sends 5 bytes command to the SD card and get response
  * @param  Cmd: The user expected command to send to SD card.
  * @param  Arg: The command argument.
  * @param  Crc: The CRC.
  * @param  Response: Expected response from the SD card
  * @retval  HAL_StatusTypeDef HAL Status
  */
uint8_t SD_IO_WriteCmd_wResp(uint8_t Cmd, uint32_t Arg, uint8_t Crc)
{
  uint32_t n = 0x00, resp;
  uint8_t frame[6];

  /* Prepare Frame to send */
  frame[0] = (Cmd | 0x40); /* Construct byte 1 */
  frame[1] = (uint8_t)(Arg >> 24); /* Construct byte 2 */
  frame[2] = (uint8_t)(Arg >> 16); /* Construct byte 3 */
  frame[3] = (uint8_t)(Arg >> 8); /* Construct byte 4 */
  frame[4] = (uint8_t)(Arg); /* Construct byte 5 */
  frame[5] = (Crc); /* Construct CRC: byte 6 */
  
  /* SD chip select low */
  SENSORTILE_SD_CS_LOW();
    
  /* Send Frame */
  for (n = 0; n < 6; n++)
  {
    SD_IO_WriteByte(frame[n]); /* Send the Cmd bytes */
  }

  n = 10; /* Wait for response (10 bytes max) */
  do {
    resp = SD_IO_ReadByte();
  } while ((resp & 0x80) && --n);
  
  return resp;	/* Return received response */
}


/**
  * @brief  Sends 5 bytes command to the SD card and get response
  * @param  Cmd: The user expected command to send to SD card.
  * @param  Arg: The command argument.
  * @param  Crc: The CRC.
  * @param  Response: Expected response from the SD card
  * @retval  HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef SD_IO_WriteCmd(uint8_t Cmd, uint32_t Arg, uint8_t Crc, uint8_t Response)
{
  uint32_t counter = 0x00;
  uint8_t frame[6];

  /* Prepare Frame to send */
  frame[0] = (Cmd | 0x40); /* Construct byte 1 */
  frame[1] = (uint8_t)(Arg >> 24); /* Construct byte 2 */
  frame[2] = (uint8_t)(Arg >> 16); /* Construct byte 3 */
  frame[3] = (uint8_t)(Arg >> 8); /* Construct byte 4 */
  frame[4] = (uint8_t)(Arg); /* Construct byte 5 */
  frame[5] = (Crc); /* Construct CRC: byte 6 */
  
  /* SD chip select low */
  SENSORTILE_SD_CS_LOW();
    
  /* Send Frame */
  for (counter = 0; counter < 6; counter++)
  {
    SD_IO_WriteByte(frame[counter]); /* Send the Cmd bytes */
  }

  if(Response != SENSORTILE_SD_NO_RESPONSE_EXPECTED)
  {
    return SD_IO_WaitResponse(Response);
  }
  
  return HAL_OK;
}

/**
  * @brief  Waits response from the SD card
  * @param  Response: Expected response from the SD card
  * @retval  HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef SD_IO_WaitResponse(uint8_t Response)
{
  uint32_t timeout = 0xFF;

  /* Check if response is got or a timeout is happen */
  while ((SD_IO_ReadByte() != Response) && timeout)
  {
    timeout--;
  }

  if (timeout == 0)
  {
    /* After time out */
    return HAL_TIMEOUT;
  }
  else
  {
    /* Right response got */
    return HAL_OK;
  }
}

/**
  * @brief  Sends dummy byte with CS High
  * @param  None
  * @retval None
  */
void SD_IO_WriteDummy(void)
{
    /* SD chip select high */
    SENSORTILE_SD_CS_HIGH();
    
    /* Send Dummy byte 0xFF */
    SD_IO_WriteByte(SENSORTILE_SD_DUMMY_BYTE);
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
