nasm -f bin bootsector.asm -o bootsector.bin
dd if=bootsector.bin of=disk.img

nasm -f bin bootloader.asm -o bootloader.bin
dd if=bootloader.bin of=disk.img seek=1 bs=512

nasm -f bin pmbootloader.asm -o pmbootloader.bin
dd if=pmbootloader.bin of=disk.img seek=4 bs=512







#i686-elf-gcc -c ctest.c -o ctest.o -std=gnu99 -ffreestanding -Wall -Wextra -g

#i686-elf-gcc -T link.ld -o kernel.bin -ffreestanding -O0 -nostdlib ctest.o
