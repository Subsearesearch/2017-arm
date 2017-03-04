################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include example/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: periph_pll_tab.axf

# Tool invocations
periph_pll_tab.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\Joey\Documents\LPCXpresso_8.2.2_650\MATE_Workspace\lpc_chip_82x\Debug" -L"C:\Users\Joey\Documents\LPCXpresso_8.2.2_650\MATE_Workspace\lpc_board_nxp_lpcxpresso_824\Debug" -Xlinker -Map="periph_pll_tab.map" -Xlinker --gc-sections -mcpu=cortex-m0 -mthumb -T "periph_pll_tab_Debug.ld" -o "periph_pll_tab.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) periph_pll_tab.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "periph_pll_tab.axf"; # arm-none-eabi-objcopy -v -O binary "periph_pll_tab.axf" "periph_pll_tab.bin" ; # checksum -p LPC824 -d "periph_pll_tab.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
