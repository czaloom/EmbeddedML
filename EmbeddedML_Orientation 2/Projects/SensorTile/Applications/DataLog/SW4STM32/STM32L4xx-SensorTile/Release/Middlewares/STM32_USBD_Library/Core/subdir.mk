################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c \
/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c \
/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c 

OBJS += \
./Middlewares/STM32_USBD_Library/Core/usbd_core.o \
./Middlewares/STM32_USBD_Library/Core/usbd_ctlreq.o \
./Middlewares/STM32_USBD_Library/Core/usbd_ioreq.o 

C_DEPS += \
./Middlewares/STM32_USBD_Library/Core/usbd_core.d \
./Middlewares/STM32_USBD_Library/Core/usbd_ctlreq.d \
./Middlewares/STM32_USBD_Library/Core/usbd_ioreq.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/STM32_USBD_Library/Core/usbd_core.o: /Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DOSX_BMS_SENSORTILE -DSTM32L476xx -DUSE_STM32L4XX_NUCLEO -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Projects/SensorTile/Applications/DataLog/Inc" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/STM32L4xx_HAL_Driver/Inc" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/CMSIS/Include" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/BSP/Components/Common" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/BSP/Components/hts221" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/BSP/Components/lsm6dsm" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/BSP/Components/lps22hb" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/BSP/Components/lsm303agr" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/BSP/Components/stc3115" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/BSP/SensorTile" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Middlewares/Third_Party/FatFs/src" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Middlewares/Third_Party/FatFs/src/drivers" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I/Users/wkaiser/Documents/Primary/Courses/ST_Microelectronics_IoT/Development/SensorTile_Inertial_Sensing/Reference_System_2-21-2019/SensorTile_Inertial_Sensing -I/Users/wkaiser/Documents/Primary/Courses/Embedded_ML/Embedded_ML_Demonstation_Project_5_31_2019 -I/Users/wkaiser/Documents/Primary/Courses/Embedded_ML/Embedded_ML_Demonstation_Project_5_31_2019/EmbeddedML_SensorTile_Introduction/Projects/SensorTile/Applications/DataLog/Src -I/Users/wkaiser/Documents/Primary/Courses/Embedded_ML/Embedded_ML_Development_XOR_AND/Embedded_ML_XOR-AND_V2  -O0 -g1 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Middlewares/STM32_USBD_Library/Core/usbd_ctlreq.o: /Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DOSX_BMS_SENSORTILE -DSTM32L476xx -DUSE_STM32L4XX_NUCLEO -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Projects/SensorTile/Applications/DataLog/Inc" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/STM32L4xx_HAL_Driver/Inc" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/CMSIS/Include" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/BSP/Components/Common" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/BSP/Components/hts221" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/BSP/Components/lsm6dsm" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/BSP/Components/lps22hb" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/BSP/Components/lsm303agr" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/BSP/Components/stc3115" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/BSP/SensorTile" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Middlewares/Third_Party/FatFs/src" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Middlewares/Third_Party/FatFs/src/drivers" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I/Users/wkaiser/Documents/Primary/Courses/ST_Microelectronics_IoT/Development/SensorTile_Inertial_Sensing/Reference_System_2-21-2019/SensorTile_Inertial_Sensing -I/Users/wkaiser/Documents/Primary/Courses/Embedded_ML/Embedded_ML_Demonstation_Project_5_31_2019 -I/Users/wkaiser/Documents/Primary/Courses/Embedded_ML/Embedded_ML_Demonstation_Project_5_31_2019/EmbeddedML_SensorTile_Introduction/Projects/SensorTile/Applications/DataLog/Src -I/Users/wkaiser/Documents/Primary/Courses/Embedded_ML/Embedded_ML_Development_XOR_AND/Embedded_ML_XOR-AND_V2  -O0 -g1 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Middlewares/STM32_USBD_Library/Core/usbd_ioreq.o: /Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DOSX_BMS_SENSORTILE -DSTM32L476xx -DUSE_STM32L4XX_NUCLEO -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Projects/SensorTile/Applications/DataLog/Inc" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/STM32L4xx_HAL_Driver/Inc" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/CMSIS/Include" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/BSP/Components/Common" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/BSP/Components/hts221" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/BSP/Components/lsm6dsm" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/BSP/Components/lps22hb" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/BSP/Components/lsm303agr" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/BSP/Components/stc3115" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Drivers/BSP/SensorTile" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Middlewares/Third_Party/FatFs/src" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Middlewares/Third_Party/FatFs/src/drivers" -I"/Users/charleszaloom/Downloads/EmbeddedML_SensorTile_Introduction/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I/Users/wkaiser/Documents/Primary/Courses/ST_Microelectronics_IoT/Development/SensorTile_Inertial_Sensing/Reference_System_2-21-2019/SensorTile_Inertial_Sensing -I/Users/wkaiser/Documents/Primary/Courses/Embedded_ML/Embedded_ML_Demonstation_Project_5_31_2019 -I/Users/wkaiser/Documents/Primary/Courses/Embedded_ML/Embedded_ML_Demonstation_Project_5_31_2019/EmbeddedML_SensorTile_Introduction/Projects/SensorTile/Applications/DataLog/Src -I/Users/wkaiser/Documents/Primary/Courses/Embedded_ML/Embedded_ML_Development_XOR_AND/Embedded_ML_XOR-AND_V2  -O0 -g1 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


