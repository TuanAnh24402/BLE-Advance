################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app.c \
../main.c \
../sl_gatt_service_device_information.c 

OBJS += \
./app.o \
./main.o \
./sl_gatt_service_device_information.o 

C_DEPS += \
./app.d \
./main.d \
./sl_gatt_service_device_information.d 


# Each subdirectory must supply rules for building sources it contributes
app.o: ../app.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -std=c99 '-DBGM220PC22HNA=1' '-DSL_APP_PROPERTIES=1' '-DBOOTLOADER_APPLOADER=1' '-DSL_BOARD_NAME="BRD4311A"' '-DSL_BOARD_REV="A01"' '-DHARDWARE_BOARD_DEFAULT_RF_BAND_2400=1' '-DHARDWARE_BOARD_SUPPORTS_1_RF_BAND=1' '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_2400=1' '-DSL_COMPONENT_CATALOG_PRESENT=1' '-DMBEDTLS_CONFIG_FILE=<sl_mbedtls_config.h>' '-DMBEDTLS_PSA_CRYPTO_CONFIG_FILE=<psa_crypto_config.h>' '-DSL_RAIL_LIB_MULTIPROTOCOL_SUPPORT=0' '-DSL_RAIL_UTIL_PA_CONFIG_HEADER=<sl_rail_util_pa_config.h>' '-DSLI_RADIOAES_REQUIRES_MASKING=1' -I"D:\Code\OJT_code\BLE-Advance\central_bgm_220P\config" -I"D:\Code\OJT_code\BLE-Advance\central_bgm_220P\config\btconf" -I"D:\Code\OJT_code\BLE-Advance\central_bgm_220P\autogen" -I"D:\Code\OJT_code\BLE-Advance\central_bgm_220P" -I"D:/Geko_new/gecko-sdk//platform/Device/SiliconLabs/BGM22/Include" -I"D:/Geko_new/gecko-sdk//app/common/util/app_assert" -I"D:/Geko_new/gecko-sdk//app/common/util/app_log" -I"D:/Geko_new/gecko-sdk//app/common/util/app_timer" -I"D:/Geko_new/gecko-sdk//protocol/bluetooth/inc" -I"D:/Geko_new/gecko-sdk//platform/common/inc" -I"D:/Geko_new/gecko-sdk//protocol/bluetooth/bgcommon/inc" -I"D:/Geko_new/gecko-sdk//protocol/bluetooth/bgstack/ll/inc" -I"D:/Geko_new/gecko-sdk//hardware/board/inc" -I"D:/Geko_new/gecko-sdk//platform/bootloader" -I"D:/Geko_new/gecko-sdk//platform/bootloader/api" -I"D:/Geko_new/gecko-sdk//platform/CMSIS/Core/Include" -I"D:/Geko_new/gecko-sdk//hardware/driver/configuration_over_swo/inc" -I"D:/Geko_new/gecko-sdk//platform/security/sl_component/sl_cryptoacc_library/include" -I"D:/Geko_new/gecko-sdk//platform/security/sl_component/sl_cryptoacc_library/src" -I"D:/Geko_new/gecko-sdk//platform/driver/debug/inc" -I"D:/Geko_new/gecko-sdk//platform/service/device_init/inc" -I"D:/Geko_new/gecko-sdk//platform/emdrv/dmadrv/inc" -I"D:/Geko_new/gecko-sdk//platform/emdrv/common/inc" -I"D:/Geko_new/gecko-sdk//platform/emlib/inc" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/plugin/fem_util" -I"D:/Geko_new/gecko-sdk//app/bluetooth/common/gatt_service_device_information" -I"D:/Geko_new/gecko-sdk//app/bluetooth/common/in_place_ota_dfu" -I"D:/Geko_new/gecko-sdk//platform/service/iostream/inc" -I"D:/Geko_new/gecko-sdk//platform/security/sl_component/sl_mbedtls_support/config" -I"D:/Geko_new/gecko-sdk//platform/security/sl_component/sl_mbedtls_support/config/preset" -I"D:/Geko_new/gecko-sdk//platform/security/sl_component/sl_mbedtls_support/inc" -I"D:/Geko_new/gecko-sdk//util/third_party/mbedtls/include" -I"D:/Geko_new/gecko-sdk//util/third_party/mbedtls/library" -I"D:/Geko_new/gecko-sdk//platform/service/mpu/inc" -I"D:/Geko_new/gecko-sdk//hardware/driver/mx25_flash_shutdown/inc/sl_mx25_flash_shutdown_usart" -I"D:/Geko_new/gecko-sdk//platform/emdrv/nvm3/inc" -I"D:/Geko_new/gecko-sdk//platform/service/power_manager/inc" -I"D:/Geko_new/gecko-sdk//platform/security/sl_component/sl_psa_driver/inc" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/common" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/protocol/ble" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/protocol/ieee802154" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/protocol/wmbus" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/protocol/zwave" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/chip/efr32/efr32xg2x" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/protocol/sidewalk" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/plugin/pa-conversions" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/plugin/pa-conversions/efr32xg22" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/plugin/rail_util_power_manager_init" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/plugin/rail_util_pti" -I"D:/Geko_new/gecko-sdk//platform/security/sl_component/se_manager/inc" -I"D:/Geko_new/gecko-sdk//platform/security/sl_component/se_manager/src" -I"D:/Geko_new/gecko-sdk//util/silicon_labs/silabs_core/memory_manager" -I"D:/Geko_new/gecko-sdk//platform/common/toolchain/inc" -I"D:/Geko_new/gecko-sdk//platform/service/system/inc" -I"D:/Geko_new/gecko-sdk//platform/service/sleeptimer/inc" -I"D:/Geko_new/gecko-sdk//platform/security/sl_component/sl_protocol_crypto/src" -I"D:/Geko_new/gecko-sdk//platform/service/udelay/inc" -Os -Wall -Wextra -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mcmse --specs=nano.specs -c -fmessage-length=0 -MMD -MP -MF"app.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.o: ../main.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -std=c99 '-DBGM220PC22HNA=1' '-DSL_APP_PROPERTIES=1' '-DBOOTLOADER_APPLOADER=1' '-DSL_BOARD_NAME="BRD4311A"' '-DSL_BOARD_REV="A01"' '-DHARDWARE_BOARD_DEFAULT_RF_BAND_2400=1' '-DHARDWARE_BOARD_SUPPORTS_1_RF_BAND=1' '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_2400=1' '-DSL_COMPONENT_CATALOG_PRESENT=1' '-DMBEDTLS_CONFIG_FILE=<sl_mbedtls_config.h>' '-DMBEDTLS_PSA_CRYPTO_CONFIG_FILE=<psa_crypto_config.h>' '-DSL_RAIL_LIB_MULTIPROTOCOL_SUPPORT=0' '-DSL_RAIL_UTIL_PA_CONFIG_HEADER=<sl_rail_util_pa_config.h>' '-DSLI_RADIOAES_REQUIRES_MASKING=1' -I"D:\Code\OJT_code\BLE-Advance\central_bgm_220P\config" -I"D:\Code\OJT_code\BLE-Advance\central_bgm_220P\config\btconf" -I"D:\Code\OJT_code\BLE-Advance\central_bgm_220P\autogen" -I"D:\Code\OJT_code\BLE-Advance\central_bgm_220P" -I"D:/Geko_new/gecko-sdk//platform/Device/SiliconLabs/BGM22/Include" -I"D:/Geko_new/gecko-sdk//app/common/util/app_assert" -I"D:/Geko_new/gecko-sdk//app/common/util/app_log" -I"D:/Geko_new/gecko-sdk//app/common/util/app_timer" -I"D:/Geko_new/gecko-sdk//protocol/bluetooth/inc" -I"D:/Geko_new/gecko-sdk//platform/common/inc" -I"D:/Geko_new/gecko-sdk//protocol/bluetooth/bgcommon/inc" -I"D:/Geko_new/gecko-sdk//protocol/bluetooth/bgstack/ll/inc" -I"D:/Geko_new/gecko-sdk//hardware/board/inc" -I"D:/Geko_new/gecko-sdk//platform/bootloader" -I"D:/Geko_new/gecko-sdk//platform/bootloader/api" -I"D:/Geko_new/gecko-sdk//platform/CMSIS/Core/Include" -I"D:/Geko_new/gecko-sdk//hardware/driver/configuration_over_swo/inc" -I"D:/Geko_new/gecko-sdk//platform/security/sl_component/sl_cryptoacc_library/include" -I"D:/Geko_new/gecko-sdk//platform/security/sl_component/sl_cryptoacc_library/src" -I"D:/Geko_new/gecko-sdk//platform/driver/debug/inc" -I"D:/Geko_new/gecko-sdk//platform/service/device_init/inc" -I"D:/Geko_new/gecko-sdk//platform/emdrv/dmadrv/inc" -I"D:/Geko_new/gecko-sdk//platform/emdrv/common/inc" -I"D:/Geko_new/gecko-sdk//platform/emlib/inc" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/plugin/fem_util" -I"D:/Geko_new/gecko-sdk//app/bluetooth/common/gatt_service_device_information" -I"D:/Geko_new/gecko-sdk//app/bluetooth/common/in_place_ota_dfu" -I"D:/Geko_new/gecko-sdk//platform/service/iostream/inc" -I"D:/Geko_new/gecko-sdk//platform/security/sl_component/sl_mbedtls_support/config" -I"D:/Geko_new/gecko-sdk//platform/security/sl_component/sl_mbedtls_support/config/preset" -I"D:/Geko_new/gecko-sdk//platform/security/sl_component/sl_mbedtls_support/inc" -I"D:/Geko_new/gecko-sdk//util/third_party/mbedtls/include" -I"D:/Geko_new/gecko-sdk//util/third_party/mbedtls/library" -I"D:/Geko_new/gecko-sdk//platform/service/mpu/inc" -I"D:/Geko_new/gecko-sdk//hardware/driver/mx25_flash_shutdown/inc/sl_mx25_flash_shutdown_usart" -I"D:/Geko_new/gecko-sdk//platform/emdrv/nvm3/inc" -I"D:/Geko_new/gecko-sdk//platform/service/power_manager/inc" -I"D:/Geko_new/gecko-sdk//platform/security/sl_component/sl_psa_driver/inc" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/common" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/protocol/ble" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/protocol/ieee802154" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/protocol/wmbus" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/protocol/zwave" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/chip/efr32/efr32xg2x" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/protocol/sidewalk" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/plugin/pa-conversions" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/plugin/pa-conversions/efr32xg22" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/plugin/rail_util_power_manager_init" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/plugin/rail_util_pti" -I"D:/Geko_new/gecko-sdk//platform/security/sl_component/se_manager/inc" -I"D:/Geko_new/gecko-sdk//platform/security/sl_component/se_manager/src" -I"D:/Geko_new/gecko-sdk//util/silicon_labs/silabs_core/memory_manager" -I"D:/Geko_new/gecko-sdk//platform/common/toolchain/inc" -I"D:/Geko_new/gecko-sdk//platform/service/system/inc" -I"D:/Geko_new/gecko-sdk//platform/service/sleeptimer/inc" -I"D:/Geko_new/gecko-sdk//platform/security/sl_component/sl_protocol_crypto/src" -I"D:/Geko_new/gecko-sdk//platform/service/udelay/inc" -Os -Wall -Wextra -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mcmse --specs=nano.specs -c -fmessage-length=0 -MMD -MP -MF"main.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

