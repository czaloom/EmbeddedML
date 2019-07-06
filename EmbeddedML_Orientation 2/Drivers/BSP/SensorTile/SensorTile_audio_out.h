/**
******************************************************************************
* @file    SensorTile_audio_out.h
* @author  Central Labs
* @version V1.2.0
 * @date    10-Nov-2016
* @brief   This file contains definitions for SensorTile_audio_out.c driver.
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
#ifndef __SENSORTILE_AUDIO_OUT_H
#define __SENSORTILE_AUDIO_OUT_H

#ifdef __cplusplus
extern "C" {
#endif 
  
  /* Includes ------------------------------------------------------------------*/
#include "PCM1774_CODEC_driver.h"
#include "SensorTile.h"
  
  /** @addtogroup BSP
  * @{
  */
  
  /** @addtogroup SENSORTILE
  * @{
  */
  
  /** @addtogroup SENSORTILE_AUDIO_OUT
  * @{
  */
  
  /** @defgroup SENSORTILE_AUDIO_OUT_Private_Types SENSORTILE_AUDIO_OUT Private Types
  * @{
  */
  
  /**
  * @}
  */
  
  
  /** @addtogroup SENSORTILE_AUDIO_OUT_Exported_Types SENSORTILE_AUDIO_OUT Exported types
  * @{
  */
  typedef enum
  {
    CODEX_SENSORS_AUTO = -1,    /* Always first element and equal to -1 */
    PCM1774_0 = 0                   /* . */  
  } CODEX_ID_t;
  /**
  * @}
  */
  
  /** @defgroup SENSORTILE_AUDIO_OUT_Exported_Variables SENSORTILE_AUDIO_OUT Exported Variables
  * @{
  */
  extern DMA_HandleTypeDef hdma_dfsdmReg_FLT[];  
  /**
  * @}
  */
  
  
  /** @defgroup SENSORTILE_AUDIO_OUT_Exported_Constants SENSORTILE_AUDIO_OUT Exported Constants
  * @{
  */
  
#define CODEC_SENSORS_MAX_NUM 2
  
#ifndef AUDIO_OK
#define AUDIO_OK                            ((uint8_t)0)
#endif  
#ifndef AUDIO_ERROR
#define AUDIO_ERROR                         ((uint8_t)1)
#endif 
#ifndef AUDIO_TIMEOUT
#define AUDIO_TIMEOUT                       ((uint8_t)2)
#endif 
  
#ifndef AUDIO_FREQUENCY_192K
#define AUDIO_FREQUENCY_192K          ((uint32_t)192000)
#endif
#ifndef AUDIO_FREQUENCY_96K
#define AUDIO_FREQUENCY_96K           ((uint32_t)96000)
#endif
#ifndef AUDIO_FREQUENCY_48K
#define AUDIO_FREQUENCY_48K           ((uint32_t)48000)
#endif
#ifndef AUDIO_FREQUENCY_44K
#define AUDIO_FREQUENCY_44K           ((uint32_t)44100)
#endif
#ifndef AUDIO_FREQUENCY_32K
#define AUDIO_FREQUENCY_32K           ((uint32_t)32000)
#endif
#ifndef AUDIO_FREQUENCY_22K
#define AUDIO_FREQUENCY_22K           ((uint32_t)22050)
#endif
#ifndef AUDIO_FREQUENCY_16K
#define AUDIO_FREQUENCY_16K           ((uint32_t)16000)
#endif
#ifndef AUDIO_FREQUENCY_11K
#define AUDIO_FREQUENCY_11K           ((uint32_t)11025)
#endif
#ifndef AUDIO_FREQUENCY_8K
#define AUDIO_FREQUENCY_8K            ((uint32_t)8000)  
#endif
  
  
  /* To have 2 separate audio stream in Both headphone and speaker the 4 slot must be activated */
#define CODEC_AUDIOFRAME_SLOT_0123                   SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1 | SAI_SLOTACTIVE_2 | SAI_SLOTACTIVE_3
  /* To have an audio stream in headphone only SAI Slot 0 and Slot 2 must be activated */ 
#define CODEC_AUDIOFRAME_SLOT_02                     SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_2 
  /* To have an audio stream in speaker only SAI Slot 1 and Slot 3 must be activated */ 
