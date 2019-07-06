/**
  @page AudioLoop application for SensorTile
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    readme.txt  
  * @version V1.0.1
  * @date    27-Sept-2016
  * @brief   This application application sends audio signals acquired by the 
  *          microphone to an onboard DAC via I2S interface
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

AudioLoop application sends audio signals acquired by the microphone to an onboard DAC via I2S interface.
In this way the user can conveniently play the recorded sound on loudspeakers or headphones.
The purpose of this example is to explain how to configure the DFSDM interface for the microphone acquisition
and the SAI (Serial Audio Interface) for the audio output.

@par Hardware and Software environment

  - This application runs on SensorTile platform (STEVAL-STLCS01V1) plugged in the Cradle Expansion board (STLCX01V1).
      
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
