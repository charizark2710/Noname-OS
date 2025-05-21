#include "../include/idt.h"
#include "../config.h"
#include "../include/terminal.h"
#include "../include/memory.h"
#include "../include/io.h"

struct idt_desc idt_descriptors[TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;

void idt_zero()
{
    print("Divide by zero error\n");
}

void idt_set(int interrupt_no, void *address)
{
    struct idt_desc *desc = &idt_descriptors[interrupt_no];
    desc->offset_high = (uint32_t)address & 0xFFFF;
    desc->selector = KERNEL_CODE_SELECTOR;
    desc->reverse = 0x00;
    desc->gateType = 0xEE;
    desc->offset_low = (uint32_t)address >> 16;
}

void no_int_handler()
{
    // print("No interrupt handler\n");
    outb(0x20, 0x20);
}

void int21_handler()
{
    print("keyboard press\n");
    outb(0x20, 0x20);
}

void idt_init()
{
    memset(idt_descriptors, 0, sizeof(idt_descriptors));
    idtr_descriptor.limit = sizeof(idt_descriptors) - 1;
    idtr_descriptor.base = (uint32_t)idt_descriptors;

    for (int i = 0; i < TOTAL_INTERRUPTS; i++)
    {
        idt_set(i, no_int);
    }
    idt_set(0, idt_zero);
    idt_set(0x21, int21h);

    // // Load the interrupt descriptor table
    idt_load(&idtr_descriptor);
}
