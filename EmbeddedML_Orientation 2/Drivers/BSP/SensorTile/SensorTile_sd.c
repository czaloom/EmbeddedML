/**
  ******************************************************************************
  * @file    SensorTile_sd.c
  * @author  Central Labs
  * @version V1.2.0
 * @date    10-Nov-2016
  * @brief   This file provides the SD card driver for the SensorTile
  *          board
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

/* File Info : -----------------------------------------------------------------
                                 User NOTES
This file implements a high level communication layer for read and write 
from/to this memory.
 
1. How To use this driver:
--------------------------
   - This driver is used to drive the micro SD external card mounted on SensorTile Cradle 
     evaluation board.
   - This driver does not need a specific component driver for the micro SD device
     to be included with.
 
------------------------------------------------------------------------------*/ 

/* Includes ------------------------------------------------------------------*/
#include "SensorTile_sd.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup SENSORTILE
  * @{
  */ 
  
/** @addtogroup SENSORTILE_SD
* @brief This file provides a set of firmware functions to manage SPI SD card 
* @{
*/ 
  
/* Private typedef -----------------------------------------------------------*/

/** @defgroup SENSORTILE_SD_Private_Types_Definitions SENSORTILE_SD Private Types Definitions
  * @{
  */ 

/**
  * @}
  */
  
/* Private define ------------------------------------------------------------*/

/** @defgroup SENSORTILE_SD_Private_Defines SENSORTILE_SD Private Defines
  * @{
  */
#define SD_DUMMY_BYTE   0xFF
#define SD_NO_RESPONSE_EXPECTED 0x80
/**
  * @}
  */
  
/* Private macro -------------------------------------------------------------*/

/** @defgroup SENSORTILE_SD_Private_Macros SENSORTILE_SD Private Macros
  * @{
  */  

/**
  * @}
  */
  
/* Private variables ---------------------------------------------------------*/

/** @defgroup SENSORTILE_SD_Private_Variables SENSORTILE_SD Private Variables
  * @{
  */       
__IO uint8_t SdStatus = SD_PRESENT;
__IO uint8_t SD_CardType = STD_CAPACITY_SD_CARD_V1_1;

/**
  * @}
  */ 

/* Private function prototypes -----------------------------------------------*/
static uint8_t SD_GetCIDRegister(SD_CID* Cid);
static uint8_t SD_GetCSDRegister(SD_CSD* Csd);
static SD_Info SD_GetDataResponse(void);
static uint8_t SD_GoIdleState(void);
static uint8_t SD_SendCmd(uint8_t Cmd, uint32_t Arg, uint8_t Crc, uint8_t Response);
static uint8_t SD_SendCmd_wResp(uint8_t Cmd, uint32_t Arg, uint8_t Crc);

/** @defgroup SENSORTILE_SD_Private_Function_Prototypes SENSORTILE_SD Private Function Prototypes
  * @{
  */ 
/**
  * @}
  */
 
/* Private functions ---------------------------------------------------------*/
    
/** @defgroup SENSORTILE_SD_Private_Functions SENSORTILE_SD Private Functions
  * @{
  */ 
  
/**
  * @brief  Initializes the SD/SD communication.
  * @param  None
  * @retval The SD Response: 
  *         - MSD_ERROR : Sequence failed
  *         - MSD_OK    : Sequence succeed
  */
uint8_t BSP_SD_Init(void)
{ 
  /* Configure IO functionalities for SD pin */
  SD_IO_Init_LS();
  
  if(SD_GoIdleState() == MSD_ERROR)
  {
    SdStatus = SD_NOT_PRESENT;
    return MSD_ERROR;
  }
  else
  {
    SdStatus = SD_PRESENT;
    
    if(SD_CardType != HIGH_CAPACITY_SD_CARD)
    {
      /* SD Card type not supported. Please use SDHC Card */
      return MSD_ERROR;
    }
    
    SD_IO_Init();
    return MSD_OK;
  }
}

/**
  * @brief  Returns information about specific card.
  * @param  cardinfo: pointer to a SD_CardInfo structure that contains all SD 
  *         card information.
  * @retval The SD Response:
  *         - MSD_ERROR : Sequence failed
  *         - MSD_OK    : Sequence succeed
  */
uint8_t BSP_SD_GetCardInfo(SD_CardInfo *pCardInfo)
{
  uint8_t status = MSD_ERROR;

  SD_GetCSDRegister(&(pCardInfo->Csd));
  status = SD_GetCIDRegister(&(pCardInfo->Cid));
  pCardInfo->CardCapacity = (pCardInfo->Csd.DeviceSize + 1) ;
  pCardInfo->CardCapacity *= (1 << (pCardInfo->Csd.DeviceSizeMul + 2));
  pCardInfo->CardBlockSize = 1 << (pCardInfo->Csd.RdBlockLen);
  pCardInfo->CardCapacity *= pCardInfo->CardBlockSize;

  /* Returns the reponse */
  return status;
}

/**
  * @brief  Reads block(s) from a specified address in an SD card, in polling mode. 
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  ReadAddr: Address from where data is to be read  
  * @param  BlockSize: SD card data block size, that should be 512
  * @param  NumOfBlocks: Number of SD blocks to read 
  * @retval SD status
  */
uint8_t BSP_SD_ReadBlocks(uint32_t* p32Data, uint64_t Sector, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
  uint32_t counter = 0, offset = 0;
  uint8_t rvalue = MSD_ERROR;
  uint8_t *pData = (uint8_t *)p32Data;
  
  /* Send CMD16 (SD_CMD_SET_BLOCKLEN) to set the size of the block and 
     Check if the SD acknowledged the set block length command: R1 response (0x00: no errors) */
  if (SD_IO_WriteCmd(SD_CMD_SET_BLOCKLEN, BlockSize, 0xFF, SD_RESPONSE_NO_ERROR) != HAL_OK)
  {
    return MSD_ERROR;
  }

  if(SD_CardType != HIGH_CAPACITY_SD_CARD)
  {
    Sector *= 512;
  }
  
  /* Data transfer */
  while (NumberOfBlocks--)
  {
    /* Send dummy byte: 8 Clock pulses of delay */
    SD_IO_WriteDummy();

    /* Send CMD17 (SD_CMD_READ_SINGLE_BLOCK) to read one block */
    /* Check if the SD acknowledged the read block command: R1 response (0x00: no errors) */
    if (SD_IO_WriteCmd(SD_CMD_READ_SINGLE_BLOCK, Sector + offset, 0xFF, SD_RESPONSE_NO_ERROR) != HAL_OK)
    {
      return MSD_ERROR;
    }

    /* Now look for the data token to signify the start of the data */
    if (SD_IO_WaitResponse(SD_START_DATA_SINGLE_BLOCK_READ) == HAL_OK)
    {
      /* Read the SD block data : read NumByteToRead data */
      for (counter = 0; counter < BlockSize; counter++)
      {
        /* Read the pointed data */
        *pData = SD_IO_ReadByte();
        /* Point to the next location where the byte read will be saved */
        pData++;
      }
      
      /* Set next write address */
      if(SD_CardType != HIGH_CAPACITY_SD_CARD)
      {
        offset += BlockSize;
      }
      else
      {
        offset += 1;
      }
      /* get CRC bytes (not really needed by us, but required by SD) */
      SD_IO_ReadByte();
      SD_IO_ReadByte();
      /* Set response value to success */
      rvalue = MSD_OK;
    }
    else
    {
      /* Set response value to failure */
      rvalue = MSD_ERROR;
    }
  }
  
  /* Send dummy byte: 8 Clock pulses of delay */
  SD_IO_WriteDummy();
  /* Returns the reponse */
  return rvalue;
}

/**
  * @brief  Writes block(s) to a specified address in an SD card, in polling mode. 
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  WriteAddr: Address from where data is to be written  
  * @param  BlockSize: SD card data block size, that should be 512
  * @param  NumOfBlocks: Number of SD blocks to write
  * @retval SD status
  */
uint8_t BSP_SD_WriteBlocks(uint32_t* p32Data, uint64_t Sector, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
  uint32_t counter = 0, offset = 0;
  uint8_t rvalue = MSD_ERROR;
  uint8_t *pData = (uint8_t *)p32Data;
  
  if(SD_CardType != HIGH_CAPACITY_SD_CARD)
  {
    Sector *= BlockSize;
  }
  
  /* Data transfer */
  while (NumberOfBlocks--)
  {
    /* Send CMD24 (SD_CMD_WRITE_SINGLE_BLOCK) to write blocks  and
       Check if the SD acknowledged the write block command: R1 response (0x00: no errors) */
    if (SD_IO_WriteCmd(SD_CMD_WRITE_SINGLE_BLOCK, Sector + offset, 0xFF, SD_RESPONSE_NO_ERROR) != HAL_OK)
    {
      return MSD_ERROR;
    }

    /* Send dummy byte */
    SD_IO_WriteByte(SD_DUMMY_BYTE);

    /* Send the data token to signify the start of the data */
    SD_IO_WriteByte(SD_START_DATA_SINGLE_BLOCK_WRITE);

    /* Write the block data to SD : write count data by block */
    for (counter = 0; counter < BlockSize; counter++)
    {
      /* Send the pointed byte */
      SD_IO_WriteByte(*pData);
      
      /* Point to the next location where the byte read will be saved */
      pData++;
    }

    /* Set next write address */
    if(SD_CardType != HIGH_CAPACITY_SD_CARD)
    {
      offset += BlockSize;
    }
    else
    {
      offset += 1;
    }

    /* Put CRC bytes (not really needed by us, but required by SD) */
    SD_IO_ReadByte();
    SD_IO_ReadByte();

    /* Read data response */
    if (SD_GetDataResponse() == SD_DATA_OK)
    {
      /* Set response value to success */
      rvalue = MSD_OK;
    }
    else
    {
      /* Set response value to failure */
      rvalue = MSD_ERROR;
    }
  }

  /* Send dummy byte: 8 Clock pulses of delay */
  SD_IO_WriteDummy();

  /* Returns the reponse */
  return rvalue;
}

/**
  * @brief  Read the CSD card register.
  *         Reading the contents of the CSD register in SPI mode is a simple 
  *         read-block transaction.
  * @param  Csd: pointer on an SCD register structure
  * @retval SD status
  */
uint8_t SD_GetCSDRegister(SD_CSD* Csd)
{
  uint32_t counter = 0;
  uint8_t rvalue = MSD_ERROR;
  uint8_t CSD_Tab[16];

  /* Send CMD9 (CSD register) or CMD10(CSD register) and Wait for response in the R1 format (0x00 is no errors) */
  if (SD_IO_WriteCmd(SD_CMD_SEND_CSD, 0, 0xFF, SD_RESPONSE_NO_ERROR) == HAL_OK)
  {
    if (SD_IO_WaitResponse(SD_START_DATA_SINGLE_BLOCK_READ) == HAL_OK)
    {
      for (counter = 0; counter < 16; counter++)
      {
        /* Store CSD register value on CSD_Tab */
        CSD_Tab[counter] = SD_IO_ReadByte();
      }

      /* Get CRC bytes (not really needed by us, but required by SD) */
      SD_IO_WriteByte(SD_DUMMY_BYTE);
      SD_IO_WriteByte(SD_DUMMY_BYTE);

      /* Set response value to success */
      rvalue = MSD_OK;
    }
  }
  /* Send dummy byte: 8 Clock pulses of delay */
  SD_IO_WriteDummy();

  if(rvalue == SD_RESPONSE_NO_ERROR)
  {
    /* Byte 0 */
    Csd->CSDStruct = (CSD_Tab[0] & 0xC0) >> 6;
    Csd->SysSpecVersion = (CSD_Tab[0] & 0x3C) >> 2;
    Csd->Reserved1 = CSD_Tab[0] & 0x03;

    /* Byte 1 */
    Csd->TAAC = CSD_Tab[1];

    /* Byte 2 */
    Csd->NSAC = CSD_Tab[2];

    /* Byte 3 */
    Csd->MaxBusClkFrec = CSD_Tab[3];

    /* Byte 4 */
    Csd->CardComdClasses = CSD_Tab[4] << 4;

    /* Byte 5 */
    Csd->CardComdClasses |= (CSD_Tab[5] & 0xF0) >> 4;
    Csd->RdBlockLen = CSD_Tab[5] & 0x0F;

    /* Byte 6 */
    Csd->PartBlockRead = (CSD_Tab[6] & 0x80) >> 7;
    Csd->WrBlockMisalign = (CSD_Tab[6] & 0x40) >> 6;
    Csd->RdBlockMisalign = (CSD_Tab[6] & 0x20) >> 5;
    Csd->DSRImpl = (CSD_Tab[6] & 0x10) >> 4;
    Csd->Reserved2 = 0; /*!< Reserved */

    Csd->DeviceSize = (CSD_Tab[6] & 0x03) << 10;

    /* Byte 7 */
    Csd->DeviceSize |= (CSD_Tab[7]) << 2;

    /* Byte 8 */
    Csd->DeviceSize |= (CSD_Tab[8] & 0xC0) >> 6;

    Csd->MaxRdCurrentVDDMin = (CSD_Tab[8] & 0x38) >> 3;
    Csd->MaxRdCurrentVDDMax = (CSD_Tab[8] & 0x07);

    /* Byte 9 */
    Csd->MaxWrCurrentVDDMin = (CSD_Tab[9] & 0xE0) >> 5;
    Csd->MaxWrCurrentVDDMax = (CSD_Tab[9] & 0x1C) >> 2;
    Csd->DeviceSizeMul = (CSD_Tab[9] & 0x03) << 1;
    /* Byte 10 */
    Csd->DeviceSizeMul |= (CSD_Tab[10] & 0x80) >> 7;
      
    Csd->EraseGrSize = (CSD_Tab[10] & 0x40) >> 6;
    Csd->EraseGrMul = (CSD_Tab[10] & 0x3F) << 1;

    /* Byte 11 */
    Csd->EraseGrMul |= (CSD_Tab[11] & 0x80) >> 7;
    Csd->WrProtectGrSize = (CSD_Tab[11] & 0x7F);

    /* Byte 12 */
    Csd->WrProtectGrEnable = (CSD_Tab[12] & 0x80) >> 7;
    Csd->ManDeflECC = (CSD_Tab[12] & 0x60) >> 5;
    Csd->WrSpeedFact = (CSD_Tab[12] & 0x1C) >> 2;
    Csd->MaxWrBlockLen = (CSD_Tab[12] & 0x03) << 2;

    /* Byte 13 */
    Csd->MaxWrBlockLen |= (CSD_Tab[13] & 0xC0) >> 6;
    Csd->WriteBlockPaPartial = (CSD_Tab[13] & 0x20) >> 5;
    Csd->Reserved3 = 0;
    Csd->ContentProtectAppli = (CSD_Tab[13] & 0x01);

    /* Byte 14 */
    Csd->FileFormatGrouop = (CSD_Tab[14] & 0x80) >> 7;
    Csd->CopyFlag = (CSD_Tab[14] & 0x40) >> 6;
    Csd->PermWrProtect = (CSD_Tab[14] & 0x20) >> 5;
    Csd->TempWrProtect = (CSD_Tab[14] & 0x10) >> 4;
    Csd->FileFormat = (CSD_Tab[14] & 0x0C) >> 2;
    Csd->ECC = (CSD_Tab[14] & 0x03);

    /* Byte 15 */
    Csd->CSD_CRC = (CSD_Tab[15] & 0xFE) >> 1;
    Csd->Reserved4 = 1;
  }
  /* Return the reponse */
  return rvalue;
}

/**
  * @brief  Read the CID card register.
  *         Reading the contents of the CID register in SPI mode is a simple 
  *         read-block transaction.
  * @param  Cid: pointer on an CID register structure
  * @retval SD status
  */
static uint8_t SD_GetCIDRegister(SD_CID* Cid)
{
  uint32_t counter = 0;
  uint8_t rvalue = MSD_ERROR;
  uint8_t CID_Tab[16];
  
  /* Send CMD10 (CID register) and Wait for response in the R1 format (0x00 is no errors) */
  if (SD_IO_WriteCmd(SD_CMD_SEND_CID, 0, 0xFF, SD_RESPONSE_NO_ERROR) == HAL_OK)
  {
    if (SD_IO_WaitResponse(SD_START_DATA_SINGLE_BLOCK_READ) == HAL_OK)
    {
      /* Store CID register value on CID_Tab */
      for (counter = 0; counter < 16; counter++)
      {
        CID_Tab[counter] = SD_IO_ReadByte();
      }

      /* Get CRC bytes (not really needed by us, but required by SD) */
      SD_IO_WriteByte(SD_DUMMY_BYTE);
      SD_IO_WriteByte(SD_DUMMY_BYTE);

      /* Set response value to success */
      rvalue = MSD_OK;
    }
  }

  /* Send dummy byte: 8 Clock pulses of delay */
  SD_IO_WriteDummy();

  if(rvalue == MSD_OK)
  {
    /* Byte 0 */
    Cid->ManufacturerID = CID_Tab[0];

    /* Byte 1 */
    Cid->OEM_AppliID = CID_Tab[1] << 8;

    /* Byte 2 */
    Cid->OEM_AppliID |= CID_Tab[2];

    /* Byte 3 */
    Cid->ProdName1 = CID_Tab[3] << 24;

    /* Byte 4 */
    Cid->ProdName1 |= CID_Tab[4] << 16;

    /* Byte 5 */
    Cid->ProdName1 |= CID_Tab[5] << 8;

    /* Byte 6 */
    Cid->ProdName1 |= CID_Tab[6];

    /* Byte 7 */
    Cid->ProdName2 = CID_Tab[7];

    /* Byte 8 */
    Cid->ProdRev = CID_Tab[8];

    /* Byte 9 */
    Cid->ProdSN = CID_Tab[9] << 24;

    /* Byte 10 */
    Cid->ProdSN |= CID_Tab[10] << 16;

    /* Byte 11 */
    Cid->ProdSN |= CID_Tab[11] << 8;

    /* Byte 12 */
    Cid->ProdSN |= CID_Tab[12];

    /* Byte 13 */
    Cid->Reserved1 |= (CID_Tab[13] & 0xF0) >> 4;
    Cid->ManufactDate = (CID_Tab[13] & 0x0F) << 8;

    /* Byte 14 */
    Cid->ManufactDate |= CID_Tab[14];

    /* Byte 15 */
    Cid->CID_CRC = (CID_Tab[15] & 0xFE) >> 1;
    Cid->Reserved2 = 1;
  }
  /* Return the reponse */
  return rvalue;
}

/**
  * @brief  Send 5 bytes command to the SD card and get response
  * @param  Cmd: The user expected command to send to SD card.
  * @param  Arg: The command argument.
  * @param  Crc: The CRC.
  * @param  Response: Expected response from the SD card
  * @retval SD status
  */
static uint8_t SD_SendCmd_wResp(uint8_t Cmd, uint32_t Arg, uint8_t Crc)
{
  return SD_IO_WriteCmd_wResp(Cmd, Arg, Crc);
}

/**
  * @brief  Send 5 bytes command to the SD card and get response
  * @param  Cmd: The user expected command to send to SD card.
  * @param  Arg: The command argument.
  * @param  Crc: The CRC.
  * @param  Response: Expected response from the SD card
  * @retval SD status
  */
static uint8_t SD_SendCmd(uint8_t Cmd, uint32_t Arg, uint8_t Crc, uint8_t Response)
{
  uint8_t status = MSD_ERROR;
  
  if(SD_IO_WriteCmd(Cmd, Arg, Crc, Response) == HAL_OK)
  {
    status = MSD_OK;
  }
  
  /* Send Dummy Byte */
  SD_IO_WriteDummy();

  return status;
}

/**
  * @brief  Get SD card data response.
  * @param  None
  * @retval The SD status: Read data response xxx0<status>1
  *         - status 010: Data accecpted
  *         - status 101: Data rejected due to a crc error
  *         - status 110: Data rejected due to a Write error.
  *         - status 111: Data rejected due to other error.
  */
static SD_Info SD_GetDataResponse(void)
{
  uint32_t counter = 0;
  SD_Info response, rvalue;

  while (counter <= 64)
  {
    /* Read response */
    response = (SD_Info)SD_IO_ReadByte();
    /* Mask unused bits */
    response &= 0x1F;
    switch (response)
    {
      case SD_DATA_OK:
      {
        rvalue = SD_DATA_OK;
        break;
      }
      case SD_DATA_CRC_ERROR:
        return SD_DATA_CRC_ERROR;
      case SD_DATA_WRITE_ERROR:
        return SD_DATA_WRITE_ERROR;
      default:
      {
        rvalue = SD_DATA_OTHER_ERROR;
        break;
      }
    }
    /* Exit loop in case of data ok */
    if (rvalue == SD_DATA_OK)
      break;
    /* Increment loop counter */
    counter++;
  }

  /* Wait null data */
  while (SD_IO_ReadByte() == 0);

  /* Return response */
  return response;
}

/**
  * @brief  Returns the SD status.
  * @param  None
  * @retval The SD status.
  */
uint8_t BSP_SD_GetStatus(void)
{
  return MSD_OK;
}

/**
  * @brief  Put SD in Idle state.
  * @param  None
  * @retval SD status
  */
static uint8_t SD_GoIdleState(void)
{
  uint8_t n, resp[4];
  
  /* Send CMD0 (SD_CMD_GO_IDLE_STATE) to put SD in SPI mode and 
     Wait for In Idle State Response (R1 Format) equal to 0x01 */
  if (SD_SendCmd(SD_CMD_GO_IDLE_STATE, 0, 0x95, SD_IN_IDLE_STATE) != MSD_OK)
  {
    /* No Idle State Response: return an error */
    return MSD_ERROR;
  }

  /* Send CMD8 */
  if(SD_SendCmd_wResp(SD_CMD_HS_SEND_EXT_CSD, 0x000001AA, 0x87) == 1)  /* SDv2? */
  {
    for (n = 0; n < 4; n++)
    {
      resp[n] = SD_IO_ReadByte();	/* Get 32 bit return value of R7 resp */
    }
    SD_IO_WriteDummy();
    
    if (resp[2] == 0x01 && resp[3] == 0xAA)  /* the card supports vcc of 2.7-3.6V? */
    {	
      do /* Wait for end of initialization with ACMD41(HCS) */
      {
        HAL_Delay(100);
        if(SD_SendCmd(SD_CMD_APP_CMD, 0, 0xFF, 1) != MSD_OK) /* CMD55 because following command id SD Card Specific */
        {
          return MSD_ERROR;
        }
      } while(SD_SendCmd(SD_CMD_SD_APP_OP_COND, 1UL << 30, 0xFF, SD_RESPONSE_NO_ERROR) != MSD_OK); /* ACMD41 */
      
      HAL_Delay(10);
      
      /* Read OCR register with CMD58 and check CCS flag (bit 30) */
      resp[0] = SD_SendCmd_wResp(SD_CMD_SDMMC_READ_OCR, 0, 0xFF);
      if(resp[0] == 0)
      {
        for (n = 0; n < 4; n++)
        {
          resp[n] = SD_IO_ReadByte();	/* Get 32 bit return value of R7 resp */
        }
        if(resp[0]&0x40)
        {
          SD_CardType = HIGH_CAPACITY_SD_CARD;  /* Card id SDv2 and High Capacity */
        }
        else
        {
          SD_CardType = STD_CAPACITY_SD_CARD_V2_0;  /* Card id SDv2 */
        }
      }
      SD_IO_WriteDummy();
    }
  }
  else /* Not SDv2 card */
  {
    SD_CardType = STD_CAPACITY_SD_CARD_V1_1;
  }
  
  SD_IO_WriteDummy();
  return MSD_OK;
}
/**
  * @brief  Erases the specified memory area of the given SD card. 
  * @param  StartAddr: Start byte address
  * @param  EndAddr: End byte address
  * @retval SD status
  */
uint8_t BSP_SD_Erase(uint32_t StartAddr, uint32_t EndAddr)
{
  uint8_t rvalue = MSD_ERROR;

  /* Send CMD32 (Erase group start) and check if the SD acknowledged the erase command: R1 response (0x00: no errors) */
  if (SD_SendCmd(SD_CMD_SD_ERASE_GRP_START, StartAddr, 0xFF, SD_RESPONSE_NO_ERROR) == MSD_OK)
  {
    /* Send CMD33 (Erase group end) and Check if the SD acknowledged the erase command: R1 response (0x00: no errors) */
    if (SD_SendCmd(SD_CMD_SD_ERASE_GRP_END, EndAddr, 0xFF, SD_RESPONSE_NO_ERROR) == MSD_OK)
    {
      /* Send CMD38 (Erase) and Check if the SD acknowledged the erase command: R1 response (0x00: no errors) */
      if (SD_SendCmd(SD_CMD_ERASE, 0, 0xFF, SD_RESPONSE_NO_ERROR) == MSD_OK)
      {
        /* Verify that SD card is ready to use after the specific command ERASE */
        if (SD_IO_WaitResponse(SD_RESPONSE_NO_ERROR) == HAL_OK)
          rvalue = MSD_OK;
      }
    }
  }
  
  /* Return the reponse */
  return rvalue;
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
