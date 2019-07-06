/**
 ******************************************************************************
 * @file    SensorTile_accelero.h
 * @author  Central Labs
 * @version V1.2.0
 * @date    10-Nov-2016
 * @brief   This file contains definitions for the SensorTile_accelero.c
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SENSORTILE_ACCELERO_H
#define __SENSORTILE_ACCELERO_H

#ifdef __cplusplus
extern "C" {
#endif



/* Includes ------------------------------------------------------------------*/

#include "LSM6DSM_ACC_GYRO_driver_HL.h"
#include "LSM303AGR_ACC_driver_HL.h"
#include "SensorTile.h"



/** @addtogroup BSP 
 * @{
 */

/** @addtogroup SENSORTILE
* @{
*/

/** @addtogroup SENSORTILE_ACCELERO
* @{
*/

/** @addtogroup SENSORTILE_ACCELERO_Exported_Types SENSORTILE_ACCELERO Exported types
  * @{
  */

typedef enum
{
  ACCELERO_SENSORS_AUTO = -1,    /* Always first element and equal to -1 */
  LSM6DSM_X_0,                   /* . */
  LSM303AGR_X_0                  /* . */
} ACCELERO_ID_t;

/**
 * @}
 */

/** @addtogroup SENSORTILE_ACCELERO_Exported_Defines SENSORTILE_ACCELERO Exported defines
  * @{
  */

#define ACCELERO_SENSORS_MAX_NUM 2

/**
 * @}
 */

/** @addtogroup SENSORTILE_ACCELERO_Exported_Function_Prototypes SENSORTILE_ACCELERO Exported function prototypes
 * @{
 */

/* Sensor Configuration Functions */
DrvStatusTypeDef BSP_ACCELERO_Init( ACCELERO_ID_t id, void **handle );
DrvStatusTypeDef BSP_ACCELERO_DeInit( void **handle );
DrvStatusTypeDef BSP_ACCELERO_Sensor_Enable( void *handle );
DrvStatusTypeDef BSP_ACCELERO_Sensor_Disable( void *handle );
DrvStatusTypeDef BSP_ACCELERO_IsInitialized( void *handle, uint8_t *status );
DrvStatusTypeDef BSP_ACCELERO_IsEnabled( void *handle, uint8_t *status );
DrvStatusTypeDef BSP_ACCELERO_IsCombo( void *handle, uint8_t *status );
DrvStatusTypeDef BSP_ACCELERO_Get_Instance( void *handle, uint8_t *instance );
DrvStatusTypeDef BSP_ACCELERO_Get_WhoAmI( void *handle, uint8_t *who_am_i );
DrvStatusTypeDef BSP_ACCELERO_Check_WhoAmI( void *handle );
DrvStatusTypeDef BSP_ACCELERO_Get_Axes( void *handle, SensorAxes_t *acceleration );
DrvStatusTypeDef BSP_ACCELERO_Get_AxesRaw( void *handle, SensorAxesRaw_t *value );
DrvStatusTypeDef BSP_ACCELERO_Get_Sensitivity( void *handle, float *sensitivity );
DrvStatusTypeDef BSP_ACCELERO_Get_ODR( void *handle, float *odr );
DrvStatusTypeDef BSP_ACCELERO_Set_ODR( void *handle, SensorOdr_t odr );
DrvStatusTypeDef BSP_ACCELERO_Set_ODR_Value( void *handle, float odr );
DrvStatusTypeDef BSP_ACCELERO_Get_FS( void *handle, float *fullScale );
DrvStatusTypeDef BSP_ACCELERO_Set_FS( void *handle, SensorFs_t fullScale );
DrvStatusTypeDef BSP_ACCELERO_Set_FS_Value( void *handle, float fullScale );
DrvStatusTypeDef BSP_ACCELERO_Get_Axes_Status( void *handle, uint8_t *xyz_enabled );
DrvStatusTypeDef BSP_ACCELERO_Set_Axes_Status( void *handle, uint8_t *enable_xyz );
DrvStatusTypeDef BSP_ACCELERO_Read_Reg( void *handle, uint8_t reg, uint8_t *data );
DrvStatusTypeDef BSP_ACCELERO_Write_Reg( void *handle, uint8_t reg, uint8_t data );
DrvStatusTypeDef BSP_ACCELERO_Get_DRDY_Status( void *handle, uint8_t *status );

