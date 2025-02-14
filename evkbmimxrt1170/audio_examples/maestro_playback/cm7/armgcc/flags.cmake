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
    -DPLATFORM_ID=LVM_IMXRT1170 \
    -DAAC_DEC=1 \
    -DALGORITHM_CS=1 \
    -DALGORITHM_XO=1 \
    -DCPU_MIMXRT1176DVMAA_cm7 \
    -DPRINTF_ADVANCED_ENABLE=1 \
    -DPRINTF_FLOAT_ENABLE=1 \
    -DOGG_OPUS_DEC=1 \
    -DWAV_DEC=1 \
    -DSSRC_PROC=1 \
    -DMP3_DEC=1 \
    -DSTREAMER_ENABLE_FILESRC \
    -DSTREAMER_ENABLE_MEM_SRC \
    -DSTREAMER_ENABLE_MEM_SINK \
    -DSTREAMER_ENABLE_DECODER \
    -DSTREAMER_ENABLE_AUDIO_PROC \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DOSA_USED \
    -DSHELL_TASK_STACK_SIZE=4000 \
    -DSAI_XFER_QUEUE_SIZE=2 \
    -DMCUXPRESSO_SDK \
    -DSDK_I2C_BASED_COMPONENT_USED=1 \
    -DBOARD_USE_CODEC=1 \
    -DCODEC_WM8962_ENABLE \
    -DSD_ENABLED \
    -DCASCFG_PLATFORM_FREERTOS \
    -DFSL_OS_SELECTED=SDK_OS_FREERTOS \
    -DFSL_OSA_TASK_ENABLE=1 \
    -DDEBUG_CONSOLE_RX_ENABLE=0 \
    -DALGORITHM_VC=1 \
    -DALGORITHM_DBE=1 \
    -DALGORITHM_EQNB=1 \
    -DALGORITHM_LM=1 \
    -DALGORITHM_TE=1 \
    -DALGORITHM_TG=1 \
    -DALGORITHM_AVL=1 \
    -DALGORITHM_PSA=1 \
    -DALGORITHM_PR_EQNB=1 \
    -DALGORITHM_LIMP=1 \
    -DALGORITHM_LIMR=1 \
    -DHAVE_CONFIG_H \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DSDK_OS_FREE_RTOS \
    -DEAP_PROC \
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
    -DPLATFORM_ID=LVM_IMXRT1170 \
    -DAAC_DEC=1 \
    -DALGORITHM_CS=1 \
    -DALGORITHM_XO=1 \
    -DCPU_MIMXRT1176DVMAA_cm7 \
    -DPRINTF_ADVANCED_ENABLE=1 \
    -DPRINTF_FLOAT_ENABLE=1 \
    -DOGG_OPUS_DEC=1 \
    -DWAV_DEC=1 \
    -DSSRC_PROC=1 \
    -DMP3_DEC=1 \
    -DSTREAMER_ENABLE_FILESRC \
    -DSTREAMER_ENABLE_MEM_SRC \
    -DSTREAMER_ENABLE_MEM_SINK \
    -DSTREAMER_ENABLE_DECODER \
    -DSTREAMER_ENABLE_AUDIO_PROC \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DOSA_USED \
    -DSHELL_TASK_STACK_SIZE=4000 \
    -DSAI_XFER_QUEUE_SIZE=2 \
    -DMCUXPRESSO_SDK \
    -DSDK_I2C_BASED_COMPONENT_USED=1 \
    -DBOARD_USE_CODEC=1 \
    -DCODEC_WM8962_ENABLE \
    -DSD_ENABLED \
    -DCASCFG_PLATFORM_FREERTOS \
    -DFSL_OS_SELECTED=SDK_OS_FREERTOS \
    -DFSL_OSA_TASK_ENABLE=1 \
    -DDEBUG_CONSOLE_RX_ENABLE=0 \
    -DALGORITHM_VC=1 \
    -DALGORITHM_DBE=1 \
    -DALGORITHM_EQNB=1 \
    -DALGORITHM_LM=1 \
    -DALGORITHM_TE=1 \
    -DALGORITHM_TG=1 \
    -DALGORITHM_AVL=1 \
    -DALGORITHM_PSA=1 \
    -DALGORITHM_PR_EQNB=1 \
    -DALGORITHM_LIMP=1 \
    -DALGORITHM_LIMR=1 \
    -DHAVE_CONFIG_H \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DSDK_OS_FREE_RTOS \
    -DEAP_PROC \
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
    -DCPU_MIMXRT1176DVMAA_cm7 \
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
    -DCPU_MIMXRT1176DVMAA_cm7 \
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
    --defsym=__use_flash64MB__=1 \
    -Xlinker \
    --defsym=__stack_size__=0x1000 \
    -Xlinker \
    --defsym=__heap_size__=0x1000 \
    ${FPU} \
    ${SPECS} \
    -T${ProjDirPath}/MIMXRT1176xxxxx_cm7_flexspi_nor.ld -static \
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
    --defsym=__use_flash64MB__=1 \
    -Xlinker \
    --defsym=__stack_size__=0x1000 \
    -Xlinker \
    --defsym=__heap_size__=0x1000 \
    ${FPU} \
    ${SPECS} \
    -T${ProjDirPath}/MIMXRT1176xxxxx_cm7_flexspi_nor.ld -static \
")
