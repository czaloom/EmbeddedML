/**
  ******************************************************************************
  * @file    STC3115_Driver
  * @author  AST/EST
  * @version V1.0.0
  * @date    07-October-2014
  * @brief   This file contains all the functions prototypes for the stc3115.c
  *          driver.
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
#ifndef __STC3115_DRIVER_H__
#define __STC3115_DRIVER_H__

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "GasGauge.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 
  
/** @addtogroup STC3115
  * @{
  */

/** @addtogroup STC3115_Public_Constants Public constants
 * @{
 */
 
  /* Private define --------------------------------------------------------------------- */
#define STC3115_SLAVE_ADDRESS            0xE0    /* STC31xx 8-bit address byte 	(0xE0 8 bit)		*/

/*STC3115 registers define ------------------------------------------------------------ */
#define STC3115_REG_MODE                 0x00    /* Mode Register             			*/
#define STC3115_REG_CTRL                 0x01    /* Control and Status Register 		*/
#define STC3115_REG_SOC                  0x02    /* SOC Data (2 bytes) 					*/
#define STC3115_REG_COUNTER              0x04    /* Number of Conversion (2 bytes) 		*/
#define STC3115_REG_CURRENT              0x06    /* Battery Current (2 bytes) 			*/
#define STC3115_REG_VOLTAGE              0x08    /* Battery Voltage (2 bytes) 			*/
#define STC3115_REG_TEMPERATURE          0x0A    /* Temperature               			*/
#define STC3115_REG_CC_ADJ_HIGH          0x0B    /* CC adjustement     					*/
#define STC3115_REG_CC_ADJ_LOW           0x19    /* CC adjustement     					*/
#define STC3115_REG_VM_ADJ_HIGH          0x0C    /* VM adjustement     					*/
#define STC3115_REG_VM_ADJ_LOW           0x1A    /* VM adjustement     					*/
#define STC3115_REG_OCV                  0x0D    /* Battery OCV (2 bytes) 				*/
#define STC3115_REG_CC_CNF               0x0F    /* CC configuration (2 bytes)    		*/
#define STC3115_REG_VM_CNF               0x11    /* VM configuration (2 bytes)    		*/
#define STC3115_REG_ALARM_SOC            0x13    /* SOC alarm level         			*/
#define STC3115_REG_ALARM_VOLTAGE        0x14    /* Low voltage alarm level 			*/
#define STC3115_REG_CURRENT_THRES        0x15    /* Current threshold for relaxation 	*/
#define STC3115_REG_RELAX_COUNT          0x16    /* Voltage relaxation counter   		*/
#define STC3115_REG_RELAX_MAX            0x17    /* Voltage relaxation max count 		*/
#define STC3115_REG_ID			 		 0x18
#define STC3115_REG_RAM     			 0x20    /* General Purpose RAM Registers 		*/
#define STC3115_REG_OCVTAB               0x30	 /* OCV OFFSET table registers			*/

/*STC3115 STC3115_REG_MODE Bit mask definition ------------------------------------ */
#define STC3115_VMODE   	0x01	 	/* Voltage mode bit mask     				*/
#define STC3115_CLR_VM_ADJ  0x02  		/* Clear VM ADJ register bit mask 			*/
#define STC3115_CLR_CC_ADJ  0x04  		/* Clear CC ADJ register bit mask 			*/
#define STC3115_ALM_ENA		0x08	 	/* Alarm enable bit mask     				*/
#define STC3115_GG_RUN		0x10	 	/* Standby mode     				*/
#define STC3115_FORCE_CC	0x20	 	/* Force CC bit mask     					*/
#define STC3115_FORCE_VM	0x40	 	/* Force VM bit mask     					*/

/*STC3115 STC3115_REG_CTRL Bit mask definition ------------------------------------ */
//ALM TBD
#define STC3115_GG_RST		0x02		/* Convertion counter reset					*/
#define STC3115_GG_VM		0x04		/* STC3115 active mode: cc=0, VM=1			*/
#define STC3115_BATFAIL		0x08		/* Battery presence state					*/
#define STC3115_PORDET		0x10	 	/* W = soft reset, R = POR detect			*/
#define STC3115_ALM_SOC		0x20	 	/* Low SOC alarm event						*/
#define STC3115_ALM_VOLT		0x40	 	/* Low voltage alarm event					*/

