################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../example/src/cr_startup_lpc82x.c \
../example/src/crp.c \
../example/src/mtb.c \
../example/src/sysinit.c \
../example/src/uart_rb.c 

OBJS += \
./example/src/cr_startup_lpc82x.o \
./example/src/crp.o \
./example/src/mtb.o \
./example/src/sysinit.o \
./example/src/uart_rb.o 

C_DEPS += \
./example/src/cr_startup_lpc82x.d \
./example/src/crp.d \
./example/src/mtb.d \
./example/src/sysinit.d \
./example/src/uart_rb.d 


# Each subdirectory must supply rules for building sources it contributes
example/src/%.o: ../example/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -D__USE_LPCOPEN -D__MTB_BUFFER_SIZE=128 -D__REDLIB__ -DCORE_M0PLUS -I"C:\Users\Joey\Documents\LPCXpresso_8.2.2_650\MATE_Workspace\lpc_chip_82x\inc" -I"C:\Users\Joey\Documents\LPCXpresso_8.2.2_650\MATE_Workspace\lpc_board_nxp_lpcxpresso_824\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


