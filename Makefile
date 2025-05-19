# all:
# 	nasm -f bin ./boot.asm -o ./boot.bin
# 	dd if=./message.txt >> ./boot.bin
# 	dd if=/dev/zero bs=512 count=1 >> ./boot.bin

# build_real:
# 	mkdir -p ./bin
# 	nasm -f bin ./src/boot/boot_real_mode.asm -o ./bin/boot_real_mode.bin
# 	dd if=./message.txt >> ./bin/boot_real_mode.bin
# 	dd if=/dev/zero bs=512 count=1 >> ./bin/boot_real_mode.bin

# Files to build
FILES = \
  ./build/kernel.asm.o \
  ./build/kernel.o \
  ./build/terminal.o \
  ./build/idt.asm.o \
  ./build/idt.o \
  ./build/memory.o \
  ./build/io.asm.o \
  ./build/heap.o \
  ./build/kheap.o \
  ./build/page.o \
  ./build/page.asm.o \
  ./build/disk.o \
  ./build/path_parser.o \
  ./build/string.o \
  ./build/streamer.o \
  ./build/ext2.o \
  ./build/pci.o \
  ./build/RTL8139.o \
  ./build/net.o

# Include directories
INCLUDES = -I ./src/include

# Compiler flags
FLAGS = \
  -g \
  -ffreestanding \
  -falign-jumps \
  -falign-functions \
  -falign-labels \
  -falign-loops \
  -fstrength-reduce \
  -fomit-frame-pointer \
  -finline-functions \
  -nostdlib \
  -nostartfiles \
  -nodefaultlibs \
  -Wall \
  -O0 \
  -Iinc

# Build targets
all_protected: build_protected ./bin/kernel.bin
	dd if=/dev/zero bs=1M count=10
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

./build/terminal.o: ./src/terminal/terminal.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/terminal/terminal.c -o ./build/terminal.o

./build/idt.asm.o: ./src/idt/idt.asm
	nasm -f elf -g ./src/idt/idt.asm -o ./build/idt.asm.o

./build/idt.o: ./src/idt/idt.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/idt/idt.c -o ./build/idt.o

./build/memory.o: ./src/memory/memory.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/memory/memory.c -o ./build/memory.o

./build/heap.o: ./src/memory/heap/heap.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/memory/heap/heap.c -o ./build/heap.o

./build/kheap.o: ./src/memory/heap/kheap.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/memory/heap/kheap.c -o ./build/kheap.o

./build/io.asm.o: ./src/io/io.asm
	nasm -f elf -g ./src/io/io.asm -o ./build/io.asm.o

./build/pci.o: ./src/io/pci.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/io/pci.c -o ./build/pci.o

./build/RTL8139.o: ./src/networking/RTL8139.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/networking/RTL8139.c -o ./build/RTL8139.o

./build/net.o: ./src/networking/net.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/networking/net.c -o ./build/net.o

./build/page.asm.o: ./src/memory/page/page.asm
	nasm -f elf -g ./src/memory/page/page.asm -o ./build/page.asm.o

./build/page.o: ./src/memory/page/page.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/memory/page/page.c -o ./build/page.o

./build/disk.o: ./src/disk/disk.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/disk/disk.c -o ./build/disk.o

./build/streamer.o: ./src/disk/streamer.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/disk/streamer.c -o ./build/streamer.o

./build/path_parser.o: ./src/fs/path_parser.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/fs/path_parser.c -o ./build/path_parser.o

./build/string.o: ./src/string/string.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/string/string.c -o ./build/string.o

./build/ext2.o: ./src/fs/ext2.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/fs/ext2.c -o ./build/ext2.o

clean:
	rm -rf ./bin/*
	rm -rf ./build/*