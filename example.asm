section .rodata
    buffer: db 'hello world', 10
    len equ $-buffer
    nums: db '01'

section .bss
    numbuffer: resb 65

section .text
global _start
_start:
    ;mov rax, 1
    ;mov rdi, 1
    ;mov rsi, buffer
    ;mov rdx, len
    ;syscall

    mov rax, 55*2*2*2*2*2-1
    mov rcx, -7714092976278539767
    add rax, 1 ; inc cmd doesnt set CF
    sbb rax, 0
    mul rcx
    shr rdx, 5
    mov rdi, rdx
    call _printbin_reg64

    mov eax, 1879048192
    mov rdx, rax
    mov rcx, 0x92492493
    imul rdx, rcx
    sar rdx, 34
    mov rcx, rdx
    cdq
    mov rax, rcx
    sub rax, rdx
    mov rdi, rax
    call _printbin_reg64

    mov rax, 60
    mov rdi, r10
    syscall

_printbin_reg64:
    mov rax, rdi
    mov ecx, 64
L1:
    mov rdx, 1
    and rdx, rax
    mov dl, BYTE [nums+edx]
    mov BYTE [numbuffer+rcx-1], dl
    shr rax, 1
    loop L1
    mov BYTE [numbuffer+64], 10
    mov rax, 1
    mov rdi, 1
    mov rsi, numbuffer
    mov rdx, 65
    syscall
    ret