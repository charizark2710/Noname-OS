#ifndef IDT_H
#define IDT_H

#include <stdint.h>

// Offset: A 32-bit value, split in two parts. It represents the address of the entry point of the Interrupt Service Routine.
// Selector: A Segment Selector with multiple fields which must point to a valid code segment in your GDT.
// Gate Type: A 4-bit value which defines the type of gate this Interrupt Descriptor represents. There are five valid type values:
//     0b0101 or 0x5: Task Gate, note that in this case, the Offset value is unused and should be set to zero.
//     0b0110 or 0x6: 16-bit Interrupt Gate
//     0b0111 or 0x7: 16-bit Trap Gate
//     0b1110 or 0xE: 32-bit Interrupt Gate
//     0b1111 or 0xF: 32-bit Trap Gate
// DPL: A 2-bit value which defines the CPU Privilege Levels which are allowed to access this interrupt via the INT instruction. Hardware interrupts ignore this mechanism.
// P: Present bit. Must be set (1) for the descriptor to be valid.
struct idt_desc
{
    uint16_t offset_high; // 16 bits
    uint16_t selector;    // 16 bits
    uint8_t reverse;      // 8 bits
    uint8_t gateType;     // 8 bits
    uint16_t offset_low;  // 16 bits
} __attribute__((packed));

struct idtr_desc
{
    uint16_t limit; // Size of descriptor table -1
    uint32_t base;  // Base address of the start of the interrupt descriptor table
} __attribute__((packed));

void idt_init();
void int21_handler();

extern void idt_load(struct idtr_desc *ptr);
extern void int21h();
extern void no_int();

#endif