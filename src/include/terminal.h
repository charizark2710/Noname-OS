#ifndef TERMINAL_H
#define TERMINAL_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 20

#include <stdint.h>
#include "string.h"

// void print_char(int x, int y, const char *str, char color);
void write(char c, uint16_t color);
void terminal_init();
// void terminal_put_char(int x, int y, char c, char color);
// uint16_t terminal_make_char(char c, char color);
void print(char *c);

#endif