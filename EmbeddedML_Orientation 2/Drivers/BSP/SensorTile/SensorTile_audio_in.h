/**
******************************************************************************
* @file    SensorTile_audio_in.h
* @author  Central Labs
* @version V1.2.0
 * @date    10-Nov-2016
* @brief   This file contains definitions for SensorTile_audio_in.c driver.
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
#ifndef __SENSORTILE_AUDIO_IN_H
#define __SENSORTILE_AUDIO_IN_H

#ifdef __cplusplus
extern "C" {
#endif 
  
  /* Includes ------------------------------------------------------------------*/
  
  /** @addtogroup BSP
  * @{
  */
  
  /** @addtogroup SENSORTILE
  * @{
  */
  
  /** @addtogroup SENSORTILE_AUDIO_IN
  * @{
  */
  
  /** @defgroup SENSORTILE_AUDIO_IN_Private_Types SENSORTILE_AUDIO_IN Private Types
  * @{
  */
  
  /**
  * @brief   HP filter state structure definition
  */
  typedef struct {
    int32_t Z; 
    int32_t oldOut; 
    int32_t oldIn; 
  }HP_FilterState_TypeDef;
  
  /**
  * @brief   Microphone internal structure definition
  */
  typedef struct {
    uint32_t MicChannels;                       /*!< Specifies the number of channels */
    
    uint32_t Sampling_Freq;                     /*!< Specifies the desired sampling frequency */
    
    HP_FilterState_TypeDef HP_Filters[4];       /*!< HP filter state for each channel*/
    
    uint16_t * PCM_Data;                        /*!< Takes track of the external PCM data buffer
    as passed by the user in the start function*/
    
  } 
  SENSORTILE_AudioIn_HandlerTypeDef;
  
  /**
  * @}
  */
  
  
  /** @defgroup SENSORTILE_AUDIO_IN_Exported_Constants SENSORTILE_AUDIO_IN Exported Constants
  * @{
  */
  
  /* Select the interrupt preemption priority and subpriority for the IT/DMA interrupt */
#define AUDIO_IN_IRQ_PREPRIO          6   /* Select the preemption priority level(0 is the highest) */
  
  /*------------------------------------------------------------------------------
  CONFIGURATION: Audio Driver Configuration parameters
  ------------------------------------------------------------------------------*/
  
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
  
#define DEFAULT_AUDIO_IN_VOLUME               64
  
  /**
  * @}
  */
  
  /** @defgroup SENSORTILE_AUDIO_IN_Exported_Variables SENSORTILE_AUDIO_IN Exported Variables
  * @{
  */
  extern DMA_HandleTypeDef hdma_dfsdmReg_FLT[];
  
  /**
  * @}
  */
  
  /** @defgroup SENSORTILE_AUDIO_IN_Exported_Macros SENSORTILE_AUDIO_IN Exported Macros
  * @{
  */
  
  
  /**
  * @}
  */
  
  /** @defgroup SENSORTILE_AUDIO_IN_Exported_Constants SENSORTILE_AUDIO_IN Exported Constants
  * @{
  */ 
#define MAX_CH_NUMBER           (4)
#define MAX_FS                  (96000)
#define MAX_SAMPLES_PER_CH      ((MAX_FS/1000)*2)
  
  /* AUDIO FREQUENCY */
#define AUDIO_FREQUENCY_192K          ((uint32_t)192000)
#define AUDIO_FREQUENCY_96K           ((uint32_t)96000)
#define AUDIO_FREQUENCY_48K           ((uint32_t)48000)
#define AUDIO_FREQUENCY_44K           ((uint32_t)44100)
#define AUDIO_FREQUENCY_32K           ((uint32_t)32000)
#define AUDIO_FREQUENCY_22K           ((uint32_t)22050)
#define AUDIO_FREQUENCY_16K           ((uint32_t)16000)
#define AUDIO_FREQUENCY_11K           ((uint32_t)11025)
#define AUDIO_FREQUENCY_8K            ((uint32_t)8000)  
  
  /* DFSDM Configuration defines */
  
#define AUDIO_IN_DFSDM_CLK_ENABLE()                             __HAL_RCC_DFSDM_CLK_ENABLE()
  
#define AUDIO_IN_DFSDM_1st_CHANNEL                              DFSDM_Channel5
#define AUDIO_IN_DFSDM_2nd_CHANNEL                              DFSDM_Channel1
#define AUDIO_IN_DFSDM_3rd_CHANNEL                              DFSDM_Channel7
#define AUDIO_IN_DFSDM_4rd_CHANNEL                              DFSDM_Channel6
  
#define AUDIO_IN_DFSDM_1st_FILTER                               DFSDM_Filter0
#define AUDIO_IN_DFSDM_2st_FILTER                               DFSDM_Filter1
#define AUDIO_IN_DFSDM_3rd_FILTER                               DFSDM_Filter2
#define AUDIO_IN_DFSDM_4th_FILTER                               DFSDM_Filter3
  
