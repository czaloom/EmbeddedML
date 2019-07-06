/**
  ******************************************************************************
  * @file    DataLog/Inc/datalog_application.h
  * @author  Central Labs
  * @version V1.1.0
  * @date    27-Sept-2016
  * @brief   Header for datalog_application.c module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DATALOG_APPLICATION_H
#define __DATALOG_APPLICATION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cube_hal.h"
  
extern volatile uint8_t SD_Log_Enabled;


void DATALOG_SD_Init(void);
uint8_t DATALOG_SD_Log_Enable(void);
void DATALOG_SD_Log_Disable(void);
void DATALOG_SD_NewLine(void);
void RTC_Handler( RTC_HandleTypeDef *RtcHandle );
void Accelero_Sensor_Handler( void *handle );
void Gyro_Sensor_Handler( void *handle );
void Magneto_Sensor_Handler( void *handle );
void Temperature_Sensor_Handler( void *handle );
void Pressure_Sensor_Handler( void *handle );
void Humidity_Sensor_Handler( void *handle );
void floatToInt( float in, int32_t *out_int, int32_t *out_dec, int32_t dec_prec );
void Gas_Gauge_Handler( void *handle );


#ifdef __cplusplus
}
#endif

#endif /* __DATALOG_APPLICATION_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
