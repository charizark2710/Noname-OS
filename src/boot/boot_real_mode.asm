; ORG 0x7c00
ORG 0
BITS 16

_start:
    jmp short start
    nop
    times 33 db 0 ; set 33 byte to 0 (33 is the sum of all parameter block in FAT) so that this can start on any 16 bits device

start:
    jmp 0x7c0:main ; jump code segment to 0x7c0 address

; make interrupt
; zero_interrupt:
;     mov ah, 1
;     mov al, 0
;     add al, ah
;     add al, 48        ; Convert the result to an ASCII character ('2')

;     mov ah, 0eh

;     mov bx, 0x00
;     int 0x10
;     iret ;return to interrupt

; one_interrupt:
;     mov ah, 1
;     mov al, 1
;     add al, ah
;     add al, 48        ; Convert the result to an ASCII character ('2')
    
;     mov ah, 0eh

;     mov bx, 0x00
;     int 0x10
;     iret ;return to interrupt

main:
    cli ; clean interrupt
    mov ax, 0x7c0
    ; move data segment and extra segment to 0x7c0
    ; if not do this, the bios may assume it is 0x000
    mov ds, ax
    mov es, ax

    ; set stack segment to 0x00 and its pointer to 0x7c0 (same with ds and es)
    ; specifies where the stack will start.
    mov ax, 0x00
    mov ss, ax
    mov sp, 0x7c0
    sti ; enable interrupt

    ; mov word[ss:0x00], zero_interrupt
    ; mov word[ss:0x02], 0x7c0
    ; int 0

    ; mov word[ss:0x04], one_interrupt
    ; mov word[ss:0x06], 0x7c0

    ; int 1
    ; this si register will use data from ds (DS:SI)
    ; absolute address: (0x7cc * 16) + message_bits
    ; mov si, message
    ; call print_fn
    
    call read_from_sector_2

    mov si, buffer

    call print_fn

    jmp $

; AH = 02h
; AL = number of sectors to read (must be nonzero)
; CH = low eight bits of cylinder number
; CL = sector number 1-63 (bits 0-5)
; high two bits of cylinder (bits 6-7, hard disk only)
; DH = head number
; DL = drive number (bit 7 set for hard disk)
; ES:BX -> data buffer

; Return:
; CF set on error
; if AH = 11h (corrected ECC error), AL = burst length
; CF clear if successful
; AH = status (see #00234)
; AL = number of sectors transferred (only valid if CF set for some
; BIOSes)
read_from_sector_2:
    mov ah, 02h
    mov al, 1
    mov ch, 0
    mov cl, 2
    mov dh, 0

    mov bx, buffer
    int 0x13
    jc error
    ret

error:
    mov si, error_msg
    call print_fn
    jmp $

error_msg: db 'Failed to load sector', 0

print_fn:
    mov ah, 0eh
    mov bx, 0
.loop:
    lodsb ; Load byte at SI into AL and increment SI
    cmp al, 0
    je .done
    call print_char
    jmp .loop
.done:
    ret

print_char:
    int 0x10
    ret

message: db 'Hello World'

times 510-($ - $$) db 0

dw 0xAA55 ; mark at the end of boot sector

buffer:; buffer at second sector
