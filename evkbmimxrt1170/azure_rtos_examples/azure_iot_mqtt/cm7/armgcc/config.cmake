# config to select component, the format is CONFIG_USE_${component}
# Please refer to cmake files below to get available components:
#  ${SdkRootDirPath}/devices/MIMXRT1176/all_lib_device.cmake

set(CONFIG_COMPILER gcc)
set(CONFIG_TOOLCHAIN armgcc)
set(CONFIG_USE_COMPONENT_CONFIGURATION false)
set(CONFIG_USE_middleware_azure_rtos_nxd true)
set(CONFIG_USE_middleware_netxduo_imxrt true)
set(CONFIG_USE_driver_enet true)
set(CONFIG_USE_driver_phy-device-rtl8201 true)
set(CONFIG_USE_driver_phy-device-rtl8211f true)
set(CONFIG_USE_driver_caam true)
set(CONFIG_USE_component_lists true)
set(CONFIG_USE_component_lpuart_adapter true)
set(CONFIG_USE_component_serial_manager_uart true)
set(CONFIG_USE_component_serial_manager true)
set(CONFIG_USE_driver_lpuart true)
set(CONFIG_USE_device_MIMXRT1176_CMSIS true)
set(CONFIG_USE_device_MIMXRT1176_startup true)
set(CONFIG_USE_driver_cache_armv7_m7 true)
set(CONFIG_USE_driver_clock true)
set(CONFIG_USE_driver_common true)
set(CONFIG_USE_driver_dcdc_soc true)
set(CONFIG_USE_driver_igpio true)
set(CONFIG_USE_driver_iomuxc true)
set(CONFIG_USE_driver_lpi2c true)
set(CONFIG_USE_driver_pmu_1 true)
set(CONFIG_USE_driver_xip_board_evkbmimxrt1170 true)
set(CONFIG_USE_driver_xip_device true)
set(CONFIG_USE_utility_assert true)
set(CONFIG_USE_utility_debug_console true)
set(CONFIG_USE_utilities_misc_utilities true)
set(CONFIG_USE_middleware_azure_rtos_nxd_template true)
set(CONFIG_USE_middleware_azure_rtos_fx true)
set(CONFIG_USE_middleware_azure_rtos_fx_template true)
set(CONFIG_USE_middleware_azure_rtos_tx true)
set(CONFIG_USE_middleware_azure_rtos_tx_template true)
set(CONFIG_USE_driver_memory true)
set(CONFIG_USE_driver_phy-common true)
set(CONFIG_USE_CMSIS_Include_core_cm true)
set(CONFIG_USE_device_MIMXRT1176_system true)
set(CONFIG_USE_driver_anatop_ai true)
set(CONFIG_CORE cm7f)
set(CONFIG_DEVICE MIMXRT1176)
set(CONFIG_BOARD evkbmimxrt1170)
set(CONFIG_KIT evkbmimxrt1170)
set(CONFIG_DEVICE_ID MIMXRT1176xxxxx)
set(CONFIG_FPU DP_FPU)
set(CONFIG_DSP NO_DSP)
set(CONFIG_CORE_ID cm7)
