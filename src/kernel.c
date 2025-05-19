#include "include/kernel.h"
#include "include/disk.h"
#include "include/ext2.h"

struct page_4gb_chunk *chunk = 0;
void kernel_main()
{
  terminal_init();
  search_and_init();
  kheap_init();
  idt_init();

  // chunk = new_4gb_page(PAGE_WRITABLE | PAGE_PRESENT | PAGE_USER_MODE);

  // switch_page(get_directory(chunk));

  char c[] = "Kernel load successfully! \n";
  print(c);
  void *c_p = &c;
  c_p = 0x0;
  // void *ptr = kmalloc(50);
  // void *ptr2 = kmalloc(50000);

  // kfree(ptr2);
  // void *ptr3 = kmalloc(9000);

  // void *ptr = kmalloc(4096);

  // enable_paging();

  // char *str = (char *)0x3000;
  // memory_map(get_directory(chunk), str, (size_t)ptr | PAGE_PRESENT | PAGE_WRITABLE);
  // str[0] = 'T';
  // str[1] = 'e';
  // str[2] = 's';
  // str[3] = 't';
  // str[4] = '\n';

  // print(str);

  // print(ptr);

  // char buf[512];

  // search_and_init();
  // read_from_disk(get_disk(0), 0, 1, buf);
  init_ext2();

  struct directory *home_dir = create_dir("home", 0, NULL);

  char data[4096];
  for (int i = 0; i < 4096; i++)
  {
    data[i] = 'a';
  }

  struct inode *test_inode = create_inode("os.bin", data, 4096, 0, home_dir);

  struct path_root *p = path_parser("/home/os.bin", false);

  // char *res = read_file(p->inode);
  enable_int();

  init_pci();

  struct disk_streamer *streamer = new_streamer(0);

  seek_streamer(streamer, 0x201);
  unsigned char ch = 0;
  read_streamer(streamer, &ch, 1);

  uint8_t dest_mac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  struct ethernet_package *demo = build_package_rtl8139(dest_mac, "Hi", 2, 0x0806);
  uint16_t ethertype = 0x0806;
  send_package((void *)demo, sizeof(struct ethernet_package) + 2);
  kfree(demo);
}
