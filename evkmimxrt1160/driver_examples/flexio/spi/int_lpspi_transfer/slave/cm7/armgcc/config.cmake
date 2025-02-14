# config to select component, the format is CONFIG_USE_${component}
# Please refer to cmake files below to get available components:
#  ${SdkRootDirPath}/devices/MIMXRT1166/all_lib_device.cmake

set(CONFIG_COMPILER gcc)
set(CONFIG_TOOLCHAIN armgcc)
set(CONFIG_USE_COMPONENT_CONFIGURATION false)
set(CONFIG_USE_utility_debug_console_lite true)
set(CONFIG_USE_utility_assert_lite true)
set(CONFIG_USE_driver_clock true)
set(CONFIG_USE_driver_flexio true)
set(CONFIG_USE_driver_flexio_spi true)
set(CONFIG_USE_driver_lpspi true)
set(CONFIG_USE_driver_common true)
set(CONFIG_USE_device_MIMXRT1166_CMSIS true)
set(CONFIG_USE_component_lpuart_adapter true)
set(CONFIG_USE_driver_lpuart true)
set(CONFIG_USE_component_lists true)
set(CONFIG_USE_device_MIMXRT1166_startup true)
set(CONFIG_USE_driver_iomuxc true)
set(CONFIG_USE_driver_igpio true)
set(CONFIG_USE_driver_xip_device true)
set(CONFIG_USE_driver_xip_board_evkmimxrt1160 true)
set(CONFIG_USE_driver_pmu_1 true)
set(CONFIG_USE_driver_dcdc_soc true)
set(CONFIG_USE_driver_cache_armv7_m7 true)
set(CONFIG_USE_utilities_misc_utilities true)
set(CONFIG_USE_driver_anatop_ai true)
set(CONFIG_USE_CMSIS_Include_core_cm true)
set(CONFIG_USE_device_MIMXRT1166_system true)
set(CONFIG_CORE cm7f)
set(CONFIG_DEVICE MIMXRT1166)
set(CONFIG_BOARD evkmimxrt1160)
set(CONFIG_KIT evkmimxrt1160)
set(CONFIG_DEVICE_ID MIMXRT1166xxxxx)
set(CONFIG_FPU DP_FPU)
set(CONFIG_DSP NO_DSP)
set(CONFIG_CORE_ID cm7)
