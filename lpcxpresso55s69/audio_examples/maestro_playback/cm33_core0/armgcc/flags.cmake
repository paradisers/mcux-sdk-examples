IF(NOT DEFINED FPU)  
    SET(FPU "-mfloat-abi=hard -mfpu=fpv5-sp-d16")  
ENDIF()  

IF(NOT DEFINED SPECS)  
    SET(SPECS "--specs=nano.specs --specs=nosys.specs")  
ENDIF()  

IF(NOT DEFINED DEBUG_CONSOLE_CONFIG)  
    SET(DEBUG_CONSOLE_CONFIG "-DSDK_DEBUGCONSOLE=1")  
ENDIF()  

SET(CMAKE_ASM_FLAGS_DEBUG " \
    ${CMAKE_ASM_FLAGS_DEBUG} \
    -DDEBUG \
    -D__STARTUP_CLEAR_BSS \
    -mcpu=cortex-m33 \
    -mthumb \
    ${FPU} \
")
SET(CMAKE_ASM_FLAGS_RELEASE " \
    ${CMAKE_ASM_FLAGS_RELEASE} \
    -DNDEBUG \
    -D__STARTUP_CLEAR_BSS \
    -mcpu=cortex-m33 \
    -mthumb \
    ${FPU} \
")
SET(CMAKE_C_FLAGS_DEBUG " \
    ${CMAKE_C_FLAGS_DEBUG} \
    -DDEBUG \
    -DPLATFORM_ID=LVM_LPC55 \
    -DMAX_SAMPLE_SIZE=576 \
    -DCPU_LPC55S69JBD100_cm33_core0 \
    -DPRINTF_ADVANCED_ENABLE=1 \
    -DPRINTF_FLOAT_ENABLE=1 \
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
    -DMCUXPRESSO_SDK \
    -DSDK_I2C_BASED_COMPONENT_USED=1 \
    -DBOARD_USE_CODEC=1 \
    -DCODEC_WM8904_ENABLE \
    -DSD_ENABLED \
    -DDEBUG_CONSOLE_RX_ENABLE=0 \
    -DCASCFG_PLATFORM_FREERTOS \
    -DFSL_OS_SELECTED=SDK_OS_FREERTOS \
    -DFSL_OSA_TASK_ENABLE=1 \
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
    -mcpu=cortex-m33 \
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
SET(CMAKE_C_FLAGS_RELEASE " \
    ${CMAKE_C_FLAGS_RELEASE} \
    -DNDEBUG \
    -DPLATFORM_ID=LVM_LPC55 \
    -DMAX_SAMPLE_SIZE=576 \
    -DCPU_LPC55S69JBD100_cm33_core0 \
    -DPRINTF_ADVANCED_ENABLE=1 \
    -DPRINTF_FLOAT_ENABLE=1 \
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
    -DMCUXPRESSO_SDK \
    -DSDK_I2C_BASED_COMPONENT_USED=1 \
    -DBOARD_USE_CODEC=1 \
    -DCODEC_WM8904_ENABLE \
    -DSD_ENABLED \
    -DDEBUG_CONSOLE_RX_ENABLE=0 \
    -DCASCFG_PLATFORM_FREERTOS \
    -DFSL_OS_SELECTED=SDK_OS_FREERTOS \
    -DFSL_OSA_TASK_ENABLE=1 \
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
    -mcpu=cortex-m33 \
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
SET(CMAKE_CXX_FLAGS_DEBUG " \
    ${CMAKE_CXX_FLAGS_DEBUG} \
    -DDEBUG \
    -DCPU_LPC55S69JBD100_cm33_core0 \
    -DMCUXPRESSO_SDK \
    -DSERIAL_PORT_TYPE_UART=1 \
    -g \
    -O0 \
    -mcpu=cortex-m33 \
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
SET(CMAKE_CXX_FLAGS_RELEASE " \
    ${CMAKE_CXX_FLAGS_RELEASE} \
    -DNDEBUG \
    -DCPU_LPC55S69JBD100_cm33_core0 \
    -DMCUXPRESSO_SDK \
    -DSERIAL_PORT_TYPE_UART=1 \
    -Os \
    -mcpu=cortex-m33 \
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
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG " \
    ${CMAKE_EXE_LINKER_FLAGS_DEBUG} \
    -g \
    -mcpu=cortex-m33 \
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
    --defsym=__heap_size__=0x2000 \
    ${FPU} \
    ${SPECS} \
    -T${ProjDirPath}/LPC55S69_cm33_core0_flash.ld -static \
")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE " \
    ${CMAKE_EXE_LINKER_FLAGS_RELEASE} \
    -mcpu=cortex-m33 \
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
    --defsym=__heap_size__=0x2000 \
    ${FPU} \
    ${SPECS} \
    -T${ProjDirPath}/LPC55S69_cm33_core0_flash.ld -static \
")
