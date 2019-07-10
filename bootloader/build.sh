# Painful fix 
export FIX="-B /mnt/c/buildto/bin/i686-elf-"

# Assemble bootsector and write to first sector of disk.
nasm -f bin bootsector.asm -o ./build/bootsector.bin
dd if=./build/bootsector.bin of=disk.img

# Assemble and write bootloader.
nasm -f bin bootloader.asm -o ./build/bootloader.bin
dd if=./build/bootloader.bin of=disk.img seek=1 bs=512

# Assemble protected mode stuff.
nasm -f elf pmbootloader.asm -o ./build/pmbootloader.o
nasm -f elf misc.asm -o ./build/misc_asm.o
nasm -f elf idt.asm -o ./build/idt_asm.o
nasm -f elf exception.asm -o ./build/exception_asm.o
nasm -f elf gdt.asm -o ./build/gdt_asm.o
nasm -f elf irq.asm -o ./build/irq_asm.o
nasm -f elf paging.asm -o ./build/paging_asm.o

# Compile kernel (C).
i686-elf-gcc -c kernel.c -o ./build/kernel.o -std=gnu99 -ffreestanding -Wall -Wextra -g $FIX
i686-elf-gcc -c textmode.c -o ./build/textmode.o -std=gnu99 -ffreestanding -Wall -Wextra -g $FIX
i686-elf-gcc -c gdt.c -o ./build/gdt.o -std=gnu99 -ffreestanding -Wall -Wextra -g $FIX
i686-elf-gcc -c idt.c -o ./build/idt.o -std=gnu99 -ffreestanding -Wall -Wextra -g $FIX
i686-elf-gcc -c misc.c -o ./build/misc.o -std=gnu99 -ffreestanding -Wall -Wextra -g $FIX
i686-elf-gcc -c exception.c -o ./build/exception.o -std=gnu99 -ffreestanding -Wall -Wextra -g $FIX
i686-elf-gcc -c memorymap.c -o ./build/memorymap.o -std=gnu99 -ffreestanding -Wall -Wextra -g $FIX
i686-elf-gcc -c pic.c -o ./build/pic.o -std=gnu99 -ffreestanding -Wall -Wextra -g $FIX
i686-elf-gcc -c irq.c -o ./build/irq.o -std=gnu99 -ffreestanding -Wall -Wextra -g $FIX
i686-elf-gcc -c acpi.c -o ./build/acpi.o -std=gnu99 -ffreestanding -Wall -Wextra -g $FIX
i686-elf-gcc -c hpet.c -o ./build/hpet.o -std=gnu99 -ffreestanding -Wall -Wextra -g $FIX
i686-elf-gcc -c pit.c -o ./build/pit.o -std=gnu99 -ffreestanding -Wall -Wextra -g $FIX
i686-elf-gcc -c paging.c -o ./build/paging.o -std=gnu99 -ffreestanding -Wall -Wextra -g $FIX

# Link kernel binary.
i686-elf-gcc -T linktest.ld -o ./build/kernel.bin -ffreestanding -O0 -nostdlib ./build/pmbootloader.o ./build/kernel.o ./build/misc.o ./build/misc_asm.o ./build/textmode.o ./build/idt.o ./build/idt_asm.o ./build/exception.o ./build/exception_asm.o ./build/gdt_asm.o ./build/gdt.o ./build/memorymap.o ./build/irq_asm.o ./build/irq.o ./build/pic.o ./build/acpi.o ./build/hpet.o ./build/pit.o ./build/paging.o ./build/paging_asm.o -fno-use-linker-plugin $FIX

# Write kernel binary.
dd if=./build/kernel.bin of=disk.img seek=4 bs=512

# Make a copy of the disk so it can be viewed while QEMU is running.
cp disk.img diskcopy.img
