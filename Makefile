# all:
# 	nasm -f bin ./boot.asm -o ./boot.bin
# 	dd if=./message.txt >> ./boot.bin
# 	dd if=/dev/zero bs=512 count=1 >> ./boot.bin

build_real:
	nasm -f bin ./src/boot/boot_real_mode.asm -o ./bin/boot_real_mode.bin

build_protected:
	nasm -f bin ./src/boot/boot_protected_mode.asm -o ./bin/boot_protected_mode.bin

clean:
	rm -rf ./bin