#define CODEC_AUDIOFRAME_SLOT_13                     SAI_SLOTACTIVE_1 | SAI_SLOTACTIVE_3
  
  /* SAI peripheral configuration defines */
#define AUDIO_SAIx                            SAI2_Block_A
#define AUDIO_SAIx_CLK_ENABLE()               __HAL_RCC_SAI2_CLK_ENABLE()
#define AUDIO_SAIx_MCKB_SCKB_SDB_FSB_AF       GPIO_AF13_SAI2
  
#define AUDIO_SAIx_MCKB_SCKB_SDB_FSB_ENABLE()   __HAL_RCC_GPIOG_CLK_ENABLE()
#define AUDIO_SAIx_FSB_PIN                      GPIO_PIN_10
#define AUDIO_SAIx_SCKB_PIN                     GPIO_PIN_9
#define AUDIO_SAIx_SDB_PIN                      GPIO_PIN_12
#define AUDIO_SAIx_MCKB_PIN                     GPIO_PIN_11
#define AUDIO_SAIx_MCKB_SCKB_SDB_FSB_GPIO_PORT  GPIOG
  
  /* SAI DMA Channel definitions */
#define AUDIO_SAIx_DMAx_CLK_ENABLE()         __HAL_RCC_DMA2_CLK_ENABLE()
#define AUDIO_SAIx_DMAx_CHANNEL              DMA2_Channel3
#define AUDIO_SAIx_DMAx_IRQ                  DMA2_Channel3_IRQn
#define AUDIO_SAIx_DMAx_PERIPH_DATA_SIZE     DMA_PDATAALIGN_HALFWORD
#define AUDIO_SAIx_DMAx_MEM_DATA_SIZE        DMA_MDATAALIGN_HALFWORD
#define DMA_MAX_SZE                          (uint32_t)0xFFFF
  
#define AUDIO_SAIx_DMAx_IRQHandler           DMA2_Channel3_IRQHandler
  
  /* Select the interrupt preemption priority for the DMA interrupt */
#define AUDIO_OUT_IRQ_PREPRIO           7   /* Select the preemption priority level(0 is the highest) */   
  
  
  /**
  * @} 
  */
  
  
  /** @defgroup SENSORTILE_AUDIO_OUT_Exported_Macros SENSORTILE_AUDIO_OUT Exported Macros
  * @{
  */
#define DMA_MAX(_X_)                (((_X_) <= DMA_MAX_SZE)? (_X_):DMA_MAX_SZE)
  
  /**
  * @}
  */
  
  /** @defgroup SENSORTILE_AUDIO_OUT_Exported_Functions SENSORTILE_AUDIO_OUT Exported Functions
  * @{
  */
  uint8_t BSP_AUDIO_OUT_Init(CODEX_ID_t id, void **handle, uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq);
  uint8_t BSP_AUDIO_OUT_Play(void *handle, uint16_t* pBuffer, uint32_t Size);
  uint8_t BSP_AUDIO_OUT_Pause(void *handle);
  uint8_t BSP_AUDIO_OUT_Resume(void *handle);
  uint8_t BSP_AUDIO_OUT_SetFrequency(void *handle, uint32_t AudioFreq);
  uint8_t BSP_AUDIO_OUT_ClockConfig(uint32_t AudioFreq, void *Params);  
  uint8_t BSP_AUDIO_OUT_SetOutputMode(void *handle, uint8_t Output);
  uint8_t BSP_AUDIO_OUT_SetMute(void *handle, uint32_t Cmd);
  uint8_t BSP_AUDIO_OUT_SetVolume(void *handle, uint8_t Volume);
  uint8_t BSP_AUDIO_OUT_Stop(void *handle, uint32_t Option);    
  /* User Callbacks: user has to implement these functions in his code if they are needed. */
  /* These functions should be implemented by the user application.*/
  void    BSP_AUDIO_OUT_TransferComplete_CallBack(void);
  void    BSP_AUDIO_OUT_HalfTransfer_CallBack(void);  
  void    BSP_AUDIO_OUT_Error_CallBack(void);  
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

#endif /* __SENSORTILE_AUDIO_OUT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

