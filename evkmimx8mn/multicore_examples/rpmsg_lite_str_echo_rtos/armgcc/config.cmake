# config to select component, the format is CONFIG_USE_${component}
# Please refer to cmake files below to get available components:
#  ${SdkRootDirPath}/devices/MIMX8MN6/all_lib_device.cmake

set(CONFIG_COMPILER gcc)
set(CONFIG_TOOLCHAIN armgcc)
set(CONFIG_USE_COMPONENT_CONFIGURATION false)
set(CONFIG_USE_middleware_multicore_rpmsg_lite_imx8mn_m7_freertos true)
set(CONFIG_USE_middleware_multicore_rpmsg_lite_freertos true)
set(CONFIG_USE_middleware_multicore_rpmsg_lite true)
set(CONFIG_USE_middleware_freertos-kernel_heap_4 true)
set(CONFIG_USE_driver_clock true)
set(CONFIG_USE_driver_mu true)
set(CONFIG_USE_middleware_freertos-kernel true)
set(CONFIG_USE_driver_common true)
set(CONFIG_USE_driver_rdc true)
set(CONFIG_USE_device_MIMX8MN6_CMSIS true)
set(CONFIG_USE_utility_debug_console true)
set(CONFIG_USE_component_iuart_adapter true)
set(CONFIG_USE_component_serial_manager_uart true)
set(CONFIG_USE_component_serial_manager true)
set(CONFIG_USE_driver_iuart true)
set(CONFIG_USE_component_lists true)
set(CONFIG_USE_device_MIMX8MN6_startup true)
set(CONFIG_USE_utility_assert true)
set(CONFIG_USE_utilities_misc_utilities true)
set(CONFIG_USE_middleware_freertos-kernel_template true)
set(CONFIG_USE_middleware_freertos-kernel_extension true)
set(CONFIG_USE_CMSIS_Include_core_cm true)
set(CONFIG_USE_device_MIMX8MN6_system true)
set(CONFIG_CORE cm7f)
set(CONFIG_DEVICE MIMX8MN6)
set(CONFIG_BOARD evkmimx8mn)
set(CONFIG_KIT evkmimx8mn)
set(CONFIG_DEVICE_ID MIMX8MN6xxxJZ)
set(CONFIG_FPU SP_FPU)
set(CONFIG_DSP NO_DSP)
