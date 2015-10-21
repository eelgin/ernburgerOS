#setup in shell variables
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"
export PATH="$HOME/opt/cross/bin:$PATH"

#test that shell is pointing at cross compiler
$HOME/opt/cross/bin/$TARGET-gcc --version

echo "Validate setup then press enter to continue"
read

#compile assembly
nasm -felf32 boot.asm -o boot.o

#compile kernel.c
i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

#linking the kernel
i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

#set up to make iso image
cp myos.bin isodir/boot/myos.bin
cp grub.cfg isodir/boot/grub/grub.cfg

#create iso image
grub-mkrescue -o myos.iso isodir
