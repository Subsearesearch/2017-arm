################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../src/aeabi_romdiv_patch.s 

C_SRCS += \
../src/ROV_Controller_Prototype.c \
../src/SCT.c \
../src/cr_startup_lpc82x.c \
../src/crp.c \
../src/mtb.c \
../src/sysinit.c 

OBJS += \
./src/ROV_Controller_Prototype.o \
./src/SCT.o \
./src/aeabi_romdiv_patch.o \
./src/cr_startup_lpc82x.o \
./src/crp.o \
./src/mtb.o \
./src/sysinit.o 

C_DEPS += \
./src/ROV_Controller_Prototype.d \
./src/SCT.d \
./src/cr_startup_lpc82x.d \
./src/crp.d \
./src/mtb.d \
./src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M0PLUS -D__MTB_BUFFER_SIZE=256 -D__USE_ROMDIVIDE -D__USE_LPCOPEN -D__LPC82X__ -D__REDLIB__ -I"C:\Users\Joey\Documents\LPCXpresso_8.2.2_650\MATE_Workspace\lpc_board_nxp_lpcxpresso_824\inc" -I"C:\Users\Joey\Documents\LPCXpresso_8.2.2_650\MATE_Workspace\lpc_chip_82x\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU Assembler'
	arm-none-eabi-gcc -c -x assembler-with-cpp -DDEBUG -D__CODE_RED -DCORE_M0PLUS -D__USE_ROMDIVIDE -D__USE_LPCOPEN -D__LPC82X__ -D__REDLIB__ -I"C:\Users\Joey\Documents\LPCXpresso_8.2.2_650\MATE_Workspace\lpc_board_nxp_lpcxpresso_824\inc" -I"C:\Users\Joey\Documents\LPCXpresso_8.2.2_650\MATE_Workspace\lpc_chip_82x\inc" -g3 -mcpu=cortex-m0 -mthumb -specs=redlib.specs -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


