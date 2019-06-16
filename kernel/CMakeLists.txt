cmake_minimum_required ( VERSION 3.0 )
project("PPKOS_2")

#function(cat IN_FILE OUT_FILE)
#  file(READ ${IN_FILE} CONTENTS)
#  file(APPEND ${OUT_FILE} "${CONTENTS}")
#endfunction()

message ( ${CMAKE_PROJECT_NAME} )

enable_language(ASM_NASM)

#일단 true
set ( CMAKE_BUILD_TYPE <Debug|Release> )

set(CMAKE_VERBOSE_MAKEFILE true )
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -fno-stack-protector")
set(CMAKE_ASM_NASM_OBJECT_FORMAT "elf64")
set(CMAKE_EXE_LINKER_FLAGS "-nostdlib -T kernel.ld")

set(CXX_SRC_FILE main.cpp)
set(ASM_SRC_FILE early_main.nasm)

add_executable(kernel.bin ${CXX_SRC_FILE} ${ASM_SRC_FILE} )

add_custom_command(TARGET kernel.bin POST_BUILD
    COMMAND python "/home/coder/project/dev/kernel/tools/padding.py" kernel.bin
)