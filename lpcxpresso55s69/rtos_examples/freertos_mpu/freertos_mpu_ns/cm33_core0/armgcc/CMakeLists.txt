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
    SET(SdkRootDirPath "${ProjDirPath}/../../../../../../..")
endif()

SET(EXECUTABLE_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})
SET(LIBRARY_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})


project(freertos_mpu_ns)

set(MCUX_BUILD_TYPES release debug)

set(MCUX_SDK_PROJECT_NAME freertos_mpu_ns.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${SdkRootDirPath})
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../main_ns.c"
"${SdkRootDirPath}/examples/lpcxpresso55s69/rtos_examples/freertos_mpu/freertos_mpu_s/cm33_core0/nsc_functions.h"
"${SdkRootDirPath}/rtos/freertos/freertos-kernel/portable/GCC/ARM_CM33/secure/secure_context.h"
"${SdkRootDirPath}/rtos/freertos/freertos-kernel/portable/GCC/ARM_CM33/secure/secure_init.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${ProjDirPath}/..
    ${SdkRootDirPath}/examples/lpcxpresso55s69/rtos_examples/freertos_mpu/freertos_mpu_s/cm33_core0
    ${SdkRootDirPath}/rtos/freertos/freertos-kernel/portable/GCC/ARM_CM33/secure
    ${SdkRootDirPath}/examples/lpcxpresso55s69/rtos_examples/freertos_mpu/freertos_mpu_ns
)

set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/rtos/freertos/freertos-kernel
    ${SdkRootDirPath}/core/drivers/common
    ${SdkRootDirPath}/core/devices/LPC55S69/drivers
    ${SdkRootDirPath}/core/devices/LPC55S69
    ${SdkRootDirPath}/core/drivers/flexcomm
    ${SdkRootDirPath}/core/drivers/lpc_iocon
    ${SdkRootDirPath}/core/drivers/lpc_gpio
    ${SdkRootDirPath}/core/utilities/assert
    ${SdkRootDirPath}/core/utilities
    ${SdkRootDirPath}/core/components/uart
    ${SdkRootDirPath}/core/components/serial_manager
    ${SdkRootDirPath}/core/components/lists
    ${SdkRootDirPath}/core/CMSIS/Core/Include
    ${SdkRootDirPath}/core/utilities/misc_utilities
)

# include modules
include(middleware_freertos-kernel_cm33_secure_port)

include(middleware_freertos-kernel_heap_4)

include(driver_common)

include(driver_power)

include(driver_clock)

include(device_CMSIS)

include(device_startup_LPC55S69_cm33_core0)

include(driver_flexcomm_usart)

include(driver_flexcomm)

include(driver_lpc_iocon)

include(driver_lpc_gpio)

include(utility_assert)

include(utility_debug_console)

include(component_usart_adapter)

include(component_serial_manager)

include(component_lists)

include(component_serial_manager_uart_LPC55S69_cm33_core0)

include(middleware_freertos-kernel_LPC55S69_cm33_core0)

include(middleware_freertos-kernel_extension)

include(middleware_freertos-kernel_mpu_wrappers)

include(driver_reset)

include(CMSIS_Include_core_cm)

include(utilities_misc_utilities)

include(device_system_LPC55S69_cm33_core0)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../freertos_mpu_s/cm33_core0/armgcc/debug/freertos_mpu_s_CMSE_lib.o)
endif(CMAKE_BUILD_TYPE STREQUAL debug)

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../freertos_mpu_s/cm33_core0/armgcc/release/freertos_mpu_s_CMSE_lib.o)
endif(CMAKE_BUILD_TYPE STREQUAL release)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


