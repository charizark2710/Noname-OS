
#include "../include/terminal.h"

uint16_t col;
uint16_t row;
uint16_t *vid_mem;

uint16_t terminal_make_char(char c, char color)
{
  // The VGA text buffer is 16 bits wide, with the first 8 bits being the color
  // and the last 8 bits being the character
  return color << 8 | c;
}

void terminal_put_char(int x, int y, char c, char color)
{
  vid_mem[y * VGA_WIDTH + x] = terminal_make_char(c, color);
}

void print_char(int x, int y, const char *str, char color)
{
  size_t len = strlen(str);
  for (int i = 0; i < len; i++)
  {
    terminal_put_char(x, y, str[i], color);
  }
}

void terminal_init()
{
  col = 0;
  row = 0;
  vid_mem = (uint16_t *)0xb8000; // 0xb8000 is the memory address of the VGA text buffer
  for (int i = 0; i < VGA_WIDTH; i++)
  {
    for (int j = 0; j < VGA_HEIGHT; j++)
    {
      print_char(i, j, " ", 0);
    }
  }
}

void write(char c, uint16_t color)
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

  terminal_put_char(col, row, c, color);
  col++;
  if (col >= VGA_WIDTH)
  {
    col = 0;
    row++;
  }
}

void print(void *c)
{
  for (size_t i = 0; i < strlen(c); i++)
  {
    write(((char *)c)[i], 15);
  }
}
