# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)

# THE VERSION NUMBER
SET (MCUXPRESSO_CMAKE_FORMAT_MAJOR_VERSION 2)
SET (MCUXPRESSO_CMAKE_FORMAT_MINOR_VERSION 0)

include(ide_overrides.cmake OPTIONAL)

if(CMAKE_SCRIPT_MODE_FILE)
  message("${MCUXPRESSO_CMAKE_FORMAT_MAJOR_VERSION}")
  return()
endif()


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


project(erpc_matrix_multiply_rpmsg_rtos_cm7)

set(MCUX_BUILD_TYPES flexspi_nor_debug flexspi_nor_release)

set(MCUX_SDK_PROJECT_NAME erpc_matrix_multiply_rpmsg_rtos_cm7.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${SdkRootDirPath})
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${SdkRootDirPath}/examples/evkmimxrt1160/multicore_examples/erpc_common/erpc_error_handler.h"
"${SdkRootDirPath}/examples/evkmimxrt1160/multicore_examples/erpc_common/erpc_error_handler.cpp"
"${SdkRootDirPath}/examples/evkmimxrt1160/multicore_examples/erpc_common/erpc_matrix_multiply/service/erpc_matrix_multiply.erpc"
"${SdkRootDirPath}/examples/evkmimxrt1160/multicore_examples/erpc_common/erpc_matrix_multiply/service/erpc_matrix_multiply.h"
"${SdkRootDirPath}/examples/evkmimxrt1160/multicore_examples/erpc_common/erpc_matrix_multiply/service/erpc_matrix_multiply_client.cpp"
"${ProjDirPath}/../main_core0.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${SdkRootDirPath}/examples/evkmimxrt1160/multicore_examples/erpc_matrix_multiply_rpmsg_rtos/rpmsg_config.h"
"${SdkRootDirPath}/examples/evkmimxrt1160/multicore_examples/erpc_matrix_multiply_rpmsg_rtos/erpc_config.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${SdkRootDirPath}/core/boards/evkmimxrt1160/dcd.c"
"${SdkRootDirPath}/core/boards/evkmimxrt1160/dcd.h"
"${ProjDirPath}/../evkmimxrt1160_connect_cm4_cm7side.jlinkscript"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${SdkRootDirPath}/core/boards/evkmimxrt1160
    ${SdkRootDirPath}/examples/evkmimxrt1160/multicore_examples/erpc_common
    ${SdkRootDirPath}/examples/evkmimxrt1160/multicore_examples/erpc_common/erpc_matrix_multiply/service
    ${SdkRootDirPath}/examples/evkmimxrt1160/multicore_examples/erpc_matrix_multiply_rpmsg_rtos
)

set_source_files_properties("${ProjDirPath}/../FreeRTOSConfig.h" PROPERTIES COMPONENT_CONFIG_FILE "middleware_freertos-kernel_template")

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../cm4/armgcc/debug)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_debug)

if(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)
    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../cm4/armgcc/release)
endif(CMAKE_BUILD_TYPE STREQUAL flexspi_nor_release)

include(${SdkRootDirPath}/core/devices/MIMXRT1166/all_lib_device.cmake)

IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

set_target_properties(${MCUX_SDK_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES "output.map")

