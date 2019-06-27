# Assemble bootsector and write to first sector of disk.
nasm -f bin bootsector.asm -o bootsector.bin
dd if=bootsector.bin of=disk.img

# Assemble and write bootloader.
nasm -f bin bootloader.asm -o bootloader.bin
dd if=bootloader.bin of=disk.img seek=1 bs=512

# Assemble procted mode stuff.
nasm -f elf pmbootloader.asm -o pmbootloader.o
nasm -f elf misc.asm -o misc_asm.o
nasm -f elf idt.asm -o idt_asm.o
nasm -f elf isr.asm -o isr_asm.o
nasm -f elf irq.asm -o irq_asm.o

# Compile kernel (C).
i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -Wall -Wextra -g
i686-elf-gcc -c textmode.c -o textmode.o -std=gnu99 -ffreestanding -Wall -Wextra -g
i686-elf-gcc -c idt.c -o idt.o -std=gnu99 -ffreestanding -Wall -Wextra -g
i686-elf-gcc -c misc.c -o misc.o -std=gnu99 -ffreestanding -Wall -Wextra -g
i686-elf-gcc -c isr.c -o isr.o -std=gnu99 -ffreestanding -Wall -Wextra -g

# Link kernel binary.
i686-elf-gcc -T linktest.ld -o kernel.bin -ffreestanding -O0 -nostdlib pmbootloader.o kernel.o misc.o misc_asm.o textmode.o idt.o idt_asm.o isr.o isr_asm.o irq_asm.o irq.o
# Write kernel binary.
dd if=kernel.bin of=disk.img seek=4 bs=512

# Make a copy of the disk so it can be viewed while QEMU is running.
cp disk.img diskcopy.img
