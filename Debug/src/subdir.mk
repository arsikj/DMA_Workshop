################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cr_startup_lpc175x_6x.c \
../src/crp.c \
../src/iap_driver.c \
../src/main.c \
../src/md5.c \
../src/payload_generator.c 

OBJS += \
./src/cr_startup_lpc175x_6x.o \
./src/crp.o \
./src/iap_driver.o \
./src/main.o \
./src/md5.o \
./src/payload_generator.o 

C_DEPS += \
./src/cr_startup_lpc175x_6x.d \
./src/crp.d \
./src/iap_driver.d \
./src/main.d \
./src/md5.d \
./src/payload_generator.d 


# Each subdirectory must supply rules for building sources it contributes
src/cr_startup_lpc175x_6x.o: ../src/cr_startup_lpc175x_6x.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC17xx -D__LPC17XX__ -I"C:\Users\Dejan\Desktop\FINKI\lpc\DMA_Workshop\inc" -I"C:\Users\Dejan\Desktop\FINKI\lpc\CMSIS_CORE_LPC17xx\inc" -Os -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/cr_startup_lpc175x_6x.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC17xx -D__LPC17XX__ -I"C:\Users\Dejan\Desktop\FINKI\lpc\DMA_Workshop\inc" -I"C:\Users\Dejan\Desktop\FINKI\lpc\CMSIS_CORE_LPC17xx\inc" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


