# config to select component, the format is CONFIG_USE_${component}
# Please refer to cmake files below to get available components:
#  ${SdkRootDirPath}/devices/LPC51U68/all_lib_device.cmake

set(CONFIG_COMPILER gcc)
set(CONFIG_TOOLCHAIN armgcc)
set(CONFIG_USE_COMPONENT_CONFIGURATION false)
set(CONFIG_USE_driver_flexcomm_usart true)
set(CONFIG_USE_driver_ctimer true)
set(CONFIG_USE_driver_common true)
set(CONFIG_USE_driver_clock true)
set(CONFIG_USE_driver_power true)
set(CONFIG_USE_driver_lpc_iocon true)
set(CONFIG_USE_driver_lpc_gpio true)
set(CONFIG_USE_driver_inputmux true)
set(CONFIG_USE_device_LPC51U68_CMSIS true)
set(CONFIG_USE_utility_debug_console true)
set(CONFIG_USE_component_usart_adapter true)
set(CONFIG_USE_component_serial_manager_uart true)
set(CONFIG_USE_component_serial_manager true)
set(CONFIG_USE_component_lists true)
set(CONFIG_USE_device_LPC51U68_startup true)
set(CONFIG_USE_driver_flexcomm true)
set(CONFIG_USE_utility_assert true)
set(CONFIG_USE_utilities_misc_utilities true)
set(CONFIG_USE_driver_reset true)
set(CONFIG_USE_driver_inputmux_connections true)
set(CONFIG_USE_CMSIS_Include_core_cm true)
set(CONFIG_USE_device_LPC51U68_system true)
set(CONFIG_CORE cm0p)
set(CONFIG_DEVICE LPC51U68)
set(CONFIG_BOARD lpcxpresso51u68)
set(CONFIG_KIT lpcxpresso51u68)
set(CONFIG_DEVICE_ID LPC51U68)
set(CONFIG_FPU NO_FPU)
set(CONFIG_DSP NO_DSP)
