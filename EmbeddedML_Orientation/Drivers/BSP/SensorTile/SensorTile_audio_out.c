/**
******************************************************************************
* @file    SensorTile_audio_out.c
* @author  Central Labs
* @version V1.2.0
 * @date    10-Nov-2016
* @brief   This file provides the Audio Out driver for the Sensor Tile
*          expansion board
*******************************************************************************
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
#include "stm32l4xx_hal.h"
#include "SensorTile_audio_out.h"

/** @addtogroup BSP
* @{
*/

/** @addtogroup SENSORTILE
* @{
*/

/** @addtogroup SENSORTILE_AUDIO_OUT
* @brief This file provides a set of firmware functions to manage audio output codec.
* @{
*/ 

/** @defgroup SENSORTILE_AUDIO_OUT_Private_Types SENSORTILE_AUDIO_OUT Private Types
* @{
*/ 

/**
* @}
*/ 

/** @defgroup SENSORTILE_AUDIO_OUT_Private_Defines SENSORTILE_AUDIO_OUT Private Defines 
* @{
*/ 

/**
* @}
*/ 

/** @defgroup SENSORTILE_AUDIO_OUT_Private_Macros SENSORTILE_AUDIO_OUT Private Macros
* @{
*/

#define SAIClockDivider(__FREQUENCY__) \
((__FREQUENCY__ == AUDIO_FREQUENCY_8K)  ? 12 \
  : (__FREQUENCY__ == AUDIO_FREQUENCY_11K) ? 2 \
    : (__FREQUENCY__ == AUDIO_FREQUENCY_16K) ? 6 \
      : (__FREQUENCY__ == AUDIO_FREQUENCY_22K) ? 1 \
        : (__FREQUENCY__ == AUDIO_FREQUENCY_32K) ? 3 \
          : (__FREQUENCY__ == AUDIO_FREQUENCY_44K) ? 0 \
            : (__FREQUENCY__ == AUDIO_FREQUENCY_48K) ? 2 : 1)

/**
* @}
*/ 

/** @defgroup SENSORTILE_AUDIO_OUT_Private_Variables SENSORTILE_AUDIO_OUT Private Variables
* @{
*/

static DrvContextTypeDef CODEC_Handle[CODEC_SENSORS_MAX_NUM ];


SAI_HandleTypeDef                 haudio_out_sai;
static void SAIx_MspInit(void);
static void SAIx_Init(uint32_t AudioFreq);



/**
* @}
*/ 

/** @defgroup SENSORTILE_AUDIO_OUT_Private_Function_Prototypes SENSORTILE_AUDIO_OUT Private Function Prototypes
* @{
*/ 
static DrvStatusTypeDef BSP_PCM1774_CODEC_Init( void **handle, uint8_t Volume, uint32_t AudioFreq);

/**
* @}
*/ 


/** @addtogroup SENSORTILE_AUDIO_OUT_Exported_Functions SENSORTILE_AUDIO_OUT Exported Functions
* @{
*/ 

/**
* @brief  Configures the audio peripherals.
* @param  OutputDevice: OUTPUT_DEVICE_SPEAKER, OUTPUT_DEVICE_HEADPHONE,
*                       or OUTPUT_DEVICE_BOTH.
* @param  Volume: Initial volume level (from 0 (Mute) to 63 (Max))
* @param  AudioFreq: Audio frequency used to play the audio stream.
* @note   The SAI PLL input clock must be done in the user application.  
* @retval AUDIO_OK if correct communication, else wrong communication
*/
uint8_t BSP_AUDIO_OUT_Init(CODEX_ID_t id, void **handle, uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq)
{ 
  if (AudioFreq != AUDIO_FREQUENCY_8K && AudioFreq != AUDIO_FREQUENCY_16K && AudioFreq && AudioFreq != AUDIO_FREQUENCY_32K && AudioFreq != AUDIO_FREQUENCY_48K)
    return COMPONENT_ERROR;
  
  BSP_AUDIO_OUT_ClockConfig(AudioFreq, NULL);
  
  *handle = NULL;  
  
  switch(id)
  {
  case CODEX_SENSORS_AUTO:
  default:
    {  
      /* Try to init the LSM6DSM before */
      if(BSP_PCM1774_CODEC_Init(handle, Volume, AudioFreq) == COMPONENT_ERROR )
      {  
        return COMPONENT_ERROR;    
      }
      break;
    }
  case PCM1774_0:
    {
      if( BSP_PCM1774_CODEC_Init(handle, Volume, AudioFreq) == COMPONENT_ERROR )
      {
        return COMPONENT_ERROR;
      }
      break;
    }
  }   
    
  /* SAI data transfer preparation:
  Prepare the Media to be used for the audio transfer from memory to SAI peripheral */
  SAIx_Init(AudioFreq);
  
  return COMPONENT_OK;  
}

