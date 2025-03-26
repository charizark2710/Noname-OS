#include "include/kernel.h"
#include "include/disk.h"
#include "include/streamer.h"

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

struct page_4gb_chunk *chunk = 0;
void kernel_main()
{
  terminal_init();
  search_and_init();
  kheap_init();
  idt_init();

  chunk = new_4gb_page(PAGE_WRITABLE | PAGE_PRESENT | PAGE_USER_MODE);

  switch_page(get_directory(chunk));

  char c[] = "Kernel load successfully! \n";
  print(c);
  // void *ptr = kmalloc(50);
  // void *ptr2 = kmalloc(50000);

  // kfree(ptr2);
  // void *ptr3 = kmalloc(9000);

  void *ptr = kmalloc(4096);

  enable_paging();

  char *str = (char *)0x3000;
  memory_map(get_directory(chunk), str, (size_t)ptr | PAGE_PRESENT | PAGE_WRITABLE);
  str[0] = 'T';
  str[1] = 'e';
  str[2] = 's';
  str[3] = 't';
  str[4] = '\n';

  print(str);

  print(ptr);

  // char buf[512];

  // search_and_init();
  // read_from_disk(get_disk(0), 0, 1, buf);

  struct path_root * p = path_parser("/home/root/os.bin");
  enable_int();

  struct disk_streamer *streamer = new_streamer(0);

  seek_streamer(streamer, 0x201);
  unsigned char ch = 0;
  read_streamer(streamer, &ch, 1);
}

void print(char *c)
{
  for (size_t i = 0; i < strlen(c); i++)
  {
    write(c[i], 15);
  }
}