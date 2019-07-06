/**
  ******************************************************************************
  * @file    SensorTile_sd.h
  * @author  Central Labs
  * @version V1.2.0
 * @date    10-Nov-2016
  * @brief   This file contains definitions for SensorTile_sd.c driver.
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
#ifndef __SENSORTILE_SD_H
#define __SENSORTILE_SD_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "SensorTile.h" 
   
/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup SENSORTILE
  * @{
  */
    
/** @defgroup SENSORTILE_SD
  * @{
  */    


/** @defgroup SENSORTILE_SD_Exported_Types SENSORTILE_SD Exported Types
  * @{
  */

/** 
  * @brief  SD status structure definition  
  */     
#define MSD_OK         0x00
#define MSD_ERROR      0x01

typedef enum
{
/**
  * @brief  SD reponses and error flags
  */
  SD_RESPONSE_NO_ERROR      = (0x00),
  SD_IN_IDLE_STATE          = (0x01),
  SD_ERASE_RESET            = (0x02),
  SD_ILLEGAL_COMMAND        = (0x04),
  SD_COM_CRC_ERROR          = (0x08),
  SD_ERASE_SEQUENCE_ERROR   = (0x10),
  SD_ADDRESS_ERROR          = (0x20),
  SD_PARAMETER_ERROR        = (0x40),
  SD_RESPONSE_FAILURE       = (0xFF),

/**
  * @brief  Data response error
  */
  SD_DATA_OK                = (0x05),
  SD_DATA_CRC_ERROR         = (0x0B),
  SD_DATA_WRITE_ERROR       = (0x0D),
  SD_DATA_OTHER_ERROR       = (0xFF)
}SD_Info;

/** 
  * @brief  Card Specific Data: CSD Register
  */ 
typedef struct
{
  __IO uint8_t  CSDStruct;            /* CSD structure */
  __IO uint8_t  SysSpecVersion;       /* System specification version */
  __IO uint8_t  Reserved1;            /* Reserved */
  __IO uint8_t  TAAC;                 /* Data read access-time 1 */
  __IO uint8_t  NSAC;                 /* Data read access-time 2 in CLK cycles */
  __IO uint8_t  MaxBusClkFrec;        /* Max. bus clock frequency */
  __IO uint16_t CardComdClasses;      /* Card command classes */
  __IO uint8_t  RdBlockLen;           /* Max. read data block length */
  __IO uint8_t  PartBlockRead;        /* Partial blocks for read allowed */
  __IO uint8_t  WrBlockMisalign;      /* Write block misalignment */
  __IO uint8_t  RdBlockMisalign;      /* Read block misalignment */
  __IO uint8_t  DSRImpl;              /* DSR implemented */
  __IO uint8_t  Reserved2;            /* Reserved */
  __IO uint32_t DeviceSize;           /* Device Size */
  __IO uint8_t  MaxRdCurrentVDDMin;   /* Max. read current @ VDD min */
  __IO uint8_t  MaxRdCurrentVDDMax;   /* Max. read current @ VDD max */
  __IO uint8_t  MaxWrCurrentVDDMin;   /* Max. write current @ VDD min */
  __IO uint8_t  MaxWrCurrentVDDMax;   /* Max. write current @ VDD max */
  __IO uint8_t  DeviceSizeMul;        /* Device size multiplier */
  __IO uint8_t  EraseGrSize;          /* Erase group size */
  __IO uint8_t  EraseGrMul;           /* Erase group size multiplier */
  __IO uint8_t  WrProtectGrSize;      /* Write protect group size */
  __IO uint8_t  WrProtectGrEnable;    /* Write protect group enable */
  __IO uint8_t  ManDeflECC;           /* Manufacturer default ECC */
  __IO uint8_t  WrSpeedFact;          /* Write speed factor */
  __IO uint8_t  MaxWrBlockLen;        /* Max. write data block length */
  __IO uint8_t  WriteBlockPaPartial;  /* Partial blocks for write allowed */
  __IO uint8_t  Reserved3;            /* Reserded */
  __IO uint8_t  ContentProtectAppli;  /* Content protection application */
  __IO uint8_t  FileFormatGrouop;     /* File format group */
  __IO uint8_t  CopyFlag;             /* Copy flag (OTP) */
  __IO uint8_t  PermWrProtect;        /* Permanent write protection */
  __IO uint8_t  TempWrProtect;        /* Temporary write protection */
  __IO uint8_t  FileFormat;           /* File Format */
  __IO uint8_t  ECC;                  /* ECC code */
  __IO uint8_t  CSD_CRC;              /* CSD CRC */
  __IO uint8_t  Reserved4;            /* always 1*/
} SD_CSD;

