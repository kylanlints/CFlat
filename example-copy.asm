section .rodata
    buffer: db "hello world", 10
    len equ $-buffer

section .text
global _start
_start:
    mov rax, 1
    mov rdi, 1
    mov rsi, buffer
    mov rdx, len
    syscall

    ucomiss xmm0, xmm1

    mov rax, 60
    mov rdi, rax
    syscall
