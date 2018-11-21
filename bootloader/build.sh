nasm -f bin bootsector.asm -o bootsector.bin
dd if=bootsector.bin of=disk.img