#define AUDIO_IN_DFSDM_CKOUT_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOC_CLK_ENABLE()
#define AUDIO_IN_DFSDM_CKOUT_GPIO_PORT                          GPIOC
#define AUDIO_IN_DFSDM_CKOUT_PIN                                GPIO_PIN_2
  
#define AUDIO_IN_DFSDM_CH12_DATAIN_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOB_CLK_ENABLE()
#define AUDIO_IN_DFSDM_CH12_DATIN_GPIO_PORT                     GPIOB
#define AUDIO_IN_DFSDM_CH12_DATIN_PIN                           GPIO_PIN_6
  
#define AUDIO_IN_DFSDM_CH12_EXT_DATAIN_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOB_CLK_ENABLE()
#define AUDIO_IN_DFSDM_CH12_EXT_DATIN_GPIO_PORT                     GPIOB
#define AUDIO_IN_DFSDM_CH12_EXT_DATIN_PIN                           GPIO_PIN_8
  
#define AUDIO_IN_DFSDM_CH34_EXT_DATAIN_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOC_CLK_ENABLE()
#define AUDIO_IN_DFSDM_CH34_EXT_DATIN_GPIO_PORT                     GPIOC
#define AUDIO_IN_DFSDM_CH34_EXT_DATIN_PIN                           GPIO_PIN_0
  
#define AUDIO_IN_DFSDM_CKOUT_DATIN_AF                           GPIO_AF6_DFSDM
  
  
#define AUDIO_IN_DFSDM_DMA_CLK_ENABLE()                         __HAL_RCC_DMA1_CLK_ENABLE()
  
#define AUDIO_IN_DFSDM_DMA_1st_CHANNEL                          DMA1_Channel4
#define AUDIO_IN_DFSDM_DMA_2nd_CHANNEL                          DMA1_Channel5
#define AUDIO_IN_DFSDM_DMA_3rd_CHANNEL                          DMA1_Channel6
#define AUDIO_IN_DFSDM_DMA_4th_CHANNEL                          DMA1_Channel7  
  
#define AUDIO_IN_DFSDM_DMA_1st_CH_IRQn                          DMA1_Channel4_IRQn
#define AUDIO_IN_DFSDM_DMA_2nd_CH_IRQn                          DMA1_Channel5_IRQn
#define AUDIO_IN_DFSDM_DMA_3rd_CH_IRQn                          DMA1_Channel6_IRQn
#define AUDIO_IN_DFSDM_DMA_4th_CH_IRQn                          DMA1_Channel7_IRQn  
  
#define AUDIO_IN_DFSDM_DMA_PERIPH_DATA_SIZE                     DMA_PDATAALIGN_WORD
#define AUDIO_IN_DFSDM_DMA_MEM_DATA_SIZE                        DMA_MDATAALIGN_WORD
  
#define AUDIO_IN_DFSDM_DMA_1st_CH_IRQHandler                    DMA1_Channel4_IRQHandler
#define AUDIO_IN_DFSDM_DMA_2nd_CH_IRQHandler                    DMA1_Channel5_IRQHandler
#define AUDIO_IN_DFSDM_DMA_3rd_CH_IRQHandler                    DMA1_Channel6_IRQHandler
#define AUDIO_IN_DFSDM_DMA_4th_CH_IRQHandler                    DMA1_Channel7_IRQHandler  
  
  /**
  * @}
  */
  
  
  /** @defgroup SENSORTILE_AUDIO_IN_Exported_Functions SENSORTILE_AUDIO_IN Exported Functions
  * @{
  */  
  uint8_t BSP_AUDIO_IN_SetVolume(uint8_t Volume);
  uint8_t BSP_AUDIO_IN_Init(uint32_t AudioFreq, uint32_t BitRes, uint32_t ChnlNbr);
  uint8_t BSP_AUDIO_IN_ClockConfig(uint32_t AudioFreq, void *Params);
  uint8_t BSP_AUDIO_IN_Record(uint16_t* pbuf, uint32_t size);
  uint8_t BSP_AUDIO_IN_Stop(void);
  uint8_t BSP_AUDIO_IN_Pause(void);
  uint8_t BSP_AUDIO_IN_Resume(void);
  
  /* User Callbacks: user has to implement these functions in his code if they are needed. */
  /* These functions should be implemented by the user application.*/
  void BSP_AUDIO_IN_TransferComplete_CallBack(void);
  void BSP_AUDIO_IN_HalfTransfer_CallBack(void);
  void BSP_AUDIO_IN_Error_Callback(void);
  
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

#endif /* __SENSORTILE_AUDIO_IN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

