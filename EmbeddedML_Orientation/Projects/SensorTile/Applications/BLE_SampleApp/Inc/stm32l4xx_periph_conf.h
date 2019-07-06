/**
  ******************************************************************************
  * @file    stm32l4xx_periph_conf.h
  * @author  Central LAB
  * @version V1.0.0
  * @date    21-Nov-2016
  * @brief   peripherals configuration file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
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
#ifndef __STM32L4xx_PERIPH_CONF_H
#define __STM32L4xx_PERIPH_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32l4xx_hal.h"

/* Exported constants --------------------------------------------------------*/

/* USART2 */
#define USART2_CMN_DEFAULT_BAUDRATE    460800
#define USART2_CMN_DEFAULT_WORLDLENGTH UART_WORDLENGTH_8B
#define USART2_CMN_DEFAULT_STOPBITS    UART_STOPBITS_1
#define USART2_CMN_DEFAULT_PARITY      UART_PARITY_NONE
#define USART2_CMN_DEFAULT_HWFLOWCTL   UART_HWCONTROL_NONE
#define USART2_CMN_DEFAULT_MODE        UART_MODE_TX_RX

#define USART2_CMN_DEFAULT_CLK_ENABLE()         __USART2_CLK_ENABLE();
#define USART2_CMN_DEFAULT_RX_GPIO_CLK_ENABLE() __GPIOA_CLK_ENABLE()
#define USART2_CMN_DEFAULT_TX_GPIO_CLK_ENABLE() __GPIOA_CLK_ENABLE() 

#define USART2_CMN_DEFAULT_FORCE_RESET()        __USART2_FORCE_RESET()
#define USART2_CMN_DEFAULT_RELEASE_RESET()      __USART2_RELEASE_RESET()

#define USART2_CMN_DEFAULT_TX_PIN       GPIO_PIN_2
#define USART2_CMN_DEFAULT_TX_GPIO_PORT GPIOA  
#define USART2_CMN_DEFAULT_TX_AF        GPIO_AF7_USART2
#define USART2_CMN_DEFAULT_RX_PIN       GPIO_PIN_3
#define USART2_CMN_DEFAULT_RX_GPIO_PORT GPIOA 
#define USART2_CMN_DEFAULT_RX_AF        GPIO_AF7_USART2

/* I2C1 */
//#define I2C1_CMN_DEFAULT_TIMING    0x0020098E /* to reach 100Khz speed (Rise time = 50ns, Fall time = 10ns) */
#define I2C1_CMN_DEFAULT_TIMING  0x00B0298B /* I2C 400khz for Clock 80Mhz, Fast Mode, Analog Filter ON, Rise time 50ns, Fall time 25ns */
#define I2C1_CMN_DEFAULT_OWNADDRESS1     0x33
#define I2C1_CMN_DEFAULT_OWNADDRESS2     0
#define I2C1_CMN_DEFAULT_ADDRESSINGMODE  I2C_ADDRESSINGMODE_7BIT
#define I2C1_CMN_DEFAULT_DUALADDRESSMODE I2C_DUALADDRESS_DISABLED
#define I2C1_CMN_DEFAULT_GENERALCALLMODE I2C_GENERALCALL_DISABLED
#define I2C1_CMN_DEFAULT_NOSTRETCHMODE   I2C_NOSTRETCH_DISABLED

#define I2C1_CMN_DEFAULT_CLK_ENABLE()               __I2C1_CLK_ENABLE()
#define I2C1_CMN_DEFAULT_SCL_SDA_GPIO_CLK_ENABLE()  __GPIOB_CLK_ENABLE()
#define I2C1_CMN_DEFAULT_SCL_SDA_AF                 GPIO_AF4_I2C1
#define I2C1_CMN_DEFAULT_SCL_SDA_GPIO_PORT          GPIOB
#define I2C1_CMN_DEFAULT_SCL_PIN                    GPIO_PIN_8
#define I2C1_CMN_DEFAULT_SDA_PIN                    GPIO_PIN_9

#define I2C1_CMN_DEFAULT_FORCE_RESET()              __I2C1_FORCE_RESET()
#define I2C1_CMN_DEFAULT_RELEASE_RESET()            __I2C1_RELEASE_RESET()

/* I2C interrupt requests */
#define I2C1_CMN_DEFAULT_EV_IRQn                    I2C1_EV_IRQn
#define I2C1_CMN_DEFAULT_ER_IRQn                    I2C1_ER_IRQn

/* SPI1 */
#define SPI1_CMN_DEFAULT_MODE               SPI_MODE_MASTER
#define SPI1_CMN_DEFAULT_DIRECTON           SPI_DIRECTION_2LINES
#define SPI1_CMN_DEFAULT_DATASIZE           SPI_DATASIZE_8BIT
#define SPI1_CMN_DEFAULT_CLKPOLARITY        SPI_POLARITY_LOW
#define SPI1_CMN_DEFAULT_CLKPHASE           SPI_PHASE_1EDGE
#define SPI1_CMN_DEFAULT_NSS                SPI_NSS_SOFT
#define SPI1_CMN_DEFAULT_FIRSTBIT           SPI_FIRSTBIT_MSB
#define SPI1_CMN_DEFAULT_TIMODE             SPI_TIMODE_DISABLED
#define SPI1_CMN_DEFAULT_CRCPOLYNOMIAL      7
#define SPI1_CMN_DEFAULT_BAUNDRATEPRESCALER SPI_BAUDRATEPRESCALER_4
#define SPI1_CMN_DEFAULT_CRCCALCULATION     SPI_CRCCALCULATION_DISABLED

