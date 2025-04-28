#ifndef RTL8139_H
#define RTL8139_H
#include "io.h"
#include "kheap.h"
#include "pci.h"
#include "idt.h"
#include "terminal.h"
#include <stdint.h>

char *setup_RTL8139(struct pci_device);

#define OWN (1<<31)
#define EOR (1<<30)
#define FS  (1<<29)
#define LS  (1<<28)
#define LGSEN (1<<27)
#define IPCS (1<<18)
#define UDPCS (1<<17)
#define TCPCS (1<<16)

struct Descriptor
{
    uint32_t command;  /* command/status uint32_t */
    uint32_t vlan;     /* currently unused */
    uint32_t low_buf;  /* low 32-bits of physical buffer address */
    uint32_t high_buf; /* high 32-bits of physical buffer address */
};

struct Package_Received_Descriptor
{
    unsigned long buffersize;
    unsigned long low_buf;
    unsigned long high_buf;
};

struct Package_Send_Descriptor
{
    void *addr;
    uint32_t len;
};

void rtl_send_package(void *base_address, struct Package_Send_Descriptor data);
struct Package_Received_Descriptor rtl_recievePackage();

#endif