# all:
# 	nasm -f bin ./boot.asm -o ./boot.bin
# 	dd if=./message.txt >> ./boot.bin
# 	dd if=/dev/zero bs=512 count=1 >> ./boot.bin

# build_real:
# 	mkdir -p ./bin
# 	nasm -f bin ./src/boot/boot_real_mode.asm -o ./bin/boot_real_mode.bin
# 	dd if=./message.txt >> ./bin/boot_real_mode.bin
# 	dd if=/dev/zero bs=512 count=1 >> ./bin/boot_real_mode.bin

FILES = ./build/kernel.asm.o ./build/kernel.o ./build/idt.asm.o ./build/idt.o ./build/memory.o
INCLUDES = -I ./src/include
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all_protected: build_protected ./bin/kernel.bin
	dd if=./bin/boot_protected_mode.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=512 count=100 >> ./bin/os.bin

./bin/kernel.bin: $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o ./build/kernel_full.o
	i686-elf-gcc $(FLAGS) -T ./src/linker.ld ./build/kernel_full.o -o ./bin/kernel.bin

build_protected: ./src/boot/boot_protected_mode.asm
	mkdir -p ./bin
	nasm -f bin ./src/boot/boot_protected_mode.asm -o ./bin/boot_protected_mode.bin

./build/kernel.asm.o: ./src/kernel.asm
	nasm -f elf -g ./src/kernel.asm -o ./build/kernel.asm.o

./build/kernel.o: ./src/kernel.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/kernel.c -o ./build/kernel.o

./build/idt.asm.o: ./src/idt.asm
	nasm -f elf -g ./src/idt.asm -o ./build/idt.asm.o

./build/idt.o: ./src/idt.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/idt.c -o ./build/idt.o

./build/memory.o: ./src/memory.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/memory.c -o ./build/memory.o

clean:
	rm -rf ./bin/*
	rm -rf ./build/*