PROJECT_PATH = /home/coder/project
SRC_PATH = $(PROJECT_PATH)/dev
TOOL_PATH = $(PROJECT_PATH)/tools
LD = $(TOOL_PATH)/x86_64-elf-ld
LDFLAGS = -nostdlib -T $(SRC_PATH)/kernel/kernel_ldscript.ld
all:
	make Bootloader
	make Kernel
	make disk.img

Bootloader:
	@echo ===== Enter bootloader =====
	make -C bootsect
Kernel:
	make -C kernel

disk.img: bootsect/stage1.o bootsect/stage2.o kernel/kernel.o
	$(LD) $(LDFLAGS) ../dev/bootsect/stage1.o $(SRC_PATH)/bootsect/stage2.o $(SRC_PATH)/kernel/kernel.o -o disk.bin
	
clean:
	make -C bootsect clean
	make -C kernel clean
	rm disk.img