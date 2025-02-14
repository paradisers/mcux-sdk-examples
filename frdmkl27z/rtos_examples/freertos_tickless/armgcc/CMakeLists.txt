# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 3.1.1)

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

set(MCUX_SDK_PROJECT_NAME freertos_tickless.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../low_power_tickless/fsl_tickless_lptmr.c"
"${ProjDirPath}/../low_power_tickless/fsl_tickless_lptmr.h"
"${ProjDirPath}/../freertos_tickless.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${SdkRootDirPath}/core/boards/frdmkl27z/board.c"
"${SdkRootDirPath}/core/boards/frdmkl27z/board.h"
"${SdkRootDirPath}/core/boards/frdmkl27z/clock_config.c"
"${SdkRootDirPath}/core/boards/frdmkl27z/clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/.. debug
    ${ProjDirPath}/../low_power_tickless
    ${ProjDirPath}/..
    ${SdkRootDirPath}/examples/frdmkl27z/rtos_examples/freertos_tickless
    ${SdkRootDirPath}/core/boards/frdmkl27z
)

#path for entry cmake
set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/core
)

#Include Entry cmake component
include(all_devices)

# include modules
include(driver_lptmr)

include(driver_port)

include(driver_gpio)

include(driver_clock)

include(middleware_freertos-kernel_heap_4)

include(driver_common)

include(device_CMSIS)

include(utility_debug_console)

include(component_lpuart_adapter)

include(component_serial_manager)

include(component_lists)

include(component_serial_manager_uart)

include(driver_lpuart)

include(device_startup)

include(driver_uart)

include(driver_smc)

include(utility_assert)

include(CMSIS_Include_core_cm)

include(CMSIS_Include_core_cm)

include(CMSIS_DSP_Include)

include(middleware_freertos-kernel_MKL27Z644)

include(middleware_freertos-kernel_extension)

include(utilities_misc_utilities_MKL27Z644)

include(device_system)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


