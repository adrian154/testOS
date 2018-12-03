nasm -f bin bootsector.asm -o bootsector.bin
dd if=bootsector.bin of=disk.img

nasm -f bin bootloader.asm -o bootloader.bin
dd if=bootloader.bin of=disk.img seek=1 bs=512

nasm -f bin pmbootloader.asm -o bootloader.bin
dd if=bootloader.bin of=disk.img seek=4 bs=512