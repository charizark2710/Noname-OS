#include "../include/idt.h"
#include "../config.h"
#include "../include/kernel.h"
#include "../include/memory.h"

struct idt_desc idt_descriptors[PEACHOS_TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;


void idt_zero()
{
    print("Divide by zero error\n");
}

void idt_set(int interrupt_no, void* address)
{
    struct idt_desc* desc = &idt_descriptors[interrupt_no];
    desc->offset_high = (uint32_t) address & 0x0000ffff;
    desc->selector = KERNEL_CODE_SELECTOR;
    desc->reverse = 0x00;
    desc->gateType = 0xEE;
    desc->offset_low = (uint32_t) address >> 16;
}

void idt_init()
{
    memset(idt_descriptors, 0, sizeof(idt_descriptors));
    idtr_descriptor.limit = sizeof(idt_descriptors) -1;
    idtr_descriptor.base = (uint32_t) idt_descriptors;

    idt_set(0, idt_zero);

    // // Load the interrupt descriptor table
    idt_load(&idtr_descriptor);
}