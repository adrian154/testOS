# Clean builds, images
./clean.sh

# Assemble bootsector, bootloader
nasm -f bin ./bootloader/bootsector.asm -o ../build/bootsector.bin
nasm -f bin ./bootloader/bootloader.asm -o ../build/bootloader.bin

# Assemble protected mode stuff.
nasm -f elf ./kernel/pmbootloader.asm -o ../build/pmbootloader.o
nasm -f elf ./kernel/misc.asm -o ../build/misc_asm.o
nasm -f elf ./kernel/idt.asm -o ../build/idt_asm.o
nasm -f elf ./kernel/exception.asm -o ../build/exception_asm.o
nasm -f elf ./kernel/gdt.asm -o ../build/gdt_asm.o
nasm -f elf ./kernel/irq.asm -o ../build/irq_asm.o
nasm -f elf ./kernel/paging.asm -o ../build/paging_asm.o

export CFLAGS="-ffreestanding -Wall -Wextra -std=gnu11 -g"

# Compile kernel (C).
i686-elf-gcc -c ./kernel/kernel.c -o ../build/kernel.o $CFLAGS
i686-elf-gcc -c ./kernel/textmode.c -o ../build/textmode.o $CFLAGS
i686-elf-gcc -c ./kernel/gdt.c -o ../build/gdt.o $CFLAGS
i686-elf-gcc -c ./kernel/idt.c -o ../build/idt.o $CFLAGS
i686-elf-gcc -c ./kernel/misc.c -o ../build/misc.o $CFLAGS
i686-elf-gcc -c ./kernel/exception.c -o ../build/exception.o $CFLAGS
i686-elf-gcc -c ./kernel/memorymap.c -o ../build/memorymap.o $CFLAGS
i686-elf-gcc -c ./kernel/pic.c -o ../build/pic.o $CFLAGS
i686-elf-gcc -c ./kernel/irq.c -o ../build/irq.o $CFLAGS
i686-elf-gcc -c ./kernel/acpi.c -o ../build/acpi.o $CFLAGS
i686-elf-gcc -c ./kernel/hpet.c -o ../build/hpet.o $CFLAGS
i686-elf-gcc -c ./kernel/pit.c -o ../build/pit.o $CFLAGS
i686-elf-gcc -c ./kernel/serial.c -o ../build/serial.o $CFLAGS
i686-elf-gcc -c ./kernel/ps2.c -o ../build/ps2.o $CFLAGS
i686-elf-gcc -c ./kernel/ps2kb.c -o ../build/ps2kb.o $CFLAGS
i686-elf-gcc -c ./kernel/paging.c -o ../build/paging.o $CFLAGS

# Link kernel binary.
i686-elf-gcc -T ./kernel/linker.ld -o ../build/kernel.bin -ffreestanding -O0 -nostdlib ../build/pmbootloader.o ../build/kernel.o ../build/misc.o ../build/misc_asm.o ../build/textmode.o ../build/idt.o ../build/idt_asm.o ../build/exception.o ../build/exception_asm.o ../build/gdt_asm.o ../build/gdt.o ../build/memorymap.o ../build/irq_asm.o ../build/irq.o ../build/pic.o ../build/acpi.o ../build/hpet.o ../build/serial.o ../build/ps2.o ../build/ps2kb.o ../build/paging.o ../build/paging_asm.o

# Write bootsector, bootloader, and kernel binary
dd if=../build/bootsector.bin of=../img/disk.img
dd if=../build/bootloader.bin of=../img/disk.img seek=1 bs=512
dd if=../build/kernel.bin of=../img/disk.img seek=5 bs=512

# Make a copy of the disk so it can be viewed while QEMU is running.
cp ../img/disk.img ../img/diskcopy.img
