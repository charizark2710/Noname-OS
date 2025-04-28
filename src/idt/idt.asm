section .asm

extern int21_handler
extern int_RTL8139_handler
extern no_int_handler

global idt_load
global int21h
global int_RTL8139
global no_int
global enable_int
global disable_int

enable_int:
    sti
    ret

disable_interrupts:
    cli
    ret

idt_load:
    push ebp
    mov ebp, esp

    mov ebx, [ebp+8] ; idt address
    lidt [ebx]
    pop ebp
    ret

int21h:
    cli
    pushad
    call int21_handler
    popad
    sti
    iret

int_RTL8139:
    cli
    pushad
    call int_RTL8139_handler
    popad
    sti
    iret

no_int:
    cli
    pushad
    call no_int_handler
    popad
    sti
    iret
