; ORG 0x7c00
ORG 0x7c00
BITS 16

CODE_SEG equ 0x08
DATA_SEG equ 0x10

_start:
    jmp short start
    nop

times 33 db 0 ; set 33 byte to 0 (33 is the sum of all parameter block in FAT) so that this can start on any 16 bits device

start:
    jmp 0:post_start ; jump code segment to 0x7c0 address

; gdt_entries:
;     times 40 db 0

; gdt_entries_end:

; gdt_descriptor:
;     dw gdt_entries_end - gdt_entries - 1 ; Limit (size of GDT - 1)
;     dd gdt_entries                      ; Base address of the GDT

; [BITS 32]
post_start:
    cli ; clean interrupt
    mov ax, 0x00
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00

    ; ; null segment
    ; push 0x00
    ; push 0x00
    ; push 0x00
    ; push 0x00
    ; push 0x00
    ; call set_gdt
    ; add esp, 20

    ; ; kernel code segment
    ; push dword 0xCF
    ; push dword 0x9A
    ; push dword 0xFFFFFFFF
    ; push dword 0
    ; push dword 1
    ; call set_gdt
    ; add esp, 20

    ; ; kernel data segment
    ; push dword 0xCF
    ; push dword 0x92
    ; push dword 0xFFFFFFFF
    ; push dword 0
    ; push 2
    ; call set_gdt
    ; add esp, 20

    ; ; user code segment
    ; push dword 0xCF
    ; push dword 0xFA
    ; push dword 0xFFFFFFFF
    ; push dword 0
    ; push 3
    ; call set_gdt
    ; add esp, 20
    
    ; ; user data segment
    ; push dword 0xCF
    ; push dword 0xF2
    ; push dword 0xFFFFFFFF
    ; push dword 0
    ; push 4
    ; call set_gdt
    ; add esp, 20    
    sti ; enable interrupt

; [esp+4]: index
; [esp+8]: base
; [esp+12]: limit
; [esp+16]: access byte
; [esp+20]: gran
; set_gdt:
;     push ebp               ; Save the base pointer
;     mov ebp, esp           ; Set the stack frame

;     mov ebx, [ebp+4]       ; Load index
;     shl ebx, 3             ; Multiply index by 8 (each entry is 8 bytes)
;     add ebx, gdt_entries   ; Calculate the address of the GDT entry

;     mov eax, [ebp+8]      ; Load base address
;     mov ecx, [ebp+12]     ; Load limit
;     mov edx, [ebp+16]      ; Load access byte

;     ; Set the base address (first 16 bits)
;     mov [ebx+2], ax

;     ; Set the base address (next 8 bits)
;     shr eax, 16
;     and eax, 0xFF
;     mov [ebx+4], al

;     ; Set the base address (last 8 bits)
;     shr eax, 8
;     and eax, 0xFF
;     mov [ebx+7], al

;     ; Set the access byte
;     mov [ebx+5], edx

;     ; Set the segment limit (first 16 bits)
;     mov [ebx], cx

;     ; Set the granularity and limit (last 4 bits)
;     mov ax, [ebp+20]      ; Load granularity

;     shr ecx, 16
;     and ecx, 0x0F
;     and ax, 0xF0
;     or ax, cx
;     mov [ebx+6], ax

;     pop ebp                ; Restore the base pointer
;     ret

.load_protected:
    cli
    ; Load the GDT:
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:load32 ; 0x08 is a stand-in for your code segment

; GDT
gdt_start:
gdt_null:
    dd 0x0
    dd 0x0

; offset 0x8
gdt_code:     ; CS SHOULD POINT TO THIS
    dw 0xffff ; Segment limit first 0-15 bits
    dw 0      ; Base first 0-15 bits
    db 0      ; Base 16-23 bits
    db 0x9a   ; Access byte
    db 11001111b ; High 4 bit flags and the low 4 bit flags
    db 0        ; Base 24-31 bits

; offset 0x10
gdt_data:      ; DS, SS, ES, FS, GS
    dw 0xffff ; Segment limit first 0-15 bits
    dw 0      ; Base first 0-15 bits
    db 0      ; Base 16-23 bits
    db 0x92   ; Access byte
    db 11001111b ; High 4 bit flags and the low 4 bit flags
    db 0        ; Base 24-31 bits

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start-1
    dd gdt_start

[BITS 32]
load32:
    mov eax, 1
    mov ecx, 10
    mov edi, 0x0100000
    call ata_lba_read
    jmp CODE_SEG:0x0100000

ata_lba_read:
    mov ebx, eax ; Backup the LBA
    ; Send the highest 8 bits of the lba to hard disk controller
    shr eax, 24
    or eax, 0xE0 ; Select the  master drive
    mov dx, 0x1F6
    out dx, al
    ; Finished sending the highest 8 bits of the lba

    ; Send the total sectors to read
    mov eax, ecx
    mov dx, 0x1F2
    out dx, al
    ; Finished sending the total sectors to read

    ; Send more bits of the LBA
    mov eax, ebx ; Restore the backup LBA
    mov dx, 0x1F3
    out dx, al
    ; Finished sending more bits of the LBA

    ; Send more bits of the LBA
    mov dx, 0x1F4
    mov eax, ebx ; Restore the backup LBA
    shr eax, 8
    out dx, al
    ; Finished sending more bits of the LBA

    ; Send upper 16 bits of the LBA
    mov dx, 0x1F5
    mov eax, ebx ; Restore the backup LBA
    shr eax, 16
    out dx, al
    ; Finished sending upper 16 bits of the LBA

    mov dx, 0x1f7
    mov al, 0x20
    out dx, al

    ; Read all sectors into memory
.next_sector:
    push ecx

; Checking if we need to read
.try_again:
    mov dx, 0x1f7
    in al, dx
    test al, 8
    jz .try_again

; We need to read 256 words at a time
    mov ecx, 256
    mov dx, 0x1F0
    rep insw
    pop ecx
    loop .next_sector
    ; End of reading sectors into memory
    ret

times 510-($ - $$) db 0

dw 0xAA55 ; mark at the end of boot sector
