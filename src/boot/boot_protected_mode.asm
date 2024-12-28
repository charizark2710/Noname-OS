; ORG 0x7c00
ORG 0
BITS 16
_start:
    jmp short start
    nop

times 33 db 0 ; set 33 byte to 0 (33 is the sum of all parameter block in FAT) so that this can start on any 16 bits device

start:
    jmp 0x7c0:post_start ; jump code segment to 0x7c0 address

; [esp+4]: index
; [esp+8]: base
; [esp+12]: limit
; [esp+16]: access byte
; [esp+20]: gran
set_gdt:
    push ebp               ; Save the base pointer
    mov ebp, esp           ; Set the stack frame

    mov bx, [ebp+4]       ; Load index
    shl bx, 3             ; Multiply index by 8 (each entry is 8 bytes)
    add bx, gdt_entries   ; Calculate the address of the GDT entry

    mov eax, [ebp+8]      ; Load base address
    mov ecx, [ebp+12]     ; Load limit
    mov dx, [ebp+16]      ; Load access byte
    mov si, [ebp+20]      ; Load granularity

    ; Set the base address (first 16 bits)
    mov [bx+2], ax

    ; Set the base address (next 8 bits)
    shr eax, 16
    and eax, 0xFF
    mov [bx+4], al

    ; Set the base address (last 8 bits)
    shr eax, 8
    and eax, 0xFF
    mov [bx+7], al

    ; Set the access byte
    mov [bx+5], dx

    ; Set the segment limit (first 16 bits)
    mov [bx], cx

    ; Set the granularity and limit (last 4 bits)
    shr ecx, 16
    and ecx, 0x0F
    and si, 0xF0
    or si, cx
    mov [bx+6], si

    pop ebp                ; Restore the base pointer
    ret

post_start:
    cli ; clean interrupt
    xor eax, eax
    mov ax, 0x7c0
    ; move data segment and extra segment to 0x7c0
    ; if not do this, the bios may assume it is 0x000
    mov ds, ax
    mov es, ax
    ; set stack segment to 0x00 and its pointer to 0x7c0 (same with ds and es)
    ; specifies where the stack will start.
    mov ax, 0x00
    mov ss, ax
    mov sp, 0x7c00
    sti ; enable interrupt

    ; null segment
    push 0
    push 0
    push 0
    push 0
    push 0
    call set_gdt
    add esp, 20

    ; kernel code segment
    push 0xCF
    push 0x9A
    push dword 0xFFFFFFFF
    push dword 0
    push 1
    call set_gdt
    add esp, 20

    ; kernel data segment
    push 0xCF
    push 0x92
    push dword 0xFFFFFFFF
    push dword 0
    push 2
    call set_gdt
    add esp, 20

    ; user code segment
    push 0xCF
    push 0xFA
    push dword 0xFFFFFFFF
    push dword 0
    push 3
    call set_gdt
    add esp, 20
    
    ; user data segment
    push 0xCF
    push 0xF2
    push dword 0xFFFFFFFF
    push dword 0
    push 4
    call set_gdt
    add esp, 20

gdt_entries:
    times 40 db 0

gdt_entries_end:

gdt_descriptor:
    dw gdt_entries_end - gdt_entries - 1 ; Limit (size of GDT - 1)
    dd gdt_entries                      ; Base address of the GDT

.load_protected:
    cli
    ; Load the GDT:
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    JMP 0x08:load32 ; 0x08 is a stand-in for your code segment

[BITS 32]
load32:
   ; Reload data segment registers:
    mov   ax, 0x10 ; 0x10 is a stand-in for your data segment
    mov   DS, ax
    mov   ES, ax
    mov   FS, ax
    mov   GS, ax
    mov   SS, ax
    mov ebp, 0x00200000
    mov esp, ebp
    ; enable A20
    in al, 0x92
    or al, 2
    out 0x92, al
    jmp $

times 510-($ - $$) db 0
dw 0xAA55 ; mark at the end of boot sector
