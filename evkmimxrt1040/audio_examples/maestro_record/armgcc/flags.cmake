IF(NOT DEFINED FPU)  
    SET(FPU "-mfloat-abi=hard -mfpu=fpv5-d16")  
ENDIF()  

IF(NOT DEFINED SPECS)  
    SET(SPECS "--specs=nano.specs --specs=nosys.specs")  
ENDIF()  

IF(NOT DEFINED DEBUG_CONSOLE_CONFIG)  
    SET(DEBUG_CONSOLE_CONFIG "-DSDK_DEBUGCONSOLE=1")  
ENDIF()  

SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG " \
    ${CMAKE_ASM_FLAGS_FLEXSPI_NOR_DEBUG} \
    -D__STARTUP_CLEAR_BSS \
    -DDEBUG \
    -D__STARTUP_INITIALIZE_NONCACHEDATA \
    -mcpu=cortex-m7 \
    -mthumb \
    ${FPU} \
")
SET(CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE " \
    ${CMAKE_ASM_FLAGS_FLEXSPI_NOR_RELEASE} \
    -D__STARTUP_CLEAR_BSS \
    -DNDEBUG \
    -D__STARTUP_INITIALIZE_NONCACHEDATA \
    -mcpu=cortex-m7 \
    -mthumb \
    ${FPU} \
")
SET(CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG " \
    ${CMAKE_C_FLAGS_FLEXSPI_NOR_DEBUG} \
    -DXIP_EXTERNAL_FLASH=1 \
    -DXIP_BOOT_HEADER_ENABLE=1 \
    -DDEBUG \
    -DCPU_MIMXRT1042XJM5B \
    -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 \
    -DOPUS_ENCODE \
    -DSTREAMER_ENABLE_ENCODER \
    -DSTREAMER_ENABLE_CEI_OPUS \
    -DSTREAMER_ENABLE_FILE_SINK \
    -DSTREAMER_ENABLE_MEM_SRC \
    -DSTREAMER_ENABLE_MEM_SINK \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DOSA_USED \
    -DSHELL_TASK_STACK_SIZE=4000 \
    -DSAI_XFER_QUEUE_SIZE=2 \
    -DMCUXPRESSO_SDK \
    -DSDK_I2C_BASED_COMPONENT_USED=1 \
    -DBOARD_USE_CODEC=1 \
    -DCODEC_WM8960_ENABLE \
    -DCODEC_MULTI_ADAPTERS=1 \
    -DSD_ENABLED \
    -DDISABLEFLOAT16 \
    -DHAVE_CONFIG_H \
    -DCASCFG_PLATFORM_FREERTOS \
    -DFSL_OS_SELECTED=SDK_OS_FREERTOS \
    -DFSL_OSA_TASK_ENABLE=1 \
    -DDEBUG_CONSOLE_RX_ENABLE=0 \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DCODEC_CS42448_ENABLE \
    -DSDK_OS_FREE_RTOS \
    -g \
    -O0 \
    -mcpu=cortex-m7 \
    -Wall \
    -mthumb \
    -MMD \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mapcs \
    -std=gnu99 \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE " \
    ${CMAKE_C_FLAGS_FLEXSPI_NOR_RELEASE} \
    -DXIP_EXTERNAL_FLASH=1 \
    -DXIP_BOOT_HEADER_ENABLE=1 \
    -DNDEBUG \
    -DCPU_MIMXRT1042XJM5B \
    -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 \
    -DOPUS_ENCODE \
    -DSTREAMER_ENABLE_ENCODER \
    -DSTREAMER_ENABLE_CEI_OPUS \
    -DSTREAMER_ENABLE_FILE_SINK \
    -DSTREAMER_ENABLE_MEM_SRC \
    -DSTREAMER_ENABLE_MEM_SINK \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DOSA_USED \
    -DSHELL_TASK_STACK_SIZE=4000 \
    -DSAI_XFER_QUEUE_SIZE=2 \
    -DMCUXPRESSO_SDK \
    -DSDK_I2C_BASED_COMPONENT_USED=1 \
    -DBOARD_USE_CODEC=1 \
    -DCODEC_WM8960_ENABLE \
    -DCODEC_MULTI_ADAPTERS=1 \
    -DSD_ENABLED \
    -DDISABLEFLOAT16 \
    -DHAVE_CONFIG_H \
    -DCASCFG_PLATFORM_FREERTOS \
    -DFSL_OS_SELECTED=SDK_OS_FREERTOS \
    -DFSL_OSA_TASK_ENABLE=1 \
    -DDEBUG_CONSOLE_RX_ENABLE=0 \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DCODEC_CS42448_ENABLE \
    -DSDK_OS_FREE_RTOS \
    -Os \
    -mcpu=cortex-m7 \
    -Wall \
    -mthumb \
    -MMD \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mapcs \
    -std=gnu99 \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG " \
    ${CMAKE_CXX_FLAGS_FLEXSPI_NOR_DEBUG} \
    -DDEBUG \
    -DCPU_MIMXRT1042XJM5B \
    -DMCUXPRESSO_SDK \
    -DSERIAL_PORT_TYPE_UART=1 \
    -g \
    -O0 \
    -mcpu=cortex-m7 \
    -Wall \
    -mthumb \
    -MMD \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mapcs \
    -fno-rtti \
    -fno-exceptions \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE " \
    ${CMAKE_CXX_FLAGS_FLEXSPI_NOR_RELEASE} \
    -DNDEBUG \
    -DCPU_MIMXRT1042XJM5B \
    -DMCUXPRESSO_SDK \
    -DSERIAL_PORT_TYPE_UART=1 \
    -Os \
    -mcpu=cortex-m7 \
    -Wall \
    -mthumb \
    -MMD \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mapcs \
    -fno-rtti \
    -fno-exceptions \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG " \
    ${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_DEBUG} \
    -g \
    -mcpu=cortex-m7 \
    -Wall \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mthumb \
    -mapcs \
    -Xlinker \
    --gc-sections \
    -Xlinker \
    -static \
    -Xlinker \
    -z \
    -Xlinker \
    muldefs \
    -Xlinker \
    -Map=output.map \
    -Wl,--print-memory-usage \
    -Xlinker \
    --defsym=__stack_size__=0x1000 \
    -Xlinker \
    --defsym=__heap_size__=0x1000 \
    ${FPU} \
    ${SPECS} \
    -T${ProjDirPath}/MIMXRT1042xxxxx_flexspi_nor.ld -static \
")
SET(CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE " \
    ${CMAKE_EXE_LINKER_FLAGS_FLEXSPI_NOR_RELEASE} \
    -mcpu=cortex-m7 \
    -Wall \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mthumb \
    -mapcs \
    -Xlinker \
    --gc-sections \
    -Xlinker \
    -static \
    -Xlinker \
    -z \
    -Xlinker \
    muldefs \
    -Xlinker \
    -Map=output.map \
    -Wl,--print-memory-usage \
    -Xlinker \
    --defsym=__stack_size__=0x1000 \
    -Xlinker \
    --defsym=__heap_size__=0x1000 \
    ${FPU} \
    ${SPECS} \
    -T${ProjDirPath}/MIMXRT1042xxxxx_flexspi_nor.ld -static \
")
