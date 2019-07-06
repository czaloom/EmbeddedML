/**
******************************************************************************
* file    readme.txt
* Version V1.0.0
* date    01-June-2016
******************************************************************************
* Attention
*
* COPYRIGHT(c) 2016 STMicroelectronics
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

Application Description 

This firware is one Boot Loader that must run at the Flash beginning (0x08000000 address) and has the purpouse to apply the Firmware-Over-The-Air (FOTA) updated receveid, already checked and stored in Flash, or run the program normally if there is not one update.

For STMF401RE-Nucleo the 512Kbyte of Flash are splitted on 8 sectors:

|sector 0   | sector 1  | sector 2  | sector 3  | sector 4  | sector 5  | sector 6  | sector 7  |
|16K        | 16K       | 16K       | 16K       | 64K       | 128K      | 128K      | 128K      |
|0x08000000 |0x08004000 |0x08008000 |0x0800C000 |0x08010000 |0x08020000 |0x08040000 |0x08060000 |
|-----------------------------------------------------------------------------------------------|
|BootLoader | Running Program                                           |  FOTA                 |


This Boot Loader must be Loaded on Sector 0 and it checks if there is one FOTA stored on Sector 6 (0x08040000 address):
- If YES:
  - it erases the sectors from 1 to 5
  - it copies the FOTA on that sectors
  - it erases the FOTA region after the update.
- if NOT:
  - it works like a trampoline for executing the normal program stored from sector1 (0x08004000 address)

The FOTA must be less than 240Kbytes



For STM32L476RG (Nucleo or SensorTile [STEVAL-STLCS01V1]) the 1Mbytes of Flash is splitted on 2 banks of 51Kbytes each one, and each bank is splitted on 256 pages of 2Kbytes.

For analogy with the STM32F401RE we use only the first bank of Flash and we save the BootLoader, Running Program and the FOTA on the same addresses used for STM32F401RE

|Page 0->7  | Page 8->127       | Page 128->255 | Page 256->511 |
|16K        | 240K              | 256K          | 512K          |
|0x08000000 |0x08004000         |0x08040000     |0x0808000      |
|---------------------------------------------------------------|
|BootLoader | Running Program   |  FOTA         |  Free2Use     |


The "Running Program" and the FOTA are compiled for running from 0x08004000 address. If they are placed at the beginning of the FLASH they doesn't work.
And without the BootLoader that programs could not be executed.

 /******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/