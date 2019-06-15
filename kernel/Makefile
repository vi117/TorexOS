PROJECT_PATH = /home/coder/project
SRC_PATH = $(PROJECT_PATH)/dev
TOOL_PATH = $(PROJECT_PATH)/tools
CC = g++
CCFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -fno-stack-protector
LD = $(TOOL_PATH)/x86_64-elf-ld
LDFLAGS = -nostdlib -T $(SRC_PATH)/kernel/kernel.ld

all:
	make Kernel
Kernel: kernel.ld
	make early_main.o
	make main.o
	$(LD) $(LDFLAGS) -o kernel.bin early_main.o main.o

early_main.o:
	nasm -f elf64 -o early_main.o early_main.s
main.o: main.cpp
	$(CC) $(CCFLAGS) -c main.cpp -o main.o

clean:
	rm *.o
	rm kernel.bin