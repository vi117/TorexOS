all:
	make Kernel
Kernel: early_main.s
	nasm -o kernel.bin early_main.bin
clean:
	rm kernel.bin