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

disk.img: bootsect/stage1.bin bootsect/stage2.bin kernel/kernel.bin
	cat bootsect/stage1.bin bootsect/stage2.bin kernel/kernel.bin > disk.img
	
clean:
	make -C bootsect clean
	make -C kernel clean
	rm disk.img