/** 
  * @brief  Card Identification Data: CID Register   
  */
typedef struct
{
  __IO uint8_t  ManufacturerID;       /* ManufacturerID */
  __IO uint16_t OEM_AppliID;          /* OEM/Application ID */
  __IO uint32_t ProdName1;            /* Product Name part1 */
  __IO uint8_t  ProdName2;            /* Product Name part2*/
  __IO uint8_t  ProdRev;              /* Product Revision */
  __IO uint32_t ProdSN;               /* Product Serial Number */
  __IO uint8_t  Reserved1;            /* Reserved1 */
  __IO uint16_t ManufactDate;         /* Manufacturing Date */
  __IO uint8_t  CID_CRC;              /* CID CRC */
  __IO uint8_t  Reserved2;            /* always 1 */
} SD_CID;

/** 
  * @brief SD Card information 
  */
typedef struct
{
  SD_CSD Csd;
  SD_CID Cid;
  uint32_t CardCapacity;  /* Card Capacity */
  uint32_t CardBlockSize; /* Card Block Size */
  uint16_t RCA;            /*!< SD relative card address               */
  uint8_t CardType;       /*!< SD card type                           */
} SD_CardInfo;

/**
  * @}
  */
  
/** @defgroup SENSORTILE_SD_Exported_Constants SENSORTILE_SD Exported Constants
  * @{
  */ 
  
    
    /** 
  * @brief Supported SD Memory Cards 
  */
#define STD_CAPACITY_SD_CARD_V1_1             ((uint32_t)0x00000000)
#define STD_CAPACITY_SD_CARD_V2_0             ((uint32_t)0x00000001)
#define HIGH_CAPACITY_SD_CARD                 ((uint32_t)0x00000002)
    
/**
  * @brief  Start Data tokens:
  *         Tokens (necessary because at nop/idle (and CS active) only 0xff is 
  *         on the data/command line)  
  */ 
#define SD_START_DATA_SINGLE_BLOCK_READ    0xFE  /* Data token start byte, Start Single Block Read */
#define SD_START_DATA_MULTIPLE_BLOCK_READ  0xFE  /* Data token start byte, Start Multiple Block Read */
#define SD_START_DATA_SINGLE_BLOCK_WRITE   0xFE  /* Data token start byte, Start Single Block Write */
#define SD_START_DATA_MULTIPLE_BLOCK_WRITE 0xFD  /* Data token start byte, Start Multiple Block Write */
#define SD_STOP_DATA_MULTIPLE_BLOCK_WRITE  0xFD  /* Data toke stop byte, Stop Multiple Block Write */

/**
  * @brief  SD detection on its memory slot
  */
#define SD_PRESENT               ((uint8_t)0x01)
#define SD_NOT_PRESENT           ((uint8_t)0x00)
   
/** 
  * @brief SD Commands Index 
  */
#define SD_CMD_GO_IDLE_STATE                       ((uint8_t)0)   /*!< Resets the SD memory card.                                                               */
#define SD_CMD_SEND_OP_COND                        ((uint8_t)1)   /*!< Sends host capacity support information and activates the card's initialization process. */
#define SD_CMD_ALL_SEND_CID                        ((uint8_t)2)   /*!< Asks any card connected to the host to send the CID numbers on the CMD line.             */
#define SD_CMD_SET_REL_ADDR                        ((uint8_t)3)   /*!< Asks the card to publish a new relative address (RCA).                                   */
#define SD_CMD_SET_DSR                             ((uint8_t)4)   /*!< Programs the DSR of all cards.                                                           */
#define SD_CMD_SDMMC_SEN_OP_COND                   ((uint8_t)5)   /*!< Sends host capacity support information (HCS) and asks the accessed card to send its 
                                                                       operating condition register (OCR) content in the response on the CMD line.              */
#define SD_CMD_HS_SWITCH                           ((uint8_t)6)   /*!< Checks switchable function (mode 0) and switch card function (mode 1).                   */
#define SD_CMD_SEL_DESEL_CARD                      ((uint8_t)7)   /*!< Selects the card by its own relative address and gets deselected by any other address    */
#define SD_CMD_HS_SEND_EXT_CSD                     ((uint8_t)8)   /*!< Sends SD Memory Card interface condition, which includes host supply voltage information 
                                                                       and asks the card whether card supports voltage.                                         */
