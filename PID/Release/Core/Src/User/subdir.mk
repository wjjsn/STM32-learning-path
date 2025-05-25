################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/User/OLED.c \
../Core/Src/User/font.c \
../Core/Src/User/menu.c \
../Core/Src/User/motor.c \
../Core/Src/User/reDefineIT.c 

OBJS += \
./Core/Src/User/OLED.o \
./Core/Src/User/font.o \
./Core/Src/User/menu.o \
./Core/Src/User/motor.o \
./Core/Src/User/reDefineIT.o 

C_DEPS += \
./Core/Src/User/OLED.d \
./Core/Src/User/font.d \
./Core/Src/User/menu.d \
./Core/Src/User/motor.d \
./Core/Src/User/reDefineIT.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/User/%.o Core/Src/User/%.su Core/Src/User/%.cyclo: ../Core/Src/User/%.c Core/Src/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-User

clean-Core-2f-Src-2f-User:
	-$(RM) ./Core/Src/User/OLED.cyclo ./Core/Src/User/OLED.d ./Core/Src/User/OLED.o ./Core/Src/User/OLED.su ./Core/Src/User/font.cyclo ./Core/Src/User/font.d ./Core/Src/User/font.o ./Core/Src/User/font.su ./Core/Src/User/menu.cyclo ./Core/Src/User/menu.d ./Core/Src/User/menu.o ./Core/Src/User/menu.su ./Core/Src/User/motor.cyclo ./Core/Src/User/motor.d ./Core/Src/User/motor.o ./Core/Src/User/motor.su ./Core/Src/User/reDefineIT.cyclo ./Core/Src/User/reDefineIT.d ./Core/Src/User/reDefineIT.o ./Core/Src/User/reDefineIT.su

.PHONY: clean-Core-2f-Src-2f-User

