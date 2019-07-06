/**
  @page BLE_SampleApp application for SensorTile
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    readme.txt  
  * @version V1.0.0
  * @date    21-Nov-2016
  * @brief   This application contains an example which shows how to configure 
  *          the SensorTile to stream Environmental sensors data via Bluetooth
  *	    	 Low Energy.
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

BLE_SampleApp provides an example of Bluetooth Low Energy configuration that
 enables SensorTile to stream environmental sensors data; it is compatible 
 with ST BlueMS app available for both Android and iOS.

 After reset the firmware performs the following actions:
 - Configure HAL and clocks
 - Configure and disable sensors Chip Select pins
 - Initilize the target platform:
   - USB peripheral (for debugging)
   - LED1
   - Environmental sensors
 - Initialize Bluetooth Low Energy stack
 - Initialize Bluetooth Low Energy services
 - Initialize timers
 - Main Loop:
   - Led management
   - BLE events management
   - Environmental sensors data mangement


@par Hardware and Software environment

  - This application runs on SensorTile platform (STEVAL-STLKT01V1).
  - The USB version works with the SensorTile plugged on the Cradle or the Cradle Expansion.
  - ST BlueMS app (www.st.com/bluems) is available for both Android and iOS.
    
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
