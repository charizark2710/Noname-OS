#include "../include/kernel.h"
#include <stdint.h>

uint16_t col;
uint16_t row;

uint16_t *terminal_init()
{
  col = 0;
  row = 0;
  uint16_t *vid_mem = (uint16_t *)0xb8000; // 0xb8000 is the memory address of the VGA text buffer
  for (int i = 0; i < VGA_WIDTH; i++)
  {
    for (int j = 0; j < VGA_HEIGHT; j++)
    {
      print(i, j, " ", 0, vid_mem);
    }
  }
  return vid_mem;
}

uint16_t terminal_make_char(char c, char color)
{
  // The VGA text buffer is 16 bits wide, with the first 8 bits being the color
  // and the last 8 bits being the character
  return color << 8 | c;
}

void terminal_put_char(int x, int y, char c, char color, uint16_t *vid_mem)
{
  vid_mem[y * VGA_WIDTH + x] = terminal_make_char(c, color);
}

void print(int x, int y, const char *str, char color, uint16_t *vid_mem)
{
  size_t len = strlen(str);
  for (int i = 0; i < len; i++)
  {
    terminal_put_char(x, y, str[i], color, vid_mem);
  }
}

void write(char c, uint16_t color, uint16_t *vid_mem)
{
  if (row >= VGA_HEIGHT || col >= VGA_WIDTH)
  {
    return;
  }

  switch (c)
  {
  case '\n':
    row++;
    col = 0;
    return;
  case '\t':
    col += 4;
    break;
  default:
    break;
  }

  terminal_put_char(col, row, c, color, vid_mem);
  col++;
  if (col >= VGA_WIDTH)
  {
    col = 0;
    row++;
  }
}

size_t strlen(const char *str)
{
  size_t len = 0;
  while (str[len])
  {
    len++;
  }
  return len;
}

void kernel_main()
{
  uint16_t *vid_mem = terminal_init();
  char c[] = "Hello, World!";
  for (size_t i = 0; i < strlen(c); i++)
  {
    write(c[i], 15, vid_mem);
  }
}