#define SD_CMD_SEND_CSD                            ((uint8_t)9)   /*!< Addressed card sends its card specific data (CSD) on the CMD line.                       */
#define SD_CMD_SEND_CID                            ((uint8_t)10)  /*!< Addressed card sends its card identification (CID) on the CMD line.                      */
#define SD_CMD_READ_DAT_UNTIL_STOP                 ((uint8_t)11)  /*!< SD card doesn't support it.                                                              */
#define SD_CMD_STOP_TRANSMISSION                   ((uint8_t)12)  /*!< Forces the card to stop transmission.                                                    */
#define SD_CMD_SEND_STATUS                         ((uint8_t)13)  /*!< Addressed card sends its status register.                                                */
#define SD_CMD_HS_BUSTEST_READ                     ((uint8_t)14) 
#define SD_CMD_GO_INACTIVE_STATE                   ((uint8_t)15)  /*!< Sends an addressed card into the inactive state.                                         */
#define SD_CMD_SET_BLOCKLEN                        ((uint8_t)16)  /*!< Sets the block length (in bytes for SDSC) for all following block commands 
                                                                       (read, write, lock). Default block length is fixed to 512 Bytes. Not effective 
                                                                       for SDHS and SDXC.                                                                       */
#define SD_CMD_READ_SINGLE_BLOCK                   ((uint8_t)17)  /*!< Reads single block of size selected by SET_BLOCKLEN in case of SDSC, and a block of 
                                                                       fixed 512 bytes in case of SDHC and SDXC.                                                */
#define SD_CMD_READ_MULT_BLOCK                     ((uint8_t)18)  /*!< Continuously transfers data blocks from card to host until interrupted by 
                                                                       STOP_TRANSMISSION command.                                                               */
#define SD_CMD_HS_BUSTEST_WRITE                    ((uint8_t)19)  /*!< 64 bytes tuning pattern is sent for SDR50 and SDR104.                                    */
#define SD_CMD_WRITE_DAT_UNTIL_STOP                ((uint8_t)20)  /*!< Speed class control command.                                                             */
#define SD_CMD_SET_BLOCK_COUNT                     ((uint8_t)23)  /*!< Specify block count for CMD18 and CMD25.                                                 */
#define SD_CMD_WRITE_SINGLE_BLOCK                  ((uint8_t)24)  /*!< Writes single block of size selected by SET_BLOCKLEN in case of SDSC, and a block of 
                                                                       fixed 512 bytes in case of SDHC and SDXC.                                                */
#define SD_CMD_WRITE_MULT_BLOCK                    ((uint8_t)25)  /*!< Continuously writes blocks of data until a STOP_TRANSMISSION follows.                    */
#define SD_CMD_PROG_CID                            ((uint8_t)26)  /*!< Reserved for manufacturers.                                                              */
#define SD_CMD_PROG_CSD                            ((uint8_t)27)  /*!< Programming of the programmable bits of the CSD.                                         */
#define SD_CMD_SET_WRITE_PROT                      ((uint8_t)28)  /*!< Sets the write protection bit of the addressed group.                                    */
#define SD_CMD_CLR_WRITE_PROT                      ((uint8_t)29)  /*!< Clears the write protection bit of the addressed group.                                  */
#define SD_CMD_SEND_WRITE_PROT                     ((uint8_t)30)  /*!< Asks the card to send the status of the write protection bits.                           */
#define SD_CMD_SD_ERASE_GRP_START                  ((uint8_t)32)  /*!< Sets the address of the first write block to be erased. (For SD card only).              */
#define SD_CMD_SD_ERASE_GRP_END                    ((uint8_t)33)  /*!< Sets the address of the last write block of the continuous range to be erased.           */
#define SD_CMD_ERASE_GRP_START                     ((uint8_t)35)  /*!< Sets the address of the first write block to be erased. Reserved for each command 
                                                                       system set by switch function command (CMD6).                                            */
#define SD_CMD_ERASE_GRP_END                       ((uint8_t)36)  /*!< Sets the address of the last write block of the continuous range to be erased. 
                                                                       Reserved for each command system set by switch function command (CMD6).                  */
