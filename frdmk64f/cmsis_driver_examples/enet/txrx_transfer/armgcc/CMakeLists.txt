# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)

# THE VERSION NUMBER
SET (Tutorial_VERSION_MAJOR 1)
SET (Tutorial_VERSION_MINOR 0)

# ENABLE ASM
ENABLE_LANGUAGE(ASM)

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)

SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

# CURRENT DIRECTORY
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})

if (DEFINED ENV{SdkRootDirPath})
    SET(SdkRootDirPath "$ENV{SdkRootDirPath}")
else()
    SET(SdkRootDirPath "${ProjDirPath}/../../../../../..")
endif()

SET(EXECUTABLE_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})
SET(LIBRARY_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})


project(cmsis_enet_txrx_transfer)

set(MCUX_SDK_PROJECT_NAME cmsis_enet_txrx_transfer.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../cmsis_enet_txrx_transfer.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../RTE_Device.h"
"${SdkRootDirPath}/core/boards/frdmk64f/board.c"
"${SdkRootDirPath}/core/boards/frdmk64f/board.h"
"${SdkRootDirPath}/core/boards/frdmk64f/clock_config.c"
"${SdkRootDirPath}/core/boards/frdmk64f/clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${SdkRootDirPath}/examples/frdmk64f/cmsis_driver_examples/enet/txrx_transfer
    ${SdkRootDirPath}/core/boards/frdmk64f
)

#path for entry cmake
set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/core
)

#Include Entry cmake component
include(all_devices)

# include modules
include(driver_mdio-enet)

include(driver_phy-device-ksz8081)

include(utility_debug_console_lite)

include(utility_assert_lite)

include(driver_port)

include(driver_sysmpu)

include(driver_sim)

include(driver_gpio)

include(driver_clock)

include(driver_enet)

include(driver_cmsis_enet)

include(driver_common)

include(driver_flash)

include(driver_uart)

include(driver_smc)

include(device_CMSIS)

include(component_uart_adapter)

include(component_lists)

include(device_startup)

include(driver_mdio-common)

include(CMSIS_Include_core_cm)

include(driver_phy-common)

include(CMSIS_Driver_Include_Ethernet_MAC)

include(CMSIS_Driver_Include_Ethernet)

include(CMSIS_Driver_Include_Common)

include(CMSIS_Driver_Include_Ethernet_PHY)

include(driver_edma_MK64F12)

include(driver_dmamux)

include(utilities_misc_utilities_MK64F12)

include(device_system)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