/*STC3115 General purpose define ---------------------------------------------------------- */
#define STC3115_ID          0x14    	/* STC3115 ID 										*/
#define RAM_SIZE            16      	/* Total RAM size of STC3115 in bytes 				*/
#define OCVTAB_SIZE         16      	/* OCVTAB size of STC3115 in bytes 					*/
#define VCOUNT				4       	/* counter value for 1st current/temp measurements	*/

#define VM_MODE 			1       /*Voltage gas gauge with power saving Coulomb counter is not used. No current sensing*/
#define MIXED_MODE			0       /*Mixed mode, Coulomb counter is active, voltage gas gauge runs in parallel*/

#define CC_MODE 			0 

#define MAX_HRSOC          	51200  		/* 100% in 1/512% units								*/
#define MAX_SOC            	1000   		/* 100% in 0.1% units 								*/
#define BATT_OK				0
#define BATT_NOK			1
#define OK 0
#define VoltageFactor  		9011      	/* LSB=2.20mV ~9011/4096 - convert to mV         	*/
#define CurrentFactor		24084		/* LSB=5.88uV/R= ~24084/R/4096 - convert to mA  	*/

#define RAM_TSTWORD 		0x53A9		/* STC3115 RAM test word 							*/
#define STC3115_INIT     'I'			/* Gas gauge Init states 							*/
#define STC3115_RUNNING  'R'			/* Gas gauge Running states 						*/
#define STC3115_POWERDN  'D'			/* Gas gauge Stop states 							*/
  
/**
  * @}
  */
 
/** @defgroup STC3115_Exported_Types
  * @{
  */
   
   /*stc3115 configuration structure --------------------------------------------- */
 typedef struct  {
  int Vmode;       		/* 1=Voltage mode, 0=mixed mode 						*/
  int Alm_SOC;     		/* SOC alarm level in %									*/
  int Alm_Vbat;    		/* Vbat alarm level in mV								*/
  int CC_cnf;      		/* nominal battery CC_cnf 								*/
  int VM_cnf;      		/* nominal battery VM cnf 								*/
  int Cnom;        		/* nominal battery capacity in mAh 						*/
  int Rsense;      		/* sense resistor in mOhms								*/
  int RelaxCurrent; 	/* relaxation current(< C/20) in mA						*/
  unsigned char OCVOffset[16];    /* OCV curve adjustment in 0.55mV				*/
} STC3115_ConfigData_TypeDef;

/*battery output structure ---------------------------------------------------- */
typedef struct  {
  int status;			/* STC3115 status registers 							*/
  int HRSOC;			/* battery relative SOC (%) in 1/512% 					*/
  int SOC;            	/* battery relative SOC (%) in 0.1% 					*/
  int Voltage;        	/* battery voltage in mV 								*/
  int Current;        	/* battery current in mA 								*/
  int Temperature;    	/* battery temperature in 0.1°C 						*/
  int ConvCounter;		/* STC3115 convertion counter in 0.5s					*/
  int OCV;				/* battery relax voltage in mV 							*/
  int Presence;			/* battery presence										*/
  int ChargeValue;    	/* battery remaining capacity in mAh 					*/
  int RemTime;        	/* battery remaining operating time during discharge 	*/
 } STC3115_BatteryData_TypeDef;
 
/* stc3115 RAM registers structure -------------------------------------------- */
typedef union {
  unsigned char db[RAM_SIZE];  /* last byte holds the CRC 						*/
  struct {
    short int TstWord;     /* 0-1 												*/
    short int HRSOC;       /* 2-3 SOC backup in (1/512%)						*/
    short int CC_cnf;      /* 4-5 current CC_cnf 								*/
    short int VM_cnf;      /* 6-7 current VM_cnf 								*/
    char SOC;              /* 8 SOC (in %) 										*/
    char STC3115_Status;   /* 9  STC3115 working state							*/
    /* bytes ..RAM_SIZE-2 are free, last byte RAM_SIZE-1 is the CRC 			*/
  } reg;
} RAMData_TypeDef;


typedef struct  {
  STC3115_ConfigData_TypeDef configData;
  STC3115_BatteryData_TypeDef batteryData;
  RAMData_TypeDef ramData;
} STC3115_Data_t;

/**
  * @}
  */ 

/** @defgroup STC3115_Exported_Variables
  * @{
  */
/* LCD driver structure */
extern GG_Drv_t STC3115_Drv;

         
/**
  * @}
  */

/** @defgroup STC3115_Exported_Functions
  * @{
  */ 
/* GG IO functions */
extern uint32_t GG_IO_GetITState(void);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STC3115_DRIVER_H__ */

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
