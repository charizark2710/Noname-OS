#ifndef NET_H
#define NET_H

#include <stdint.h>
#include <stdbool.h>
#include "kheap.h"
#include "memory.h"
#include "pci.h"

typedef void *(*func_ptr_t)(void *, int);
typedef void *(*func_ptr_b)(uint8_t[6], void *, int, uint16_t);

void register_func(func_ptr_b buildFunc, func_ptr_t sendFunc);
bool send_package(uint8_t *dst, void *data, int len, uint16_t type);
uint16_t htons(uint16_t hostshort);

#endif