#include "../include/RTL8139.h"

struct Descriptor *Rx_Descriptors = (struct Descriptor *)0x100000; /* 1MB Base Address of Rx Descriptors */
struct Descriptor *Tx_Descriptors = (struct Descriptor *)0x200000; /* 2MB Base Address of Tx Descriptors */
unsigned long rx_pointer = 0;
unsigned long tx_pointer = 0;
unsigned volatile long package_recieved_ack = 0;
unsigned volatile long package_send_ack = 0;
unsigned long num_of_tx_descriptors = 1024;
volatile bool tx_success = false;
uint32_t bar0;
uint8_t mac_address[6];
void setup_rx_descriptors()
{
    /* rx_buffer_len is the size (in bytes) that is reserved for incoming packets */
    int i;
    for (i = 0; i < 10; i++) /* num_of_rx_descriptors can be up to 1024 */
    {
        unsigned long rx_buffer_len = 5 * 1024 * 1024; // 5M
        unsigned long packet_buffer_address = (unsigned long)kmalloc(rx_buffer_len);
        if (i == (1024 - 1)) /* Last descriptor? if so, set the EOR bit */
            Rx_Descriptors[i].command = (OWN | EOR | (rx_buffer_len & 0x3FFF));
        else
            Rx_Descriptors[i].command = (OWN | (rx_buffer_len & 0x3FFF));
        /* VLAN adjustments are not part of this guide at the moment - leave as zeros for normal operation */
        Rx_Descriptors[i].low_buf = (unsigned int)packet_buffer_address; /* This is where the packet data will go */
        /* If you are programming for a 64-bit OS, put the high memory location in the 'high_buf' descriptor area */
    }
}
void int_RTL8139_handler()
{
    uint8_t io_base = bar0;
    uint16_t status = insb(io_base + 0x3e);
    outb(io_base + 0x3E, 0x5);

    if (status & 0x04)
    {
        print("Packet sent\n");
    }
    if (status & 0x01)
    {
        // qemu_printf("Received packet\n");
        //  receive_packet();
    }

    outb(0x20, 0x20);
}

uint8_t tx_cur = 0;
void *rtl_send_package(void *package, int len)
{
    uint8_t TSAD_array[4] = {0x20, 0x24, 0x28, 0x2C}; // Offset for TSAD (Transmit Start Address Descriptor)
    uint8_t TSD_array[4] = {0x10, 0x14, 0x18, 0x1C};  // Offset for TSD (Transmit Status Descriptor)

    // Wait for the current descriptor to be available (OWN bit must be cleared)
    // Here, we read the TSD to check the status. The OWN bit is typically the least significant bit (0x01)
    while (insl((long)(bar0 + TSD_array[tx_cur])) & 0x01)
    {
        // Wait for the NIC to finish processing the current descriptor
    }

    // Write the packet address to the corresponding TSAD (Transmitter Start Address Descriptor)
    outl((long)(bar0 + TSAD_array[tx_cur]), (long)package);

    // Write the packet length to the corresponding TSD (Transmitter Status Descriptor)
    // Set bit 13 (OWN bit) to zero to start the transfer
    outl((long)(bar0 + TSD_array[tx_cur]), len & ~(1 << 13));
    // Move to the next descriptor in the circular buffer
    tx_cur++;
    if (tx_cur > 3)
    {
        tx_cur = 0; // Wrap around to the first descriptor
    }
    return 0;
}

struct ethernet_package *build_package_rtl8139(uint8_t dst_mac_addr[6],
                                               void *data, int len, uint16_t type)
{
    struct ethernet_package *package = kmalloc(sizeof(struct ethernet_package) + len);
    void *frame_data = (void *)package + sizeof(struct ethernet_package);
    memcpy(package->dst_mac_addr, dst_mac_addr, 6);
    memcpy(package->src_mac_addr, mac_address, 6);
    package->type = htons(type);
    memcpy(frame_data, data, len);
    return package;
}

char *setup_RTL8139(struct pci_device pci_device)
{
    idt_set(PIC_MASTER_START + pci_device.interrupt_line, int_RTL8139_handler);
    print("Found RTL8139\n");

    uint8_t bar_type = pci_device.base_addr[0] & 0x1;
    bar0 = pci_device.base_addr[0] & 0xFFFFFFFC;
    if (bar_type == 0)
    {
        bar0 = pci_device.base_addr[0] & 0xFFFFFFF0;
    }

    // enable bus mastering
    uint32_t cmd = pci_device.command;
    cmd |= (1 << 2);
    pciConfigWriteDWord(pci_device, 0x4, cmd);

    // uint8_t cfg1 = insb(bar0 + 0x52);
    // cfg1 |= (1 << 4); // power on
    // cfg1 |= (1 << 3); // enable MMIO
    // // cfg1 &= ~(1 << 2);       // disable IO
    // outb(bar0 + 0x54, cfg1); // final write
    // pci_device.is_mmio = true;
    // TODO: should mapping bar0 to VM
    outb(bar0 + 0x52, 0x0);
    outb(bar0 + 0x37, 0x10); /*set the Reset bit (0x10) to the Command Register (0x37)*/

    while ((insb(bar0 + 0x37) & 0x10) != 0)
    {
    };
    // uint32_t mmio_addr = pci_device.base_addr[1] & 0xFFFFFFF0;

    outl(bar0 + 0x44, 0xf | (1 << 7));
    outb(bar0 + 0x37, 0x0C); /* Enable Rx/Tx in the Command register */

    for (int i = 0; i < 6; i++)
    {
        mac_address[i] = insb(bar0 + i); /*bar0 is the base address obtainable from the PCI device configuration space.*/
    }

    // outw(bar0 + 0x3C, 0x0005);
    // setup_rx_descriptors();

    // uint16_t status = insw(bar0 + 0x3e);
    // outw(bar0 + 0x3E, 0x05);
    // outb(bar0 + 0x50, 0xC0);       /* Unlock config registers */
    // outl(bar0 + 0x44, 0x0000E70F); /* RxConfig = RXFTH: unlimited, MXDMA: unlimited, AAP: set (promisc. mode set) */
    // outb(bar0 + 0x37, 0x04);       /* Enable Tx in the Command register, required before setting TxConfig */
    // outl(bar0 + 0x40, 0x03000700); /* TxConfig = IFG: normal, MXDMA: unlimited */
    // outw(bar0 + 0xDA, 0x1FFF);     /* Max rx packet size */
    // outb(bar0 + 0xEC, 0x3B);       /* max tx packet size */

    // /* offset 0x20 == Transmit Descriptor Start Address Register
    //    offset 0xE4 == Receive Descriptor Start Address Register

    //    Again, these are *physical* addresses. This code assumes physical==linear, this is
    //    typically not the case in real world kernels
    // */
    // outl(bar0 + 0x20, (unsigned long)&Tx_Descriptors[0]); /* Tell the NIC where the first Tx descriptor is. NOTE: If writing a 64-bit address, split it into two 32-bit writes.*/
    // outl(bar0 + 0xE4, (unsigned long)&Rx_Descriptors[0]); /* Tell the NIC where the first Rx descriptor is. NOTE: If writing a 64-bit address, split it into two 32-bit writes.*/

    // outb(bar0 + 0x50, 0x00); /* Lock config registers */
    func_ptr_t send_func = &rtl_send_package;
    register_func(send_func);
}

uint8_t *get_mac_addr()
{
    return mac_address;
}
