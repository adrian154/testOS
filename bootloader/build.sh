nasm -f bin bootsector.asm -o bootsector.bin
dd if=bootsector.bin of=disk.img

nasm -f bin bootloader.asm -o bootloader.bin
dd if=bootloader.bin of=disk.img seek=1 count=1 bs=512