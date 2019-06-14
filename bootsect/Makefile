all: 
	make stage1.o
	make stage2.o

stage1.o: stage1.s setting.inc
	nasm -felf64 -o stage1.o stage1.s
stage2.o: stage2.s setting.inc
	nasm -o stage2.o stage2.s
	
clean:
	rm stage1.o
	rm stage2.o