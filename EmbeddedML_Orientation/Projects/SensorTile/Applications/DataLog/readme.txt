/**
  @page DataLog application for SensorTile
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    readme.txt  
  * @version V1.1.0
  * @date    27-Sept-2016
  * @brief   This application contains an example which shows how to obtain data
  *          from the various sensors on the SensorTile. The data can either be
  *  	     streamed to the PC via USB or saved on SD Card
  ******************************************************************************
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
  @endverbatim

@par Application Description 

DataLog application has two major features: sensor raw data streaming via USB
(Virtual COM Port class) and sensor data storage on SD card.
If USB is selected, the flowing data can be read using a generic application
like TeraTerm/Putty or any other.
If SD Card is selected, the data stream is logged on a tsv file (Tab Separated Values).
After the application is started:
- the user can view/record data from the on-board environmental sensors like Temperature, 
Humidity (if available) and Pressure.
- the user can view/record data from the on-board MEMS sensors like Accelerometer,
Gyroscope and Magnetometer.
- the user can view/record data from the Gas Gauge IC to monitor the battery status.

@par Hardware and Software environment

  - This application runs on SensorTile platform (STEVAL-STLKT01V1).
  - The USB version works with the SensorTile plugged on the Cradle or the Cradle Expansion, while
    the SD Card version only works with the Cradle (STLCR01V1).
  - STM32 Virtual COM Port Driver for Windows can be downloaded from st.com - STSW-STM32102
    
@par How to use it? 

This package contains projects for 3 IDEs viz. IAR, µVision and System Workbench. In order to make the 
program work, you must do the following:
 - WARNING: before opening the project with any toolchain be sure your folder
   installation path is not too in-depth since the toolchain may report errors
   after building.

For IAR:
 - Open IAR toolchain (this firmware has been successfully tested with
   Embedded Workbench V7.40.3).
 - Open the IAR project file EWARM\Project.eww
 - Rebuild all files and load your image into target memory.
 - Run the example.

For µVision:
 - Open µVision 5 toolchain (this firmware has been successfully tested with MDK-ARM Professional Version: 5.20).
 - Open the µVision project file MDK-ARM\Project.uvprojx
 - Rebuild all files and load your image into target memory.
 - Run the example.

For System Workbench:
 - Open System Workbench for STM32.
 - Set the default workspace proposed by the IDE (please be sure that there are not spaces in the workspace path).
 - Press "File" -> "Import" -> "Existing Projects into Workspace"; press "Browse" in the "Select root directory" and choose the path where the System
   Workbench project is located (SW4STM32)
 - Rebuild all files and load your image into target memory.
 - Run the example.


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
