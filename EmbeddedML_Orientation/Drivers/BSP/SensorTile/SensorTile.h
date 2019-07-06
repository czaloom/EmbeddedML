/**
  ******************************************************************************
  * @file    SensorTile.h
  * @author  Central Labs
  * @version V1.2.0
  * @date    10-Nov-2016
  * @brief   This file contains definitions for SensorTile.c file
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

/* IMPORTANT: in order to compile with RevA following flag shall be defined  */
/* in the preprocessor options:  USE_SENSORTILE_REVA !!!!!!!!!! */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SENSORTILE_H
#define __SENSORTILE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "accelerometer.h"
#include "gyroscope.h"
   

#define LSM303AGR_ACC_WHO_AM_I         0x33
#define LSM303AGR_MAG_WHO_AM_I         0x40
#define HTS221_WHO_AM_I_VAL         (uint8_t)0xBC

/** @addtogroup BSP
  * @{
  */

/** @addtogroup SENSORTILE
  * @{
  */
      
  /** @addtogroup SENSORTILE_LOW_LEVEL
  * @{
  */

/** @defgroup SENSORTILE_LOW_LEVEL_Exported_Types SENSORTILE_LOW_LEVEL Exported Types
  * @{
  */
typedef enum 
{
  LED1 = 0,
  LEDSWD = 1
}Led_TypeDef;


typedef enum
{
//  TEMPERATURE_SENSORS_AUTO = -1, /* Always first element and equal to -1 */
  LSM6DSM = 0,                  /* LSM6DSM. */
  LSM303AGR_X,                  /* LSM303AGR Accelerometer */
  LSM303AGR_M,                  /* LSM303AGR Magnetometer */
  LPS22HB                       /* LPS22HB */
} SPI_Device_t;

/**
  * @}
  */ 

/** @defgroup SENSORTILE_LOW_LEVEL_Exported_Constants SENSORTILE_LOW_LEVEL Exported Constants
  * @{
  */ 

#define LEDn                             2

#define LED1_PIN                         GPIO_PIN_12
#define LED1_GPIO_PORT                   GPIOG
#define LED1_GPIO_CLK_ENABLE()           __GPIOG_CLK_ENABLE()  
#define LED1_GPIO_CLK_DISABLE()          __GPIOG_CLK_DISABLE()  

#define LEDSWD_PIN                       GPIO_PIN_14
#define LEDSWD_GPIO_PORT                 GPIOA
#define LEDSWD_GPIO_CLK_ENABLE()         __GPIOA_CLK_ENABLE()  
#define LEDSWD_GPIO_CLK_DISABLE()        __GPIOA_CLK_DISABLE()  

#define LEDx_GPIO_CLK_ENABLE(__INDEX__)  do{if((__INDEX__) == 0) LED1_GPIO_CLK_ENABLE(); \
                                            if((__INDEX__) == 1) LEDSWD_GPIO_CLK_ENABLE(); \
                                            }while(0)
												
#define LEDx_GPIO_CLK_DISABLE(__INDEX__) do{if((__INDEX__) == 0) LED1_GPIO_CLK_DISABLE(); \
                                            if((__INDEX__) == 1) LEDSWD_GPIO_CLK_DISABLE(); \
                                            }while(0)

#define LSM6DSM_INT2_GPIO_PORT           GPIOA
#define LSM6DSM_INT2_GPIO_CLK_ENABLE()   __GPIOA_CLK_ENABLE()
#define LSM6DSM_INT2_GPIO_CLK_DISABLE()  __GPIOA_CLK_DISABLE()
#define LSM6DSM_INT2_PIN                 GPIO_PIN_2
#define LSM6DSM_INT2_EXTI_IRQn           EXTI2_IRQn

                                              
#define SENSORTILE_SENSORS_SPI                    SPI2

#define SENSORTILE_SENSORS_SPI_Port               GPIOB
#define SENSORTILE_SENSORS_SPI_MOSI_Pin           GPIO_PIN_15
#define SENSORTILE_SENSORS_SPI_SCK_Pin            GPIO_PIN_13

#define SENSORTILE_SENSORS_SPI_CLK_ENABLE()       __SPI2_CLK_ENABLE()
#define SENSORTILE_SENSORS_SPI_GPIO_CLK_ENABLE()  __GPIOB_CLK_ENABLE()

#define SENSORTILE_LSM6DSM_SPI_CS_Port	          GPIOB
#define SENSORTILE_LSM6DSM_SPI_CS_Pin     	  GPIO_PIN_12
#define SENSORTILE_LSM6DSM_SPI_CS_GPIO_CLK_ENABLE()  __GPIOB_CLK_ENABLE()
                                              
