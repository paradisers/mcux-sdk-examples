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
    SET(SdkRootDirPath "${ProjDirPath}/../../../../..")
endif()

SET(EXECUTABLE_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})
SET(LIBRARY_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})


project(erpc_server_dac_adc)

set(MCUX_BUILD_TYPES debug release)

set(MCUX_SDK_PROJECT_NAME erpc_server_dac_adc.elf)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${SdkRootDirPath})
endif()

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../fxos.c"
"${SdkRootDirPath}/examples/frdmk22f/multiprocessor_examples/erpc_common/erpc_error_handler.h"
"${SdkRootDirPath}/examples/frdmk22f/multiprocessor_examples/erpc_common/erpc_error_handler.cpp"
"${ProjDirPath}/../main_server.c"
"${ProjDirPath}/../erpc_config.h"
"${ProjDirPath}/../service/erpc_dac_adc.erpc"
"${ProjDirPath}/../service/erpc_dac_adc.h"
"${ProjDirPath}/../service/erpc_dac_adc_server.h"
"${ProjDirPath}/../service/erpc_dac_adc_server.cpp"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../RTE_Device.h"
"${SdkRootDirPath}/core/boards/frdmk22f/board.c"
"${SdkRootDirPath}/core/boards/frdmk22f/board.h"
"${SdkRootDirPath}/core/boards/frdmk22f/clock_config.c"
"${SdkRootDirPath}/core/boards/frdmk22f/clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${SdkRootDirPath}/examples/frdmk22f/multiprocessor_examples/erpc_common
    ${ProjDirPath}/..
    ${ProjDirPath}/../service
    ${SdkRootDirPath}/core/boards/frdmk22f
)

set_source_files_properties("${ProjDirPath}/../RTE_Device.h" PROPERTIES COMPONENT_CONFIG_FILE "RTE_Device.MK22F51212")

include(${SdkRootDirPath}/core/devices/MK22F51212/all_lib_device.cmake)

IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lstdc++ -lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE g)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

set_target_properties(${MCUX_SDK_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES "output.map")

