################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/User/OLED.c \
../Core/Src/User/font.c 

OBJS += \
./Core/Src/User/OLED.o \
./Core/Src/User/font.o 

C_DEPS += \
./Core/Src/User/OLED.d \
./Core/Src/User/font.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/User/%.o Core/Src/User/%.su Core/Src/User/%.cyclo: ../Core/Src/User/%.c Core/Src/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-User

clean-Core-2f-Src-2f-User:
	-$(RM) ./Core/Src/User/OLED.cyclo ./Core/Src/User/OLED.d ./Core/Src/User/OLED.o ./Core/Src/User/OLED.su ./Core/Src/User/font.cyclo ./Core/Src/User/font.d ./Core/Src/User/font.o ./Core/Src/User/font.su

.PHONY: clean-Core-2f-Src-2f-User

