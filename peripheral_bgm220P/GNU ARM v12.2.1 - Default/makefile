################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include autogen/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := peripheral_bgm220P
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: peripheral_bgm220P.axf

# Tool invocations
peripheral_bgm220P.axf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -T "D:\Code\OJT_code\BLE-Advance\peripheral_bgm220P\autogen\linkerfile.ld" -Wl,--no-warn-rwx-segments -Xlinker --gc-sections -Xlinker -Map="peripheral_bgm220P.map" -mfpu=fpv5-sp-d16 -mfloat-abi=hard --specs=nano.specs -o peripheral_bgm220P.axf -Wl,--start-group "./app.o" "./device.o" "./main.o" "./sl_gatt_service_device_information.o" "./autogen/gatt_db.o" "./autogen/sl_bluetooth.o" "./autogen/sl_board_default_init.o" "./autogen/sl_device_init_clocks.o" "./autogen/sl_event_handler.o" "./autogen/sl_iostream_handles.o" "./autogen/sl_iostream_init_usart_instances.o" "./autogen/sl_power_manager_handler.o" "D:/Geko_new/gecko-sdk//protocol/bluetooth/bgcommon/lib/libbgcommon_efr32xg22_gcc_release.a" "D:/Geko_new/gecko-sdk//protocol/bluetooth/bgstack/ll/lib/libbluetooth_controller_efr32xg22_gcc_release.a" "D:/Geko_new/gecko-sdk//protocol/bluetooth/lib/libbluetooth_host_efr32xg22_gcc_release.a" "D:/Geko_new/gecko-sdk//platform/emdrv/nvm3/lib/libnvm3_CM33_gcc.a" "D:/Geko_new/gecko-sdk//platform/radio/rail_lib/autogen/librail_release/librail_config_bgm220pc22hna_gcc.a" "D:/Geko_new/gecko-sdk//platform/radio/rail_lib/autogen/librail_release/librail_module_efr32xg22_gcc_release.a" -lgcc -lc -lm -lnosys -Wl,--end-group -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building out file: peripheral_bgm220P.out'
	arm-none-eabi-objcopy "peripheral_bgm220P.axf" "peripheral_bgm220P.out"
	@echo ' '

	@echo 'Building hex file: peripheral_bgm220P.hex'
	arm-none-eabi-objcopy -O ihex "peripheral_bgm220P.axf" "peripheral_bgm220P.hex"
	@echo ' '

	@echo 'Building bin file: peripheral_bgm220P.bin'
	arm-none-eabi-objcopy -O binary "peripheral_bgm220P.axf" "peripheral_bgm220P.bin"
	@echo ' '

	@echo 'Building s37 file: peripheral_bgm220P.s37'
	arm-none-eabi-objcopy -O srec "peripheral_bgm220P.axf" "peripheral_bgm220P.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "peripheral_bgm220P.axf" -A
	@echo ' '

# Other Targets
clean:
	-@$(foreach FILE, $(EXECUTABLES), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(OBJS), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(C_DEPS), $(shell $(RM) $(FILE)) )
	-$(RM) peripheral_bgm220P.axf
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