#define SPI1_CMN_DEFAULT_CLK_ENABLE()       __SPI1_CLK_ENABLE()

// SPI Reset Pin: PA.8
#define SPI1_CMN_DEFAULT_RESET_PIN          GPIO_PIN_8
#define SPI1_CMN_DEFAULT_RESET_MODE         GPIO_MODE_OUTPUT_PP
#define SPI1_CMN_DEFAULT_RESET_PULL         GPIO_PULLUP
#define SPI1_CMN_DEFAULT_RESET_SPEED        GPIO_SPEED_LOW
#define SPI1_CMN_DEFAULT_RESET_ALTERNATE    0
#define SPI1_CMN_DEFAULT_RESET_PORT         GPIOA
#define SPI1_CMN_DEFAULT_RESET_CLK_ENABLE() __GPIOA_CLK_ENABLE()

// SCLK: PB.3
#define SPI1_CMN_DEFAULT_SCLK_PIN           GPIO_PIN_3
#define SPI1_CMN_DEFAULT_SCLK_MODE          GPIO_MODE_AF_PP
#define SPI1_CMN_DEFAULT_SCLK_PULL          GPIO_PULLDOWN
#define SPI1_CMN_DEFAULT_SCLK_SPEED         GPIO_SPEED_HIGH
#define SPI1_CMN_DEFAULT_SCLK_ALTERNATE     GPIO_AF5_SPI1
#define SPI1_CMN_DEFAULT_SCLK_PORT          GPIOB
#define SPI1_CMN_DEFAULT_SCLK_CLK_ENABLE()  __GPIOB_CLK_ENABLE()

// MISO (Master Input Slave Output): PA.6
#define SPI1_CMN_DEFAULT_MISO_PIN           GPIO_PIN_6
#define SPI1_CMN_DEFAULT_MISO_MODE          GPIO_MODE_AF_PP
#define SPI1_CMN_DEFAULT_MISO_PULL          GPIO_NOPULL
#define SPI1_CMN_DEFAULT_MISO_SPEED         GPIO_SPEED_HIGH
#define SPI1_CMN_DEFAULT_MISO_ALTERNATE     GPIO_AF5_SPI1
#define SPI1_CMN_DEFAULT_MISO_PORT          GPIOA
#define SPI1_CMN_DEFAULT_MISO_CLK_ENABLE()  __GPIOA_CLK_ENABLE()

// MOSI (Master Output Slave Input): PA.7
#define SPI1_CMN_DEFAULT_MOSI_PIN           GPIO_PIN_7
#define SPI1_CMN_DEFAULT_MOSI_MODE          GPIO_MODE_AF_PP
#define SPI1_CMN_DEFAULT_MOSI_PULL          GPIO_NOPULL
#define SPI1_CMN_DEFAULT_MOSI_SPEED         GPIO_SPEED_HIGH
#define SPI1_CMN_DEFAULT_MOSI_ALTERNATE     GPIO_AF5_SPI1
#define SPI1_CMN_DEFAULT_MOSI_PORT          GPIOA
#define SPI1_CMN_DEFAULT_MOSI_CLK_ENABLE()  __GPIOA_CLK_ENABLE()

// NSS/CSN/CS: PA.1
#define SPI1_CMN_DEFAULT_CS_PIN             GPIO_PIN_1
#define SPI1_CMN_DEFAULT_CS_MODE            GPIO_MODE_OUTPUT_PP
#define SPI1_CMN_DEFAULT_CS_PULL            GPIO_PULLUP
#define SPI1_CMN_DEFAULT_CS_SPEED           GPIO_SPEED_HIGH
#define SPI1_CMN_DEFAULT_CS_ALTERNATE       0
#define SPI1_CMN_DEFAULT_CS_PORT            GPIOA
#define SPI1_CMN_DEFAULT_CS_CLK_ENABLE()    __GPIOA_CLK_ENABLE()

// IRQ: PA.0
#define SPI1_CMN_DEFAULT_IRQ_PIN            GPIO_PIN_0
#define SPI1_CMN_DEFAULT_IRQ_MODE           GPIO_MODE_IT_RISING
#define SPI1_CMN_DEFAULT_IRQ_PULL           GPIO_NOPULL
#define SPI1_CMN_DEFAULT_IRQ_SPEED          GPIO_SPEED_HIGH
#define SPI1_CMN_DEFAULT_IRQ_ALTERNATE      0
#define SPI1_CMN_DEFAULT_IRQ_PORT           GPIOA
#define SPI1_CMN_DEFAULT_IRQ_CLK_ENABLE()   __GPIOA_CLK_ENABLE()
   
#define SPI1_CMN_DEFAULT_EXTI_IRQn          EXTI0_IRQn
#define SPI1_CMN_DEFAULT_EXTI_IRQHandler    EXTI0_IRQHandler
#define SPI1_CMN_DEFAULT_EXTI_PIN           SPI1_CMN_DEFAULT_IRQ_PIN
#define SPI1_CMN_DEFAULT_EXTI_PORT          SPI1_CMN_DEFAULT_IRQ_PORT


#ifdef __cplusplus
}
#endif

#endif /* __STM32L4xx_PERIPH_CONF_H */


/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
