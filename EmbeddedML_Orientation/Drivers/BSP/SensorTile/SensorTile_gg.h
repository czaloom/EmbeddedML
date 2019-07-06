/**
  ******************************************************************************
  * @file    SensorTile_gg.h
  * @author  Central Labs
  * @version V1.2.0
 * @date    10-Nov-2016
  * @brief   This file contains definitions for SensorTile_gg.c 
  *          firmware driver.
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
  

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SENSORTILE_GG_H
#define __SENSORTILE_GG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "SensorTile.h"
/* Include GG sensor component driver */
#include "../Components/stc3115/STC3115_Driver.h"   
	 
/** @addtogroup BSP
  * @{
  */
  
/** @addtogroup SENSORTILE
  * @{
  */ 

/** @addtogroup SENSORTILE_GG
  * @{
  */
  
/** @addtogroup SENSORTILE_GG_Exported_Types
  * @{
  */
typedef enum 
{
  GG_OK = 0,
  GG_ERROR = 1,
  GG_TIMEOUT = 2
} 
GG_StatusTypeDef;

typedef enum
{
  GG_AUTO = -1, /* Always first element and equal to -1 */
  STC3115_0,  /* GG on SensorTile Cradle */
} GG_ID_t;

/**
  * @}
  */
  
/** @addtogroup SENSORTILE_GG_Exported_Constants
  * @{
  */
#define GG_MAX_NUM     1
#define GG1            0
/**
  *@}
  */    
    

/** @addtogroup SENSORTILE_GG_Exported_Macros
  * @{
  */

/**
  * @}
  */
 
/** @addtogroup SENSORTILE_GG_Exported_Functions
  * @{
  */
/* Sensor Configuration Functions */ 
DrvStatusTypeDef BSP_GG_Init( void **handle );

DrvStatusTypeDef BSP_GG_DeInit( void **handle );
DrvStatusTypeDef BSP_GG_IsInitialized( void *handle, uint8_t *status );
DrvStatusTypeDef BSP_GG_IsCombo( void *handle, uint8_t *status );
DrvStatusTypeDef BSP_GG_Get_WhoAmI( void *handle, uint8_t *who_am_i );


DrvStatusTypeDef BSP_GG_Task( void *handle, uint8_t*);
DrvStatusTypeDef BSP_GG_Reset( void *handle );
DrvStatusTypeDef BSP_GG_Stop( void *handle );
DrvStatusTypeDef BSP_GG_GetOCV( void *handle, uint32_t* );
DrvStatusTypeDef BSP_GG_GetSOC( void *handle, uint32_t* );
DrvStatusTypeDef BSP_GG_GetChargeValue( void *handle, uint32_t* );
DrvStatusTypeDef BSP_GG_GetPresence( void *handle, uint32_t* );
DrvStatusTypeDef BSP_GG_GetCurrent( void *handle, int32_t* );
DrvStatusTypeDef BSP_GG_GetTemperature( void *handle, int32_t* );
DrvStatusTypeDef BSP_GG_GetVoltage( void *handle, uint32_t* );

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
  
#endif /* __SENSORTILE_GG_H */
  


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/ 
