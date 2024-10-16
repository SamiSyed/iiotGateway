set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(TOOLCHAIN_PREFIX arm-none-eabi-)
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_SIZE ${TOOLCHAIN_PREFIX}size)

set(CMAKE_EXECUTABLE_SUFFIX_ASM ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf")

set(CMAKE_C_FLAGS
    "-mthumb -mcpu=cortex-m4 -fdata-sections -fmessage-length=0 -ffunction-sections"
)

set(CMAKE_EXE_LINKER_FLAGS
    "-mthumb -mcpu=cortex-m4 -fdata-sections -ffunction-sections -lnosys -lc -lm -specs=nano.specs -Wl,--cref -Wl,--gc-sections"
)

# set(CMAKE_C_FLAGS_DEBUG "-Og -g3 -ggdb")
# set(CMAKE_C_FLAGS_RELEASE "-O2 -ggdb")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
