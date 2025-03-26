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

void kernel_main();
void print_char(int x, int y, const char *str, char color);
void write(char c, uint16_t color);
void terminal_init();
void terminal_put_char(int x, int y, char c, char color);
uint16_t terminal_make_char(char c, char color);
void print(char *c);

#endif