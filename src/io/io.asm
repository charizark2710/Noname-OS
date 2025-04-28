section .asm

global insb
global insw
global insl
global outb
global outw
global outl

insb:
    push ebp
    mov ebp, esp

    xor eax, eax ; clear eax
    mov edx, [ebp+8] ; port
    in al, dx
    pop ebp
    ret

insw:
    push ebp
    mov ebp, esp

    xor eax, eax ; clear eax
    mov edx, [ebp+8] ; port
    in ax, dx
    pop ebp
    ret

insl:
    push ebp
    mov ebp, esp

    xor eax, eax ; clear eax
    mov edx, [ebp+8] ; port
    in eax, dx
    pop ebp
    ret

outb:
    push ebp
    mov ebp, esp

    mov edx, [ebp+8] ; port
    mov eax, [ebp+12] ; val
    out dx, al

    pop ebp
    ret

outw:
    push ebp
    mov ebp, esp

    mov edx, [ebp+8] ; port
    mov eax, [ebp+12] ; val
    out dx, ax

    pop ebp
    ret

outl:
    push ebp
    mov ebp, esp

    mov edx, [ebp+8] ; port
    mov eax, [ebp+12] ; val
    out dx, eax

    pop ebp
    ret
