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
    SET(SdkRootDirPath "${ProjDirPath}/../../../../../../..")
endif()

SET(EXECUTABLE_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})
SET(LIBRARY_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})


project(lpi2c_read_accel_value_xfer_m41)

set(MCUX_SDK_PROJECT_NAME lpi2c_read_accel_value_xfer_m41.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${SdkRootDirPath}/core/boards/mekmimx8qm/board.c"
"${SdkRootDirPath}/core/boards/mekmimx8qm/board.h"
"${SdkRootDirPath}/core/boards/mekmimx8qm/clock_config.c"
"${SdkRootDirPath}/core/boards/mekmimx8qm/clock_config.h"
"${ProjDirPath}/../lpi2c_read_accel_value_transfer.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${SdkRootDirPath}/examples/mekmimx8qm/driver_examples/lpi2c/read_accel_value_transfer/cm4_core1
    ${SdkRootDirPath}/core/boards/mekmimx8qm
    ${ProjDirPath}/..
)

#path for entry cmake
set(CMAKE_MODULE_PATH
    ${SdkRootDirPath}/core
)

#Include Entry cmake component
include(all_devices)

# include modules
include(driver_irqsteer)

include(driver_igpio)

include(utility_debug_console_lite)

include(utility_assert_lite)

include(driver_lpi2c)

include(driver_clock)

include(driver_common)

include(device_CMSIS)

include(device_startup_MIMX8QM6_cm4_core1)

include(driver_lpuart)

include(driver_scfw_api)

include(component_lpuart_adapter)

include(component_lists)

include(CMSIS_Include_core_cm)

include(CMSIS_Include_core_cm)

include(CMSIS_DSP_Include)

include(utilities_misc_utilities_MIMX8QM6_cm4_core1)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/m4_1_image.bin)

