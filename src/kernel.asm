[BITS 32]
global _start

extern kernel_main

CODE_SEG equ 0x08
DATA_SEG equ 0x10

_start:
   ; Reload data segment registers:
    mov   ax, DATA_SEG ; 0x10 is a stand-in for your data segment
    mov   ds, ax
    mov   es, ax
    mov   fs, ax
    mov   gs, ax
    mov   ss, ax
    mov ebp, 0x00200000
    mov esp, ebp
    ; enable A20
    in al, 0x92
    or al, 2
    out 0x92, al

    ; Remap master PIC
    mov al, 00010001b
    out 0x20, al ; Tell master PIC

    mov al, 0x20 ; interrupt 0x20 is where master IRC start
    out 0x21, al ; Set master PIC offset

    mov al, 00000001b
    out 0x21, al
    ; End remap master PIC


    ; Remap slave PIC
    mov al, 00010001b
    out 0xA0, al ; Tell slave PIC

    mov al, 0x28 ; interrupt 0x28 is where slave IRC start
    out 0x28, al ; Set slave PIC offset

    mov al, 00000001b
    out 0xA1, al

    call kernel_main

    jmp $

times 512-($ - $$) db 0
