# config to select component, the format is CONFIG_USE_${component}
# Please refer to cmake files below to get available components:
#  ${SdkRootDirPath}/devices/MIMXRT1052/all_lib_device.cmake

set(CONFIG_COMPILER gcc)
set(CONFIG_TOOLCHAIN armgcc)
set(CONFIG_USE_COMPONENT_CONFIGURATION false)
set(CONFIG_USE_component_audio_sai_edma_adapter true)
set(CONFIG_USE_component_lpi2c_adapter true)
set(CONFIG_USE_component_codec_i2c true)
set(CONFIG_USE_driver_cache_armv7_m7 true)
set(CONFIG_USE_component_wm8960_adapter true)
set(CONFIG_USE_driver_codec true)
set(CONFIG_USE_driver_dmamux true)
set(CONFIG_USE_driver_edma true)
set(CONFIG_USE_driver_lpi2c true)
set(CONFIG_USE_driver_sai true)
set(CONFIG_USE_driver_sai_edma true)
set(CONFIG_USE_driver_wm8960 true)
set(CONFIG_USE_middleware_usb_device_ehci true)
set(CONFIG_USE_middleware_usb_phy true)
set(CONFIG_USE_utility_assert_lite true)
set(CONFIG_USE_utility_debug_console_lite true)
set(CONFIG_USE_middleware_baremetal true)
set(CONFIG_USE_component_lists true)
set(CONFIG_USE_component_lpuart_adapter true)
set(CONFIG_USE_driver_lpuart true)
set(CONFIG_USE_device_MIMXRT1052_CMSIS true)
set(CONFIG_USE_device_MIMXRT1052_startup true)
set(CONFIG_USE_driver_clock true)
set(CONFIG_USE_driver_common true)
set(CONFIG_USE_driver_igpio true)
set(CONFIG_USE_driver_iomuxc true)
set(CONFIG_USE_driver_xip_board_evkbimxrt1050 true)
set(CONFIG_USE_driver_xip_device true)
set(CONFIG_USE_middleware_usb_device_controller_driver true)
set(CONFIG_USE_utilities_misc_utilities true)
set(CONFIG_USE_middleware_usb_device_ehci_config_header true)
set(CONFIG_USE_middleware_usb_device_common_header true)
set(CONFIG_USE_component_osa true)
set(CONFIG_USE_middleware_usb_common_header true)
set(CONFIG_USE_CMSIS_Include_core_cm true)
set(CONFIG_USE_device_MIMXRT1052_system true)
set(CONFIG_CORE cm7f)
set(CONFIG_DEVICE MIMXRT1052)
set(CONFIG_BOARD evkbimxrt1050)
set(CONFIG_KIT evkbimxrt1050)
set(CONFIG_DEVICE_ID MIMXRT1052xxxxB)
set(CONFIG_FPU DP_FPU)
set(CONFIG_DSP NO_DSP)