DrvStatusTypeDef BSP_ACCELERO_Enable_Free_Fall_Detection_Ext( void *handle );
DrvStatusTypeDef BSP_ACCELERO_Disable_Free_Fall_Detection_Ext( void *handle );
DrvStatusTypeDef BSP_ACCELERO_Get_Free_Fall_Detection_Status_Ext( void *handle, uint8_t *status );
DrvStatusTypeDef BSP_ACCELERO_Set_Free_Fall_Threshold_Ext( void *handle, LSM6DSM_ACC_GYRO_FF_THS_t thr );

DrvStatusTypeDef BSP_ACCELERO_Enable_Pedometer_Ext( void *handle );
DrvStatusTypeDef BSP_ACCELERO_Disable_Pedometer_Ext( void *handle );
DrvStatusTypeDef BSP_ACCELERO_Get_Pedometer_Status_Ext( void *handle, uint8_t *status );
DrvStatusTypeDef BSP_ACCELERO_Get_Step_Count_Ext( void *handle, uint16_t *step_count );
DrvStatusTypeDef BSP_ACCELERO_Reset_Step_Counter_Ext( void *handle );
DrvStatusTypeDef BSP_ACCELERO_Set_Pedometer_Threshold_Ext( void *handle, uint8_t thr );

DrvStatusTypeDef BSP_ACCELERO_Enable_Tilt_Detection_Ext( void *handle );
DrvStatusTypeDef BSP_ACCELERO_Disable_Tilt_Detection_Ext( void *handle );
DrvStatusTypeDef BSP_ACCELERO_Get_Tilt_Detection_Status_Ext( void *handle, uint8_t *status );

DrvStatusTypeDef BSP_ACCELERO_Enable_Wake_Up_Detection_Ext( void *handle );
DrvStatusTypeDef BSP_ACCELERO_Disable_Wake_Up_Detection_Ext( void *handle );
DrvStatusTypeDef BSP_ACCELERO_Get_Wake_Up_Detection_Status_Ext( void *handle, uint8_t *status );
DrvStatusTypeDef BSP_ACCELERO_Set_Wake_Up_Threshold_Ext( void *handle, uint8_t thr );

DrvStatusTypeDef BSP_ACCELERO_Enable_Single_Tap_Detection_Ext( void *handle );
DrvStatusTypeDef BSP_ACCELERO_Disable_Single_Tap_Detection_Ext( void *handle );
DrvStatusTypeDef BSP_ACCELERO_Get_Single_Tap_Detection_Status_Ext( void *handle, uint8_t *status );
DrvStatusTypeDef BSP_ACCELERO_Enable_Double_Tap_Detection_Ext( void *handle );
DrvStatusTypeDef BSP_ACCELERO_Disable_Double_Tap_Detection_Ext( void *handle );
DrvStatusTypeDef BSP_ACCELERO_Get_Double_Tap_Detection_Status_Ext( void *handle, uint8_t *status );
DrvStatusTypeDef BSP_ACCELERO_Set_Tap_Threshold_Ext( void *handle, uint8_t thr );
DrvStatusTypeDef BSP_ACCELERO_Set_Tap_Shock_Time_Ext( void *handle, uint8_t time );
DrvStatusTypeDef BSP_ACCELERO_Set_Tap_Quiet_Time_Ext( void *handle, uint8_t time );
DrvStatusTypeDef BSP_ACCELERO_Set_Tap_Duration_Time_Ext( void *handle, uint8_t time );

DrvStatusTypeDef BSP_ACCELERO_Enable_6D_Orientation_Ext( void *handle );
DrvStatusTypeDef BSP_ACCELERO_Disable_6D_Orientation_Ext( void *handle );
DrvStatusTypeDef BSP_ACCELERO_Get_6D_Orientation_Status_Ext( void *handle, uint8_t *status );
DrvStatusTypeDef BSP_ACCELERO_Get_6D_Orientation_XL_Ext( void *handle, uint8_t *xl );
DrvStatusTypeDef BSP_ACCELERO_Get_6D_Orientation_XH_Ext( void *handle, uint8_t *xh );
DrvStatusTypeDef BSP_ACCELERO_Get_6D_Orientation_YL_Ext( void *handle, uint8_t *yl );
DrvStatusTypeDef BSP_ACCELERO_Get_6D_Orientation_YH_Ext( void *handle, uint8_t *yh );
DrvStatusTypeDef BSP_ACCELERO_Get_6D_Orientation_ZL_Ext( void *handle, uint8_t *zl );
DrvStatusTypeDef BSP_ACCELERO_Get_6D_Orientation_ZH_Ext( void *handle, uint8_t *zh );

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

#endif /* __SENSORTILE_ACCELERO_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
