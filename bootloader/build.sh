# Assemble bootsector and write to first sector of disk.
nasm -f bin bootsector.asm -o bootsector.bin
dd if=bootsector.bin of=disk.img

# Assemble and write bootloader.
nasm -f bin bootloader.asm -o bootloader.bin
dd if=bootloader.bin of=disk.img seek=1 bs=512

# Assemble procted mode stuff.
nasm -f elf pmbootloader.asm -o pmbootloader.o

# Compile kernel (C).
i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -Wall -Wextra -g

# Link kernel binary.
i686-elf-gcc -T linker.ld -o kernel.bin -ffreestanding -O0 -nostdlib pmbootloader.o kernel.o

# Write kernel binary.
dd if=kernel.bin of=disk.img seek=4 bs=512

# Make a copy of the disk so it can be viewed while QEMU is running.
cp disk.img diskcopy.img
