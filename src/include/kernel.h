#ifndef KERNEL_H
#define KERNEL_H
#define VGA_WIDTH 80
#define VGA_HEIGHT 20

#include <stddef.h>
#include <stdint.h>
#include "kheap.h"
#include "page.h"
#include "idt.h"
#include "disk.h"
#include "string.h"
#include "path_parser.h"
#include "streamer.h"
#include "ext2.h"
#include "terminal.h"
#include "pci.h"

void kernel_main();

#endif