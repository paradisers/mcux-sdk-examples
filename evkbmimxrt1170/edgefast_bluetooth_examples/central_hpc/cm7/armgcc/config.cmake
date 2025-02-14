# config to select component, the format is CONFIG_USE_${component}
# Please refer to cmake files below to get available components:
#  ${SdkRootDirPath}/devices/MIMXRT1176/all_lib_device.cmake

set(CONFIG_COMPILER gcc)
set(CONFIG_TOOLCHAIN armgcc)
set(CONFIG_USE_COMPONENT_CONFIGURATION false)
set(CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind_hci_uart true)
set(CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind_hci true)
set(CONFIG_USE_middleware_wifi_fwdnld true)
set(CONFIG_USE_middleware_edgefast_bluetooth_k32w061_controller true)
set(CONFIG_USE_driver_clock true)
set(CONFIG_USE_driver_common true)
set(CONFIG_USE_device_MIMXRT1176_CMSIS true)
set(CONFIG_USE_device_MIMXRT1176_startup true)
set(CONFIG_USE_driver_lpuart true)
set(CONFIG_USE_driver_igpio true)
set(CONFIG_USE_component_lpuart_adapter true)
set(CONFIG_USE_utility_assert true)
set(CONFIG_USE_utility_debug_console true)
set(CONFIG_USE_driver_xip_device true)
set(CONFIG_USE_driver_xip_board_evkbmimxrt1170 true)
set(CONFIG_USE_driver_iomuxc true)
set(CONFIG_USE_component_serial_manager_uart true)
set(CONFIG_USE_component_serial_manager true)
set(CONFIG_USE_component_lists true)
set(CONFIG_USE_middleware_sdmmc_host_usdhc true)
set(CONFIG_USE_middleware_sdmmc_sdio true)
set(CONFIG_USE_middleware_sdmmc_host_usdhc_freertos true)
set(CONFIG_USE_driver_cache_armv7_m7 true)
set(CONFIG_USE_driver_flexspi true)
set(CONFIG_USE_middleware_usb_host_ehci true)
set(CONFIG_USE_middleware_usb_host_stack true)
set(CONFIG_USE_middleware_usb_phy true)
set(CONFIG_USE_middleware_fatfs_usb true)
set(CONFIG_USE_middleware_fatfs true)
set(CONFIG_USE_middleware_edgefast_bluetooth_pal_platform_ethermind true)
set(CONFIG_USE_middleware_edgefast_bluetooth_pal_host_msd_fatfs_ethermind true)
set(CONFIG_USE_middleware_edgefast_bluetooth_pal_db_gen_ethermind true)
set(CONFIG_USE_component_igpio_adapter true)
set(CONFIG_USE_component_osa true)
set(CONFIG_USE_middleware_freertos-kernel_heap_4 true)
set(CONFIG_USE_middleware_freertos-kernel true)
set(CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind true)
set(CONFIG_USE_middleware_edgefast_bluetooth_config_ethermind true)
set(CONFIG_USE_middleware_edgefast_bluetooth_extension_common_ethermind true)
set(CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_cm7f true)
set(CONFIG_USE_middleware_edgefast_bluetooth_pal true)
set(CONFIG_USE_middleware_edgefast_bluetooth_porting true)
set(CONFIG_USE_middleware_edgefast_bluetooth_porting_work_queue true)
set(CONFIG_USE_middleware_edgefast_bluetooth_porting_toolchain true)
set(CONFIG_USE_middleware_edgefast_bluetooth_porting_net true)
set(CONFIG_USE_middleware_edgefast_bluetooth_porting_list true)
set(CONFIG_USE_middleware_edgefast_bluetooth_porting_atomic true)
set(CONFIG_USE_middleware_edgefast_bluetooth_profile_hps true)
set(CONFIG_USE_driver_lpuart_edma true)
set(CONFIG_USE_driver_pmu_1 true)
set(CONFIG_USE_driver_dcdc_soc true)
set(CONFIG_USE_component_flexspi_nor_flash_adapter_rt1170evkb true)
set(CONFIG_USE_middleware_littlefs true)
set(CONFIG_USE_middleware_mbedtls true)
set(CONFIG_USE_driver_caam true)
set(CONFIG_USE_middleware_mbedtls_port_ksdk true)
set(CONFIG_USE_middleware_edgefast_bluetooth_wifi_nxp_controller_base true)
set(CONFIG_USE_utilities_misc_utilities true)
set(CONFIG_USE_middleware_wifi_template true)
set(CONFIG_USE_middleware_wifi_mlan_sdio true)
set(CONFIG_USE_middleware_wifi_common_files true)
set(CONFIG_USE_component_wifi_bt_module_tx_pwr_limits true)
set(CONFIG_USE_driver_anatop_ai true)
set(CONFIG_USE_CMSIS_Include_core_cm true)
set(CONFIG_USE_device_MIMXRT1176_system true)
set(CONFIG_USE_middleware_sdmmc_common true)
set(CONFIG_USE_middleware_sdmmc_osa_freertos true)
set(CONFIG_USE_middleware_sdmmc_usdhc_template true)
set(CONFIG_USE_middleware_sdmmc_host_usdhc_cache true)
set(CONFIG_USE_driver_usdhc true)
set(CONFIG_USE_component_osa_free_rtos true)
set(CONFIG_USE_component_osa_interface true)
set(CONFIG_USE_driver_memory true)
set(CONFIG_USE_middleware_usb_host_ehci_config_header true)
set(CONFIG_USE_middleware_usb_host_common_header true)
set(CONFIG_USE_middleware_usb_common_header true)
set(CONFIG_USE_middleware_fatfs_template_usb true)
set(CONFIG_USE_middleware_usb_host_msd true)
set(CONFIG_USE_middleware_freertos-kernel_template true)
set(CONFIG_USE_middleware_freertos-kernel_extension true)
set(CONFIG_USE_middleware_edgefast_bluetooth_template true)
set(CONFIG_USE_middleware_edgefast_bluetooth_ble_ethermind_lib_cm7f true)
set(CONFIG_USE_middleware_edgefast_bluetooth_config_template true)
set(CONFIG_USE_middleware_edgefast_bluetooth_mcux_linker_template_evkbmimxrt1170 true)
set(CONFIG_USE_component_log true)
set(CONFIG_USE_component_log_backend_debugconsole true)
set(CONFIG_USE_utility_str true)
set(CONFIG_USE_driver_edma true)
set(CONFIG_USE_driver_dmamux true)
set(CONFIG_USE_middleware_mbedtls_template true)
set(CONFIG_USE_middleware_edgefast_bluetooth_sdio_template_evkbmimxrt1170 true)
set(CONFIG_CORE cm7f)
set(CONFIG_DEVICE MIMXRT1176)
set(CONFIG_BOARD evkbmimxrt1170)
set(CONFIG_KIT evkbmimxrt1170)
set(CONFIG_DEVICE_ID MIMXRT1176xxxxx)
set(CONFIG_FPU DP_FPU)
set(CONFIG_DSP NO_DSP)
set(CONFIG_CORE_ID cm7)