#define SENSORTILE_LSM303AGR_X_SPI_CS_Port	  GPIOC
#define SENSORTILE_LSM303AGR_X_SPI_CS_Pin     	  GPIO_PIN_4
#define SENSORTILE_LSM303AGR_X_SPI_CS_GPIO_CLK_ENABLE()  __GPIOC_CLK_ENABLE()
                                              
#define SENSORTILE_LSM303AGR_M_SPI_CS_Port	  GPIOB
#define SENSORTILE_LSM303AGR_M_SPI_CS_Pin     	  GPIO_PIN_1
#define SENSORTILE_LSM303AGR_M_SPI_CS_GPIO_CLK_ENABLE()  __GPIOB_CLK_ENABLE()
                                              
#define SENSORTILE_LPS22HB_SPI_CS_Port	          GPIOA
#define SENSORTILE_LPS22HB_SPI_CS_Pin     	  GPIO_PIN_3
#define SENSORTILE_LPS22HB_SPI_CS_GPIO_CLK_ENABLE()  __GPIOA_CLK_ENABLE()


                                              /* I2C clock speed configuration (in Hz) */
#define I2C_SENSORTILE_TIMING_1000KHZ      0x00D00E28 /* Analog Filter ON, Rise time 120ns, Fall time 25ns */
#define I2C_SENSORTILE_TIMING_100KHZ      0x10909CEC /* Analog Filter ON, Rise time 120ns, Fall time 25ns */
                                              
                                              /* I2C peripheral configuration defines */
#define SENSORTILE_I2C_ONBOARD_SENSORS                            I2C3
#define SENSORTILE_I2C_ONBOARD_SENSORS_CLK_ENABLE()               __I2C3_CLK_ENABLE()
#define SENSORTILE_I2C_ONBOARD_SENSORS_SCL_SDA_GPIO_CLK_ENABLE()  __GPIOC_CLK_ENABLE()
#define SENSORTILE_I2C_ONBOARD_SENSORS_RCC_PERIPHCLK              RCC_PERIPHCLK_I2C3
#define SENSORTILE_I2C_ONBOARD_SENSORS_I2CCLKSOURCE               RCC_I2C3CLKSOURCE_SYSCLK
#define SENSORTILE_I2C_ONBOARD_SENSORS_SCL_SDA_AF                 GPIO_AF4_I2C3			
#define SENSORTILE_I2C_ONBOARD_SENSORS_SCL_SDA_GPIO_PORT          GPIOC
#define SENSORTILE_I2C_ONBOARD_SENSORS_SCL_PIN                    GPIO_PIN_0
#define SENSORTILE_I2C_ONBOARD_SENSORS_SDA_PIN                    GPIO_PIN_1
                                              
#define SENSORTILE_I2C_ONBOARD_SENSORS_FORCE_RESET()              __I2C3_FORCE_RESET()
#define SENSORTILE_I2C_ONBOARD_SENSORS_RELEASE_RESET()            __I2C3_RELEASE_RESET()

/* I2C interrupt requests */                  
#define SENSORTILE_I2C_ONBOARD_SENSORS_EV_IRQn                    I2C3_EV_IRQn
#define SENSORTILE_I2C_ONBOARD_SENSORS_ER_IRQn                    I2C3_ER_IRQn

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define SENSORTILE_I2C_ONBOARD_SENSORS_TIMEOUT_MAX    0x1000 /*<! The value of the maximal timeout for BUS waiting loops */

#ifdef USE_FREERTOS
  #define SENSORTILE_I2C_ONBOARD_SENSORS_MUTEX                      I2C3_Mutex_id
  #define SENSORTILE_I2C_ONBOARD_SENSORS_MUTEX_TAKE()               osMutexWait(SENSORTILE_I2C_ONBOARD_SENSORS_MUTEX, 0)
  #define SENSORTILE_I2C_ONBOARD_SENSORS_MUTEX_RELEASE()            osMutexRelease(SENSORTILE_I2C_ONBOARD_SENSORS_MUTEX)
#else
  #define SENSORTILE_I2C_ONBOARD_SENSORS_MUTEX                      0
  #define SENSORTILE_I2C_ONBOARD_SENSORS_MUTEX_TAKE()               0
  #define SENSORTILE_I2C_ONBOARD_SENSORS_MUTEX_RELEASE()            0
#endif
                                              
                                              
/*##################### SD ###################################*/
/* Chip Select macro definition */
#define SENSORTILE_SD_CS_LOW()       HAL_GPIO_WritePin(SENSORTILE_SD_CS_GPIO_PORT, SENSORTILE_SD_CS_PIN, GPIO_PIN_RESET)
#define SENSORTILE_SD_CS_HIGH()      HAL_GPIO_WritePin(SENSORTILE_SD_CS_GPIO_PORT, SENSORTILE_SD_CS_PIN, GPIO_PIN_SET)

