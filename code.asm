section .data
section .rodata
section .bss
section .text
global _start
_start:
  push rbp
  mov rbp, rsp
  mov QWORD [rbp-0], 1
  mov rax, QWORD [rbp-0]
  mov rdi, rax
  mov rax, 60
  syscall
  pop rbp
  mov rax, 60
  xor rdi, rdi
  syscall
