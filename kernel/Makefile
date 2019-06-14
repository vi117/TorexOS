all:
	make Kernel
Kernel: early_main.s
	nasm -felf64 -o kernel.o early_main.s
clean:
	rm kernel.o