/**
  * @brief  SD Control Interface pins
  */
#define SENSORTILE_SD_CS_PIN                               GPIO_PIN_12
#define SENSORTILE_SD_CS_GPIO_PORT                         GPIOG
#define SENSORTILE_SD_CS_GPIO_CLK_ENABLE()                 __GPIOG_CLK_ENABLE()
#define SENSORTILE_SD_CS_GPIO_CLK_DISABLE()                __GPIOG_CLK_DISABLE()
      
#define SENSORTILE_SD_DUMMY_BYTE   0xFF
#define SENSORTILE_SD_NO_RESPONSE_EXPECTED 0x80
    
                                                
/*##################### SPI3 SensorTile ###################################*/
#define SENSORTILE_SD_SPI                               SPI3
#define SENSORTILE_SD_SPI_CLK_ENABLE()                  __SPI3_CLK_ENABLE()

#define SENSORTILE_SD_SPI_SCK_AF                        GPIO_AF6_SPI3
#define SENSORTILE_SD_SPI_SCK_GPIO_PORT                 GPIOG
#define SENSORTILE_SD_SPI_SCK_PIN                       GPIO_PIN_9
#define SENSORTILE_SD_SPI_SCK_GPIO_CLK_ENABLE()         __GPIOG_CLK_ENABLE()
#define SENSORTILE_SD_SPI_SCK_GPIO_CLK_DISABLE()        __GPIOG_CLK_DISABLE()

#define SENSORTILE_SD_SPI_MISO_MOSI_AF                  GPIO_AF6_SPI3
#define SENSORTILE_SD_SPI_MISO_MOSI_GPIO_PORT           GPIOG
#define SENSORTILE_SD_SPI_MISO_MOSI_GPIO_CLK_ENABLE()   __GPIOG_CLK_ENABLE()
#define SENSORTILE_SD_SPI_MISO_MOSI_GPIO_CLK_DISABLE()  __GPIOG_CLK_DISABLE()
#define SENSORTILE_SD_SPI_MISO_PIN                      GPIO_PIN_10
#define SENSORTILE_SD_SPI_MOSI_PIN                      GPIO_PIN_11
/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define SENSORTILE_SD_SPI_TIMEOUT_MAX                   1000

/**
  * @}
  */ 


/** @defgroup SENSORTILE_LOW_LEVEL_Exported_Macros SENSORTILE_LOW_LEVEL Exported Macros
  * @{
  */  
/**
  * @}
  */ 

/** @defgroup SENSORTILE_LOW_LEVEL_Exported_Functions SENSORTILE_LOW_LEVEL Exported Functions
  * @{
  */
uint32_t         BSP_GetVersion(void);  
void             BSP_LED_Init(Led_TypeDef Led);
void             BSP_LED_DeInit(Led_TypeDef Led);
void             BSP_LED_On(Led_TypeDef Led);
void             BSP_LED_Off(Led_TypeDef Led);
void             BSP_LED_Toggle(Led_TypeDef Led);
DrvStatusTypeDef Sensor_IO_I2C_Init( void );
DrvStatusTypeDef Sensor_IO_SPI_Init( void );
uint8_t Sensor_IO_Read( void *handle, uint8_t ReadAddr, uint8_t *pBuffer, uint16_t nBytesToRead );
uint8_t Sensor_IO_I2C_Read( void *handle, uint8_t ReadAddr, uint8_t *pBuffer, uint16_t nBytesToRead );
uint8_t Sensor_IO_SPI_Read( void *handle, uint8_t ReadAddr, uint8_t *pBuffer, uint16_t nBytesToRead );
uint8_t Sensor_IO_Write( void *handle, uint8_t WriteAddr, uint8_t *pBuffer, uint16_t nBytesToWrite );
uint8_t Sensor_IO_I2C_Write( void *handle, uint8_t WriteAddr, uint8_t *pBuffer, uint16_t nBytesToWrite );
uint8_t Sensor_IO_SPI_Write( void *handle, uint8_t WriteAddr, uint8_t *pBuffer, uint16_t nBytesToWrite );
uint8_t Sensor_IO_SPI_CS_Init_All(void);
uint8_t Sensor_IO_SPI_CS_Init(void *handle);
uint8_t Sensor_IO_SPI_CS_Enable(void *handle);
uint8_t Sensor_IO_SPI_CS_Disable(void *handle);
DrvStatusTypeDef LSM6DSM_Sensor_IO_ITConfig( void );

void SD_IO_CS_Init(void);
void SD_IO_CS_DeInit(void);

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

#ifdef __cplusplus
}
#endif

#endif /* __SENSORTILE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
