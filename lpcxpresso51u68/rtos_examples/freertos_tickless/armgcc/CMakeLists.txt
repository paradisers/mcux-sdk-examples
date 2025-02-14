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
    SET(SdkRootDirPath "${ProjDirPath}/../../../../..")
endif()

SET(EXECUTABLE_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})
SET(LIBRARY_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})


project(freertos_tickless)

set(MCUX_BUILD_TYPES debug release)

set(MCUX_SDK_PROJECT_NAME freertos_tickless.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${SdkRootDirPath})
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../low_power_tickless/fsl_tickless_rtc.c"
"${ProjDirPath}/../low_power_tickless/fsl_tickless_rtc.h"
"${ProjDirPath}/../freertos_tickless.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${SdkRootDirPath}/core/boards/lpcxpresso51u68/board.c"
"${SdkRootDirPath}/core/boards/lpcxpresso51u68/board.h"
"${SdkRootDirPath}/core/boards/lpcxpresso51u68/clock_config.c"
"${SdkRootDirPath}/core/boards/lpcxpresso51u68/clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
    ${ProjDirPath}/../low_power_tickless
    ${SdkRootDirPath}/core/boards/lpcxpresso51u68
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/core/drivers/lpc_rtc
    ${SdkRootDirPath}/core/drivers/pint
    ${SdkRootDirPath}/core/devices/LPC51U68/drivers
    ${SdkRootDirPath}/core/devices/LPC51U68
    ${SdkRootDirPath}/rtos/freertos/freertos-kernel
    ${SdkRootDirPath}/core/drivers/inputmux
    ${SdkRootDirPath}/core/drivers/common
    ${SdkRootDirPath}/core/utilities
    ${SdkRootDirPath}/core/components/uart
    ${SdkRootDirPath}/core/components/serial_manager
    ${SdkRootDirPath}/core/components/lists
    ${SdkRootDirPath}/core/drivers/flexcomm
    ${SdkRootDirPath}/core/drivers/lpc_iocon
    ${SdkRootDirPath}/core/drivers/lpc_gpio
    ${SdkRootDirPath}/core/utilities/assert
    ${SdkRootDirPath}/core/CMSIS/Core/Include
    ${SdkRootDirPath}/core/utilities/misc_utilities
)

# include modules
include(driver_lpc_rtc)

include(driver_pint)

include(driver_clock)

include(driver_power)

include(middleware_freertos-kernel_heap_4)

include(driver_inputmux)

include(driver_common)

include(device_CMSIS)

include(utility_debug_console)

include(component_usart_adapter)

include(component_serial_manager)

include(component_lists)

include(component_serial_manager_uart)

include(driver_flexcomm_usart)

include(device_startup)

include(driver_flexcomm)

include(driver_lpc_iocon)

include(driver_lpc_gpio)

include(utility_assert)

include(driver_reset)

include(CMSIS_Include_core_cm)

include(middleware_freertos-kernel_LPC51U68)

include(middleware_freertos-kernel_extension)

include(driver_inputmux_connections)

include(utilities_misc_utilities_LPC51U68)

include(device_system)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${SdkRootDirPath}/core/devices/LPC51U68/gcc/libpower.a)
endif(CMAKE_BUILD_TYPE STREQUAL debug)

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${SdkRootDirPath}/core/devices/LPC51U68/gcc/libpower.a)
endif(CMAKE_BUILD_TYPE STREQUAL release)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


