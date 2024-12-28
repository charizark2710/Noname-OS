# all:
# 	nasm -f bin ./boot.asm -o ./boot.bin
# 	dd if=./message.txt >> ./boot.bin
# 	dd if=/dev/zero bs=512 count=1 >> ./boot.bin
FILES = ./build/kernel.asm.o

all_protected: build_protected ./bin/kernel.bin
	rm -rf ./bin/os.bin
	dd if=./bin/boot_protected_mode.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=512 count=10 >> ./bin/os.bin

./bin/kernel.bin: $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o ./build/kernel_full.o
	i686-elf-gcc -T ./src/linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernel_full.o

build_protected: ./src/boot/boot_protected_mode.asm
	mkdir -p ./bin
	nasm -f bin ./src/boot/boot_protected_mode.asm -o ./bin/boot_protected_mode.bin

./build/kernel.asm.o: ./src/kernel.asm
	nasm -f elf -g ./src/kernel.asm -o ./build/kernel.asm.o

build_real:
	mkdir -p ./bin
	nasm -f bin ./src/boot/boot_real_mode.asm -o ./bin/boot_real_mode.bin
	dd if=./message.txt >> ./bin/boot_real_mode.bin
	dd if=/dev/zero bs=512 count=1 >> ./bin/boot_real_mode.bin

clean:
	rm -rf ./bin/*
	rm -rf ./build/*