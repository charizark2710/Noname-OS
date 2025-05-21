#include "../include/pci.h"
#include "../include/RTL8139.h"

static int count_network_devices = 0;

uint16_t pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t address;
    uint32_t lbus = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint32_t tmp = 0;

    // Create configuration address as per Figure 1
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
                         (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));

    // Write out the address
    outl(PCI_CONFIG_ADDRESS, address);
    // Read in the data
    // (offset & 2) * 8) = 0 will choose the first word of the 32-bit register
    tmp = (uint16_t)((insw(PCI_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xFFFF);
    return tmp;
}

uint32_t pciConfigReadDWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t address;
    uint32_t lbus = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint32_t tmp = 0;

    // Create configuration address as per Figure 1
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
                         (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));

    // Write out the address
    outl(PCI_CONFIG_ADDRESS, address);
    // Read in the data
    tmp = insl(PCI_CONFIG_DATA); // read 32-bit data from the PCI config space
    return tmp;
}

void pciConfigWriteDWord(struct pci_device device, uint8_t offset, uint32_t value)
{
    uint32_t address = ((uint32_t)0x80000000) | ((uint32_t)device.bus << 16) | ((uint32_t)device.device << 11) | ((uint32_t)device.function << 8) | (offset & 0xFC);
    outl(PCI_CONFIG_ADDRESS, address);
    outl(PCI_CONFIG_DATA, value);
}

uint16_t pciCheckVendor(uint8_t bus, uint8_t slot)
{
    uint16_t vendor, device;
    /* Try and read the first configuration register. Since there are no
     * vendors that == 0xFFFF, it must be a non-existent device. */
    if ((vendor = pciConfigReadWord(bus, slot, 0, 0)) != 0xFFFF)
    {
        device = pciConfigReadWord(bus, slot, 0, 2);
    }
    return vendor;
}

// currently only supports bus 0
void init_pci()
{
    uint8_t bus = 0;
    struct pci_device devices[32];
    int device_count = 0;
    for (uint8_t device = 0; device < 32; device++)
    {
        for (uint8_t function = 0; function < 8; function++)
        { // Max functions per device
            uint16_t vendor_id = pciConfigReadWord(bus, device, function, 0x00);
            if (vendor_id == 0xFFFF)
            { // No device found
                continue;
            }

            // Populate the PCI device structure
            devices[device_count].bus = bus;
            devices[device_count].device = device;
            devices[device_count].function = function;
            devices[device_count].vendor_id = vendor_id;
            devices[device_count].device_id = pciConfigReadWord(bus, device, function, 0x02);
            devices[device_count].command = pciConfigReadWord(bus, device, function, 0x04);
            devices[device_count].class_code = pciConfigReadDWord(bus, device, function, 0x08);
            devices[device_count].header_type = pciConfigReadWord(bus, device, function, 0xC);
            devices[device_count].base_addr[0] = pciConfigReadDWord(bus, device, function, 0x10);
            devices[device_count].base_addr[1] = pciConfigReadDWord(bus, device, function, 0x14);
            devices[device_count].base_addr[2] = pciConfigReadDWord(bus, device, function, 0x18);
            devices[device_count].base_addr[3] = pciConfigReadDWord(bus, device, function, 0x1C);
            devices[device_count].base_addr[4] = pciConfigReadDWord(bus, device, function, 0x20);
            devices[device_count].base_addr[5] = pciConfigReadDWord(bus, device, function, 0x24);
            devices[device_count].interrupt_line = pciConfigReadWord(bus, device, function, 0x3C);
            if (devices[device_count].class_code >> 16 >> 8 == ETHERNET_CONTROLLER)
            {
                count_network_devices++;
                setup_RTL8139(devices[device_count]);
            }
            device_count++;
        }
    }
}

int countNetworkDevices()
{
    return count_network_devices;
}
