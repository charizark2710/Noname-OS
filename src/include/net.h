#ifndef NET_H
#define NET_H

#include <stdint.h>
#include <stdbool.h>
#include "kheap.h"
#include "memory.h"
#include "pci.h"

typedef void *(*func_ptr_t)(void *, int);

struct ethernet_package
{
    uint8_t dst_mac_addr[6];
    uint8_t src_mac_addr[6];
    uint16_t type;
    uint8_t data[];
} __attribute__((packed));

void register_func(func_ptr_t sendFunc);
bool send_package(void *data, int len);
uint16_t htons(uint16_t hostshort);

#endif