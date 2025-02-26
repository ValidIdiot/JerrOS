CC = gcc
LD = ld
NASM = nasm
CFLAGS = -m32 -ffreestanding -fno-stack-protector -fno-pie -I./include
LDFLAGS = -m elf_i386 -T linker.ld -L/usr/lib/ -I/usr/include

SRC = src/boot.s src/idt.s src/kernel.c src/keyboard.c src/jsh.c src/fs.c src/curses.c src/sd.c
OBJ = obj/boot.o obj/idt.o obj/kernel.o obj/keyboard.o obj/jsh.o obj/fs.o obj/curses.o obj/sd.o

KERNEL = kernel.bin
ISO = JerrOS.iso


all: $(ISO)


obj/%.o: ./src/%.s
	$(NASM) -f elf32 $< -o $@

obj/%.o: ./src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL): $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $(OBJ)

$(ISO): $(KERNEL)
	mkdir -p iso/boot/grub
	cp $(KERNEL) iso/boot/kernel.bin
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'menuentry "JerrOS" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/kernel.bin' >> iso/boot/grub/grub.cfg
	echo '  boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) iso

clean:
	rm -rf obj $(KERNEL) iso $(ISO)
	mkdir obj