cmake_minimum_required ( VERSION 3.0 )
project("Torex")

#function(cat IN_FILE OUT_FILE)
#  file(READ ${IN_FILE} CONTENTS)
#  file(APPEND ${OUT_FILE} "${CONTENTS}")
#endfunction()

message ( ${CMAKE_PROJECT_NAME} )

enable_language(ASM_NASM)
enable_testing()

add_subdirectory(lib)

#일단 true
set ( CMAKE_BUILD_TYPE <Debug|Release> )

set(CMAKE_VERBOSE_MAKEFILE true )
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -fno-stack-protector -g -fno-use-cxa-atexit -fno-threadsafe-statics")
set(CMAKE_ASM_NASM_OBJECT_FORMAT "elf64")
set(CMAKE_ASM_NASM_FLAGS "-g")
set(CMAKE_EXE_LINKER_FLAGS "-nostdlib -T kernel.ld -Xlinker -Map=output.map")

set(CXX_LIB_SRC_FILE 
"stringutil.cpp"
"raw_ostream.cpp"
"datastruc/com_string.cpp"
 )

set(TEMP "")
foreach(list_arg ${CXX_LIB_SRC_FILE})
    set(TEMP "${TEMP}" "lib/src/${list_arg}")
endforeach(list_arg)
set(CXX_LIB_SRC_FILE ${TEMP})
unset(TEMP)

set(CXX_SRC_FILE  
"debug/debug.cpp"
"debug/logger.cpp"
"acpi/rsdt.cpp"
"memory/memscan.cpp" 
"memory/mm.cpp"
"memory/slub.cpp"
"memory/buddy.cpp"
"memory/pageframe.cpp"
"vga/cursor.cpp" 
"vga/vgaout.cpp"
"arch/mminit.cpp"
"arch/interrupt.cpp"
"keyboard/scancode.cpp"
"drivers/ps2/keyboard.cpp"
"drivers/timer/cmos.cpp"
"drivers/timer/pit.cpp"
"pic8259/pic8259.cpp"
"process/pid.cpp"
"process/idletask.cpp"
"process/process.cpp"
"process/scheduler.cpp"
"process/simple_scheduler.cpp"
)

set(CXX_SRC_FILE ${CXX_SRC_FILE} "main.cpp")

set(TEMP "")
foreach(list_arg ${CXX_SRC_FILE})
    set(TEMP "${TEMP}" "src/${list_arg}")
endforeach(list_arg)
set(CXX_SRC_FILE ${TEMP})

set(ASM_SRC_FILE "src/arch/early_main.nasm" "src/arch/ISR.nasm" "src/arch/switching.nasm")

include_directories("include/" "lib/include")

add_executable(kernel.elf ${CXX_SRC_FILE} ${ASM_SRC_FILE} ${CXX_LIB_SRC_FILE})

add_custom_command(TARGET kernel.elf POST_BUILD
    COMMAND objcopy --only-keep-debug kernel.elf kernel.sym
    COMMAND objcopy --strip-all kernel.elf
    COMMAND objcopy -O binary kernel.elf kernel.bin
    COMMAND python "${PROJECT_BINARY_DIR}/tools/padding.py" kernel.bin
)