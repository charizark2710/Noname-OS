section .asm

extern int21_handler
extern no_int_handler

global idt_load
global int21h
global no_int

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

no_int:
    cli
    pushad
    call no_int_handler
    popad
    sti
    iret