static DrvStatusTypeDef BSP_PCM1774_CODEC_Init( void **handle, uint8_t Volume, uint32_t AudioFreq)
{   
  AUDIO_DrvTypeDef *driver = NULL;
  
  if(CODEC_Handle[PCM1774_0].isInitialized == 1)
  {
    /* We have reached the max num of instance for this component */
    return COMPONENT_ERROR;
  }
  
  if (Sensor_IO_I2C_Init() == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }
  
  /* Setup handle. */
  CODEC_Handle[PCM1774_0].who_am_i      = 0;
  CODEC_Handle[PCM1774_0].ifType        = 0; // I2C interface
  CODEC_Handle[PCM1774_0].address       = PCM1774_CODEC_I2C_ADDRESS_LOW;
  CODEC_Handle[PCM1774_0].instance      = PCM1774_0;
  CODEC_Handle[PCM1774_0].isInitialized = 0;
  CODEC_Handle[PCM1774_0].isEnabled     = 0;
  CODEC_Handle[PCM1774_0].isCombo       = 1;
  CODEC_Handle[PCM1774_0].pData         = ( void * )NULL;
  CODEC_Handle[PCM1774_0].pVTable       = ( void * )&PCM1774_drv;
  CODEC_Handle[PCM1774_0].pExtVTable    = 0;
  
  *handle = (void *)&CODEC_Handle[PCM1774_0];
  
  driver = ( AUDIO_DrvTypeDef * )((DrvContextTypeDef *)(*handle))->pVTable;
  
  if ( driver->Init == NULL )
  {
    memset((*handle), 0, sizeof(DrvContextTypeDef));
    *handle = NULL;
    return COMPONENT_ERROR;
  }
  
  if (driver->Init( (DrvContextTypeDef *)(*handle), Volume, AudioFreq ) == COMPONENT_ERROR )
  {
    memset((*handle), 0, sizeof(DrvContextTypeDef));
    *handle = NULL;
    return COMPONENT_ERROR;
  }
  
  return COMPONENT_OK;   
}

/**
* @brief  Starts playing audio stream from a data buffer for a determined size. 
* @param  pBuffer: Pointer to the buffer 
* @param  Size: Number of audio data BYTES.
* @retval AUDIO_OK if correct communication, else wrong communication
*/
uint8_t BSP_AUDIO_OUT_Play(void *handle, uint16_t* pBuffer, uint32_t Size)
{
  /* Start DMA transfer of PCM samples towards the serial audio interface */  
  if ( HAL_SAI_Transmit_DMA(&haudio_out_sai, (uint8_t *)pBuffer, DMA_MAX(Size))!= HAL_OK)
  {
    return AUDIO_ERROR;
  }
  
  return AUDIO_OK;
}

/**
* @brief  This function Pauses the audio file stream. In case
*         of using DMA, the DMA Pause feature is used.
* @note When calling BSP_AUDIO_OUT_Pause() function for pause, only
*       BSP_AUDIO_OUT_Resume() function should be called for resume
*       (use of BSP_AUDIO_OUT_Play() function for resume could lead 
*       to unexpected behavior).
* @retval AUDIO_OK if correct communication, else wrong communication
*/
uint8_t BSP_AUDIO_OUT_Pause(void *handle)
{
  /* Pause DMA transfer of PCM samples towards the serial audio interface */  
  if (HAL_SAI_DMAPause(&haudio_out_sai)!= HAL_OK)
  {
    return AUDIO_ERROR;
  }
  
  return AUDIO_OK;
}

/**
* @brief  This function  Resumes the audio file stream.  
* @note When calling BSP_AUDIO_OUT_Pause() function for pause, only
*       BSP_AUDIO_OUT_Resume() function should be called for resume
*       (use of BSP_AUDIO_OUT_Play() function for resume could lead to 
*       unexpected behavior).
* @retval AUDIO_OK if correct communication, else wrong communication
*/
uint8_t BSP_AUDIO_OUT_Resume(void *handle)
{    
  /* Call the Audio Codec Resume function */
  /* Resume DMA transfer of PCM samples towards the serial audio interface */  
  if (HAL_SAI_DMAResume(&haudio_out_sai)!= HAL_OK)
  {
    return AUDIO_ERROR;
  }  
  return AUDIO_OK;
}

