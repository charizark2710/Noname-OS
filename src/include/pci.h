#ifndef PCI_H
#define PCI_H

#include <stdint.h>
#include <stdbool.h>
#include "io.h"

#define ETHERNET_CONTROLLER 0x2
#define AHCI_CONTROLLER 0x102

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC
struct pci_device
{
    uint8_t bus;            // Bus number
    uint8_t device;         // Device number
    uint8_t function;       // Function number
    uint16_t vendor_id;     // Vendor ID
    uint16_t device_id;     // Device ID
    uint32_t command;       // command code
    uint32_t class_code;    // Class code
    uint8_t header_type;    // Header type
    uint32_t base_addr[6];  // Base addresses for I/O or memory mapping
    uint8_t interrupt_line; // Interrupt line number
    bool is_mmio;           // Memory-mapped I/O
};

void init_pci();
void pciConfigWriteDWord(struct pci_device device, uint8_t offset, uint32_t value);
uint16_t pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
uint32_t pciConfigReadDWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);

#endif