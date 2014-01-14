
all: george.multiboot

george.multiboot: george.o start.o threads.o vga.o util.o idt.o isr.o isr-asm.o pic.o pit.o util-asm.o gdt.o gdt-asm.o page.o
	ld -melf_i386 -no-stdlib -N -Ttext 100000 $^ -o $@

start.o: start.s
	as --32 -o $@ $^

isr-asm.o: isr-asm.s
	as --32 -o $@ $^

gdt-asm.o: gdt-asm.s
	as --32 -o $@ $^

util-asm.o: util-asm.s
	as --32 -o $@ $^

%.o: %.c
#	gcc -m32 -c -o $@ $^ -fno-builtin -nostdinc -Wall -Wextra -std=c99 -O2 -g -fgcse-after-reload -finline-functions -fipa-cp-clone -fpredictive-commoning -ftree-loop-distribute-patterns -ftree-vectorize -funswitch-loops
	gcc -m32 -c -o $@ $^ -fno-builtin -nostdinc -Wall -Wextra -std=c99 -O2 -g

george.disk: george.multiboot menu.lst
	./MKDISK.sh

clean:
	rm -f *.o george.multiboot george.disk