/**
* @brief  Stops audio playing and Power down the Audio Codec. 
* @param  Option: could be one of the following parameters 
*           - CODEC_PDWN_SW: for software power off (by writing registers). 
*                            Then no need to reconfigure the Codec after power on.
*           - CODEC_PDWN_HW: completely shut down the codec (physically). 
*                            Then need to reconfigure the Codec after power on.  
* @retval AUDIO_OK if correct communication, else wrong communication
*/
uint8_t BSP_AUDIO_OUT_Stop(void *handle, uint32_t Option)
{
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  AUDIO_DrvTypeDef *driver = NULL;
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( AUDIO_DrvTypeDef *)ctx->pVTable;
  
  if ( driver->Stop == NULL )
  {
    return COMPONENT_ERROR;
  }
  
  if ( driver->Stop(ctx, Option ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }
  
  /* Stop DMA transfer of PCM samples towards the serial audio interface */  
  if (HAL_SAI_DMAStop(&haudio_out_sai)!= HAL_OK)
  {
    return COMPONENT_ERROR;   
  }
  return COMPONENT_OK;
}

/**
* @brief  Controls the current audio volume level. 
* @param  Volume: Volume level to be set in percentage from 0 to 63 (0 for 
*         Mute and 63 for Max volume level).
* @retval AUDIO_OK if correct communication, else wrong communication
*/

uint8_t BSP_AUDIO_OUT_SetVolume(void *handle, uint8_t Volume)
{
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  AUDIO_DrvTypeDef *driver = NULL;
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( AUDIO_DrvTypeDef *)ctx->pVTable;
  
  if(Volume == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  if ( driver->SetVolume == NULL )
  {
    return COMPONENT_ERROR;
  }
  
  if ( driver->SetVolume(ctx, Volume ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }
  
  return COMPONENT_OK;  
}

/**
* @brief  Enables or disables the MUTE mode by software 
* @param  Cmd: Could be AUDIO_MUTE_ON to mute sound or AUDIO_MUTE_OFF to 
*         unmute the codec and restore previous volume level.
* @retval AUDIO_OK if correct communication, else wrong communication
*/
uint8_t BSP_AUDIO_OUT_SetMute(void *handle, uint32_t Cmd)
{ 
  DrvContextTypeDef *ctx = (DrvContextTypeDef *)handle;
  AUDIO_DrvTypeDef *driver = NULL;
  
  if(ctx == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  driver = ( AUDIO_DrvTypeDef *)ctx->pVTable;
  
  if(Cmd == NULL)
  {
    return COMPONENT_ERROR;
  }
  
  if ( driver->SetMute == NULL )
  {
    return COMPONENT_ERROR;
  }
  
  if ( driver->SetMute(ctx, Cmd ) == COMPONENT_ERROR )
  {
    return COMPONENT_ERROR;
  }
  
  return COMPONENT_OK;
}


/**
* @brief  Clock Config.
* @param  Params: additional parameters where required
* @param  AudioFreq: Audio frequency used to play the audio stream.
* @note   This API is called by BSP_AUDIO_OUT_Init() 
*         Being __weak it can be overwritten by the application     
* @retval AUDIO_OK if no problem during execution, AUDIO_ERROR otherwise
*/
__weak uint8_t BSP_AUDIO_OUT_ClockConfig(uint32_t AudioFreq, void *Params)
{ 
  RCC_PeriphCLKInitTypeDef RCC_ExCLKInitStruct;  
  HAL_RCCEx_GetPeriphCLKConfig(&RCC_ExCLKInitStruct);
  
  /* SAI clock config 
  PLLSAI1_VCO= 8 Mhz * PLLSAI1N = 8 * 43 = VCO_344M 
  SAI_CK_x = PLLSAI1_VCO/PLLSAI1P = 344/7 = 49.142 Mhz */  
  RCC_ExCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SAI2;
  RCC_ExCLKInitStruct.Sai2ClockSelection = RCC_SAI2CLKSOURCE_PLLSAI1;
  RCC_ExCLKInitStruct.PLLSAI1.PLLSAI1N = 43;
  RCC_ExCLKInitStruct.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  RCC_ExCLKInitStruct.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  RCC_ExCLKInitStruct.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1CFGR_PLLSAI1PEN;
  HAL_RCCEx_PeriphCLKConfig(&RCC_ExCLKInitStruct); 
  
  return AUDIO_OK;
}

/**
* @brief  Update the audio frequency.
* @param  AudioFreq: Audio frequency used to play the audio stream.
* @note   This API should be called after the BSP_AUDIO_OUT_Init() to adjust the
*         audio frequency.
* @retval None
*/
uint8_t BSP_AUDIO_OUT_SetFrequency(void *handle, uint32_t AudioFreq)
{ 
  BSP_AUDIO_OUT_ClockConfig(AudioFreq, NULL);
  /* Disable SAI peripheral to allow access to SAI internal registers */
  __HAL_SAI_DISABLE(&haudio_out_sai);
  
  /* Update the SAI audio frequency configuration */
  haudio_out_sai.Init.Mckdiv = SAIClockDivider(AudioFreq);
  HAL_SAI_Init(&haudio_out_sai);
  
  /* Enable SAI peripheral to generate MCLK */
  __HAL_SAI_ENABLE(&haudio_out_sai);
  
  return AUDIO_OK;
}

/**
* @brief  Tx Transfer completed callbacks.
* @param  hsai: SAI handle
* @retval None
*/
void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai)
{
  /* Manage the remaining file size and new address offset: This function 
  should be coded by user (its prototype is already declared in stm32l476g_eval_audio.h) */
  BSP_AUDIO_OUT_TransferComplete_CallBack();
}

/**
* @brief  Tx Half Transfer completed callbacks.
* @param  hsai: SAI handle
* @retval None
*/
void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
  /* Manage the remaining file size and new address offset: This function 
  should be coded by user (its prototype is already declared in stm32l476g_eval_audio.h) */
  BSP_AUDIO_OUT_HalfTransfer_CallBack();
}

/**
* @brief  SAI error callbacks.
* @param  hsai: SAI handle
* @retval None
*/
void HAL_SAI_ErrorCallback(SAI_HandleTypeDef *hsai)
{
  BSP_AUDIO_OUT_Error_CallBack();
}

/**
* @brief  Manages the DMA full Transfer complete event.
* @retval None
*/
__weak void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{
}

/**
* @brief  Manages the DMA Half Transfer complete event.
* @retval None
*/
__weak void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{
}

/**
* @brief  Manages the DMA FIFO error event.
* @retval None
*/
__weak void BSP_AUDIO_OUT_Error_CallBack(void)
{
}

/**
* @}
*/


/**
* @brief  Initializes SAI MSP.
* @retval None
*/
static void SAIx_MspInit(void)
{ 
  static DMA_HandleTypeDef hdma_saiTx;
  GPIO_InitTypeDef  GPIO_InitStruct;  
  SAI_HandleTypeDef *hsai = &haudio_out_sai;
  
  /* Enable SAI clock */
  AUDIO_SAIx_CLK_ENABLE();
  
  /* Enable GPIO clock */
  AUDIO_SAIx_MCKB_SCKB_SDB_FSB_ENABLE();
  
  /* enable USB power for GPIOG */
  HAL_PWREx_EnableVddIO2();
  
  /* CODEC_SAI pins configuration: FS, SCK, MCK and SD pins ------------------*/
  GPIO_InitStruct.Pin = AUDIO_SAIx_FSB_PIN | AUDIO_SAIx_SCKB_PIN | AUDIO_SAIx_SDB_PIN | AUDIO_SAIx_MCKB_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = AUDIO_SAIx_MCKB_SCKB_SDB_FSB_AF;
  HAL_GPIO_Init(AUDIO_SAIx_MCKB_SCKB_SDB_FSB_GPIO_PORT, &GPIO_InitStruct);
  
  /* Enable the DMA clock */
  AUDIO_SAIx_DMAx_CLK_ENABLE();
  
  if(hsai->Instance == AUDIO_SAIx)
  {
    /* Configure the hdma_saiTx handle parameters */   
    hdma_saiTx.Init.Request             = DMA_REQUEST_1;
    hdma_saiTx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    hdma_saiTx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_saiTx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_saiTx.Init.PeriphDataAlignment = AUDIO_SAIx_DMAx_PERIPH_DATA_SIZE;
    hdma_saiTx.Init.MemDataAlignment    = AUDIO_SAIx_DMAx_MEM_DATA_SIZE;
    hdma_saiTx.Init.Mode                = DMA_CIRCULAR;
    hdma_saiTx.Init.Priority            = DMA_PRIORITY_HIGH;
    
    hdma_saiTx.Instance = AUDIO_SAIx_DMAx_CHANNEL;
    
    /* Associate the DMA handle */
    __HAL_LINKDMA(hsai, hdmatx, hdma_saiTx);
    
    /* Deinitialize the Stream for new transfer */
    HAL_DMA_DeInit(&hdma_saiTx);
    
    /* Configure the DMA Stream */
    HAL_DMA_Init(&hdma_saiTx);      
  }
  
  /* SAI DMA IRQ Channel configuration */
  HAL_NVIC_SetPriority(AUDIO_SAIx_DMAx_IRQ, AUDIO_OUT_IRQ_PREPRIO, 0);
  HAL_NVIC_EnableIRQ(AUDIO_SAIx_DMAx_IRQ); 
}

/**
* @brief  Initializes the Audio Codec audio interface (SAI).
* @param  AudioFreq: Audio frequency to be configured for the SAI peripheral.
* @note   The default SlotActive configuration is set to CODEC_AUDIOFRAME_SLOT_0123 
*         and user can update this configuration using 
* @retval None
*/
static void SAIx_Init(uint32_t AudioFreq)
{
  /* Initialize the haudio_out_sai Instance parameter */
  haudio_out_sai.Instance = AUDIO_SAIx;
  
  /* Disable SAI peripheral to allow access to SAI internal registers */
  __HAL_SAI_DISABLE(&haudio_out_sai);
  
  /* Configure SAI_Block_x 
  LSBFirst: Disabled 
  DataSize: 16 */
  haudio_out_sai.Init.MonoStereoMode = SAI_STEREOMODE;
  haudio_out_sai.Init.Mckdiv = SAIClockDivider(AudioFreq);
  haudio_out_sai.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_MCKDIV;
  haudio_out_sai.Init.AudioMode = SAI_MODEMASTER_TX;
  haudio_out_sai.Init.NoDivider = SAI_MASTERDIVIDER_ENABLE;
  haudio_out_sai.Init.Protocol = SAI_FREE_PROTOCOL;
  haudio_out_sai.Init.DataSize = SAI_DATASIZE_16;
  haudio_out_sai.Init.FirstBit = SAI_FIRSTBIT_MSB;
  haudio_out_sai.Init.ClockStrobing = SAI_CLOCKSTROBING_RISINGEDGE;
  haudio_out_sai.Init.Synchro = SAI_ASYNCHRONOUS;
  haudio_out_sai.Init.OutputDrive = SAI_OUTPUTDRIVE_ENABLE;
  haudio_out_sai.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_1QF;
  
  /* Configure SAI_Block_x Frame 
  Frame Length: 64
  Frame active Length: 32
  FS Definition: Start frame + Channel Side identification
  FS Polarity: FS active Low
  FS Offset: FS asserted one bit before the first bit of slot 0 */ 
  haudio_out_sai.FrameInit.FrameLength = 32; 
  haudio_out_sai.FrameInit.ActiveFrameLength = 16;
  haudio_out_sai.FrameInit.FSDefinition = SAI_FS_CHANNEL_IDENTIFICATION;
  haudio_out_sai.FrameInit.FSPolarity = SAI_FS_ACTIVE_LOW;
  haudio_out_sai.FrameInit.FSOffset = SAI_FS_BEFOREFIRSTBIT;
  
  /* Configure SAI Block_x Slot 
  Slot First Bit Offset: 0
  Slot Size  : 16
  Slot Number: 4
  Slot Active: All slot actives */
  haudio_out_sai.SlotInit.FirstBitOffset = 0;
  haudio_out_sai.SlotInit.SlotSize = SAI_SLOTSIZE_16B;
  haudio_out_sai.SlotInit.SlotNumber = 2; 
  haudio_out_sai.SlotInit.SlotActive = SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1;
  
  /* Init the SAI */
  SAIx_MspInit();
  
  HAL_SAI_Init(&haudio_out_sai);
  
  /* Enable SAI peripheral to generate MCLK */
  __HAL_SAI_ENABLE(&haudio_out_sai);
}
/**************************/

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