sl_gatt_service_device_information.o: ../sl_gatt_service_device_information.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -std=c99 '-DBGM220PC22HNA=1' '-DSL_APP_PROPERTIES=1' '-DBOOTLOADER_APPLOADER=1' '-DSL_BOARD_NAME="BRD4311A"' '-DSL_BOARD_REV="A01"' '-DHARDWARE_BOARD_DEFAULT_RF_BAND_2400=1' '-DHARDWARE_BOARD_SUPPORTS_1_RF_BAND=1' '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_2400=1' '-DSL_COMPONENT_CATALOG_PRESENT=1' '-DMBEDTLS_CONFIG_FILE=<sl_mbedtls_config.h>' '-DMBEDTLS_PSA_CRYPTO_CONFIG_FILE=<psa_crypto_config.h>' '-DSL_RAIL_LIB_MULTIPROTOCOL_SUPPORT=0' '-DSL_RAIL_UTIL_PA_CONFIG_HEADER=<sl_rail_util_pa_config.h>' '-DSLI_RADIOAES_REQUIRES_MASKING=1' -I"D:\Code\OJT_code\BLE-Advance\central_bgm_220P\config" -I"D:\Code\OJT_code\BLE-Advance\central_bgm_220P\config\btconf" -I"D:\Code\OJT_code\BLE-Advance\central_bgm_220P\autogen" -I"D:\Code\OJT_code\BLE-Advance\central_bgm_220P" -I"D:/Geko_new/gecko-sdk//platform/Device/SiliconLabs/BGM22/Include" -I"D:/Geko_new/gecko-sdk//app/common/util/app_assert" -I"D:/Geko_new/gecko-sdk//app/common/util/app_log" -I"D:/Geko_new/gecko-sdk//app/common/util/app_timer" -I"D:/Geko_new/gecko-sdk//protocol/bluetooth/inc" -I"D:/Geko_new/gecko-sdk//platform/common/inc" -I"D:/Geko_new/gecko-sdk//protocol/bluetooth/bgcommon/inc" -I"D:/Geko_new/gecko-sdk//protocol/bluetooth/bgstack/ll/inc" -I"D:/Geko_new/gecko-sdk//hardware/board/inc" -I"D:/Geko_new/gecko-sdk//platform/bootloader" -I"D:/Geko_new/gecko-sdk//platform/bootloader/api" -I"D:/Geko_new/gecko-sdk//platform/CMSIS/Core/Include" -I"D:/Geko_new/gecko-sdk//hardware/driver/configuration_over_swo/inc" -I"D:/Geko_new/gecko-sdk//platform/security/sl_component/sl_cryptoacc_library/include" -I"D:/Geko_new/gecko-sdk//platform/security/sl_component/sl_cryptoacc_library/src" -I"D:/Geko_new/gecko-sdk//platform/driver/debug/inc" -I"D:/Geko_new/gecko-sdk//platform/service/device_init/inc" -I"D:/Geko_new/gecko-sdk//platform/emdrv/dmadrv/inc" -I"D:/Geko_new/gecko-sdk//platform/emdrv/common/inc" -I"D:/Geko_new/gecko-sdk//platform/emlib/inc" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/plugin/fem_util" -I"D:/Geko_new/gecko-sdk//app/bluetooth/common/gatt_service_device_information" -I"D:/Geko_new/gecko-sdk//app/bluetooth/common/in_place_ota_dfu" -I"D:/Geko_new/gecko-sdk//platform/service/iostream/inc" -I"D:/Geko_new/gecko-sdk//platform/security/sl_component/sl_mbedtls_support/config" -I"D:/Geko_new/gecko-sdk//platform/security/sl_component/sl_mbedtls_support/config/preset" -I"D:/Geko_new/gecko-sdk//platform/security/sl_component/sl_mbedtls_support/inc" -I"D:/Geko_new/gecko-sdk//util/third_party/mbedtls/include" -I"D:/Geko_new/gecko-sdk//util/third_party/mbedtls/library" -I"D:/Geko_new/gecko-sdk//platform/service/mpu/inc" -I"D:/Geko_new/gecko-sdk//hardware/driver/mx25_flash_shutdown/inc/sl_mx25_flash_shutdown_usart" -I"D:/Geko_new/gecko-sdk//platform/emdrv/nvm3/inc" -I"D:/Geko_new/gecko-sdk//platform/service/power_manager/inc" -I"D:/Geko_new/gecko-sdk//platform/security/sl_component/sl_psa_driver/inc" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/common" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/protocol/ble" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/protocol/ieee802154" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/protocol/wmbus" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/protocol/zwave" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/chip/efr32/efr32xg2x" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/protocol/sidewalk" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/plugin/pa-conversions" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/plugin/pa-conversions/efr32xg22" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/plugin/rail_util_power_manager_init" -I"D:/Geko_new/gecko-sdk//platform/radio/rail_lib/plugin/rail_util_pti" -I"D:/Geko_new/gecko-sdk//platform/security/sl_component/se_manager/inc" -I"D:/Geko_new/gecko-sdk//platform/security/sl_component/se_manager/src" -I"D:/Geko_new/gecko-sdk//util/silicon_labs/silabs_core/memory_manager" -I"D:/Geko_new/gecko-sdk//platform/common/toolchain/inc" -I"D:/Geko_new/gecko-sdk//platform/service/system/inc" -I"D:/Geko_new/gecko-sdk//platform/service/sleeptimer/inc" -I"D:/Geko_new/gecko-sdk//platform/security/sl_component/sl_protocol_crypto/src" -I"D:/Geko_new/gecko-sdk//platform/service/udelay/inc" -Os -Wall -Wextra -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mcmse --specs=nano.specs -c -fmessage-length=0 -MMD -MP -MF"sl_gatt_service_device_information.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


