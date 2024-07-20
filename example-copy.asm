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

    mov rax, 60
    mov rdi, rax
    syscall