#define SD_CMD_ERASE                               ((uint8_t)38)  /*!< Reserved for SD security applications.                                                   */
#define SD_CMD_FAST_IO                             ((uint8_t)39)  /*!< SD card doesn't support it (Reserved).                                                   */
#define SD_CMD_GO_IRQ_STATE                        ((uint8_t)40)  /*!< SD card doesn't support it (Reserved).                                                   */
#define SD_CMD_LOCK_UNLOCK                         ((uint8_t)42)  /*!< Sets/resets the password or lock/unlock the card. The size of the data block is set by 
                                                                       the SET_BLOCK_LEN command.                                                               */
#define SD_CMD_APP_CMD                             ((uint8_t)55)  /*!< Indicates to the card that the next command is an application specific command rather 
                                                                       than a standard command.                                                                 */
#define SD_CMD_GEN_CMD                             ((uint8_t)56)  /*!< Used either to transfer a data block to the card or to get a data block from the card 
                                                                       for general purpose/application specific commands.                                       */
#define SD_CMD_SDMMC_READ_OCR                      ((uint8_t)58)  /*!<  READ_OCR                                                                                */
#define SD_CMD_NO_CMD                              ((uint8_t)64) 

                                                                         
/** 
  * @brief Following commands are SD Card Specific commands.
  *        SDMMC_APP_CMD should be sent before sending these commands. 
  */
#define SD_CMD_APP_SD_SET_BUSWIDTH                 ((uint8_t)6)   /*!< (ACMD6) Defines the data bus width to be used for data transfer. The allowed data bus 
                                                                       widths are given in SCR register.                                                          */
#define SD_CMD_SD_APP_STATUS                       ((uint8_t)13)  /*!< (ACMD13) Sends the SD status.                                                              */
#define SD_CMD_SD_APP_SEND_NUM_WRITE_BLOCKS        ((uint8_t)22)  /*!< (ACMD22) Sends the number of the written (without errors) write blocks. Responds with 
                                                                       32bit+CRC data block.                                                                      */
#define SD_CMD_SD_APP_OP_COND                      ((uint8_t)41)  /*!< (ACMD41) Sends host capacity support information (HCS) and asks the accessed card to 
                                                                       send its operating condition register (OCR) content in the response on the CMD line.       */
#define SD_CMD_SD_APP_SET_CLR_CARD_DETECT          ((uint8_t)42)  /*!< (ACMD42) Connects/Disconnects the 50 KOhm pull-up resistor on CD/DAT3 (pin 1) of the card. */
#define SD_CMD_SD_APP_SEND_SCR                     ((uint8_t)51)  /*!< Reads the SD Configuration Register (SCR).                                                 */
#define SD_CMD_SDMMC_RW_DIRECT                     ((uint8_t)52)  /*!< For SD I/O card only, reserved for security specification.                                 */
#define SD_CMD_SDMMC_RW_EXTENDED                   ((uint8_t)53)  /*!< For SD I/O card only, reserved for security specification.                                 */
                                                                         
/**
  * @}
  */
  
/** @defgroup SENSORTILE_SD_Exported_Macro SENSORTILE_SD Exported Macro
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup SENSORTILE_SD_Exported_Functions SENSORTILE_SD Exported Functions
  * @{
  */   
uint8_t BSP_SD_Init(void);
uint8_t BSP_SD_IsDetected(void);
uint8_t BSP_SD_ReadBlocks(uint32_t* p32Data, uint64_t Sector, uint16_t BlockSize, uint32_t NumberOfBlocks);
uint8_t BSP_SD_WriteBlocks(uint32_t* p32Data, uint64_t Sector, uint16_t BlockSize, uint32_t NumberOfBlocks);
uint8_t BSP_SD_Erase(uint32_t StartAddr, uint32_t EndAddr);
uint8_t BSP_SD_GetStatus(void);
uint8_t BSP_SD_GetCardInfo(SD_CardInfo *pCardInfo);
   
/* Link functions for SD Card peripheral*/
void                    SD_IO_Init(void); /*high speed*/
void                    SD_IO_Init_LS(void);/*low speed*/

void                    SD_IO_WriteByte(uint8_t Data);
uint8_t                 SD_IO_ReadByte(void);
HAL_StatusTypeDef       SD_IO_WriteCmd(uint8_t Cmd, uint32_t Arg, uint8_t Crc, uint8_t Response);
uint8_t SD_IO_WriteCmd_wResp(uint8_t Cmd, uint32_t Arg, uint8_t Crc);
HAL_StatusTypeDef       SD_IO_WaitResponse(uint8_t Response);
void                    SD_IO_WriteDummy(void);

#ifdef __cplusplus
}
#endif

#endif /* __SENSORTILE_SD_H */

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
