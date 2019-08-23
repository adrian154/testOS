# Assemble bootsector and write to first sector of disk.
nasm -f bin ./bootloader/bootsector.asm -o ./build/bootsector.bin
dd if=./build/bootsector.bin of=./img/disk.img

# Assemble and write bootloader.
nasm -f bin ./bootloader/bootloader.asm -o ./build/bootloader.bin
dd if=./build/bootloader.bin of=./img/disk.img seek=1 bs=512

# Assemble protected mode stuff.
nasm -f elf ./kernel/pmbootloader.asm -o ./build/pmbootloader.o
nasm -f elf ./kernel/misc.asm -o ./build/misc_asm.o
nasm -f elf ./kernel/idt.asm -o ./build/idt_asm.o
nasm -f elf ./kernel/exception.asm -o ./build/exception_asm.o
nasm -f elf ./kernel/gdt.asm -o ./build/gdt_asm.o
nasm -f elf ./kernel/irq.asm -o ./build/irq_asm.o
#nasm -f elf ./kernel/paging.asm -o ./build/paging_asm.o

# Compile kernel (C).
i686-elf-gcc -c ./kernel/kernel.c -o ./build/kernel.o -std=gnu99 -ffreestanding -Wall -Wextra -g
i686-elf-gcc -c ./kernel/textmode.c -o ./build/textmode.o -std=gnu99 -ffreestanding -Wall -Wextra -g
i686-elf-gcc -c ./kernel/gdt.c -o ./build/gdt.o -std=gnu99 -ffreestanding -Wall -Wextra -g
i686-elf-gcc -c ./kernel/idt.c -o ./build/idt.o -std=gnu99 -ffreestanding -Wall -Wextra -g
i686-elf-gcc -c ./kernel/misc.c -o ./build/misc.o -std=gnu99 -ffreestanding -Wall -Wextra -g
i686-elf-gcc -c ./kernel/exception.c -o ./build/exception.o -std=gnu99 -ffreestanding -Wall -Wextra -g
i686-elf-gcc -c ./kernel/memorymap.c -o ./build/memorymap.o -std=gnu99 -ffreestanding -Wall -Wextra -g
i686-elf-gcc -c ./kernel/pic.c -o ./build/pic.o -std=gnu99 -ffreestanding -Wall -Wextra -g
i686-elf-gcc -c ./kernel/irq.c -o ./build/irq.o -std=gnu99 -ffreestanding -Wall -Wextra -g
i686-elf-gcc -c ./kernel/acpi.c -o ./build/acpi.o -std=gnu99 -ffreestanding -Wall -Wextra -g
i686-elf-gcc -c ./kernel/hpet.c -o ./build/hpet.o -std=gnu99 -ffreestanding -Wall -Wextra -g
i686-elf-gcc -c ./kernel/pit.c -o ./build/pit.o -std=gnu99 -ffreestanding -Wall -Wextra -g
#i686-elf-gcc -c ./kernel/paging.c -o ./build/paging.o -std=gnu99 -ffreestanding -Wall -Wextra -g

# Link kernel binary.
i686-elf-gcc -T ./kernel/linker.ld -o ./build/kernel.bin -ffreestanding -O0 -nostdlib ./build/pmbootloader.o ./build/kernel.o ./build/misc.o ./build/misc_asm.o ./build/textmode.o ./build/idt.o ./build/idt_asm.o ./build/exception.o ./build/exception_asm.o ./build/gdt_asm.o ./build/gdt.o ./build/memorymap.o ./build/irq_asm.o ./build/irq.o ./build/pic.o ./build/acpi.o ./build/hpet.o

# Write kernel binary.
dd if=./build/kernel.bin of=./img/disk.img seek=5 bs=512

# Make a copy of the disk so it can be viewed while QEMU is running.
cp ./img/disk.img ./img/diskcopy.img
