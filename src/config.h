#ifndef CONFIG_H
#define CONFIG_H

#define TOTAL_INTERRUPTS 0x200 // 512
#define KERNEL_CODE_SELECTOR 0x08
#define KERNEL_DATA_SELECTOR 0x10

#define HEAP_SIZE_TOTAL 1024 * 1024 * 100 // 100 MB, currently dont care about ram, only need 100mb for now
#define HEAP_BLOCK_SIZE 4096
#define HEAP_ADDRESS 0x00007E00
#define HEAP_START 0x01000000

#endif