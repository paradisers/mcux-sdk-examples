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


project(log_bm)

set(MCUX_BUILD_TYPES debug release)

set(MCUX_SDK_PROJECT_NAME log_bm.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${SdkRootDirPath})
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../log_main.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${SdkRootDirPath}/core/boards/frdmkv31f/board.c"
"${SdkRootDirPath}/core/boards/frdmkv31f/board.h"
"${SdkRootDirPath}/core/boards/frdmkv31f/clock_config.c"
"${SdkRootDirPath}/core/boards/frdmkv31f/clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
    ${SdkRootDirPath}/core/boards/frdmkv31f
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/core/drivers/gpio
    ${SdkRootDirPath}/core/devices/MKV31F51212/utilities
    ${SdkRootDirPath}/core/components/lists
    ${SdkRootDirPath}/core/components/panic
    ${SdkRootDirPath}/core/components/serial_manager
    ${SdkRootDirPath}/core/drivers/common
    ${SdkRootDirPath}/core/components/osa
    ${SdkRootDirPath}/core/components/log
    ${SdkRootDirPath}/core/devices/MKV31F51212/drivers
    ${SdkRootDirPath}/core/devices/MKV31F51212
    ${SdkRootDirPath}/core/utilities
    ${SdkRootDirPath}/core/components/uart
    ${SdkRootDirPath}/core/drivers/uart
    ${SdkRootDirPath}/core/drivers/lpuart
    ${SdkRootDirPath}/core/drivers/port
    ${SdkRootDirPath}/core/drivers/smc
    ${SdkRootDirPath}/core/utilities/assert
    ${SdkRootDirPath}/core/CMSIS/Core/Include
    ${SdkRootDirPath}/core/utilities/misc_utilities
)

# include modules
include(driver_gpio)

include(utility_shell)

include(component_lists)

include(component_panic)

include(component_serial_manager)

include(driver_common)

include(component_serial_manager_uart)

include(component_osa_bm)

include(component_log)

include(component_log_backend_debugconsole)

include(component_log_backend_ringbuffer)

include(driver_clock)

include(device_CMSIS)

include(utility_debug_console)

include(component_uart_adapter)

include(driver_uart)

include(device_startup)

include(driver_lpuart)

include(driver_port)

include(driver_smc)

include(utility_assert)

include(CMSIS_Include_core_cm)

include(utilities_misc_utilities_MKV31F51212)

include(device_system)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


