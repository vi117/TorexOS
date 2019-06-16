all: 
	make stage1.bin
	make stage2.bin

stage1.bin: stage1.s setting.inc
	nasm -o stage1.bin stage1.s
stage2.bin: stage2.s setting.inc ../kernel/kernel.bin
	nasm -dKernelSize=$(KERNELSIZE) -o stage2.bin stage2.s 
	
clean:
	rm stage1.bin
	rm stage2.bin