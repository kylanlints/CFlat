section .data
section .rodata
section .bss
section .text
global _start
_start:
  push rbp
  mov rbp, rsp
  mov DWORD [rbp-28], 0
  mov QWORD [rbp-24], 0
.LINF0:
  mov DWORD [rbp-28], 1
  mov QWORD [rbp-16], 2
  mov rax, QWORD [rbp-24]
  shr rax, 1
  cmp rax, QWORD [rbp-16]
  jb .L0
.L3:
  mov rax, QWORD [rbp-24]
  cqo
  div QWORD [rbp-16]
  mul QWORD [rbp-16]
  mov rcx, rax
  mov rax, QWORD [rbp-24]
  sub rax, rcx
  cmp rax, 0
  jne .L6
  mov DWORD [rbp-28], 0
  jmp .L0
.L6:
  mov rax, QWORD [rbp-16]
  add rax, 1
  mov QWORD [rbp-16], rax
  mov rax, QWORD [rbp-24]
  shr rax, 1
  cmp rax, QWORD [rbp-16]
  jae .L3
.L0:
  cmp DWORD [rbp-28], 0
  jz .L9
  mov DWORD [rbp-8], 12345
.L9:
  mov rax, QWORD [rbp-24]
  add rax, 1
  mov QWORD [rbp-24], rax
  jmp .LINF0
.B1:
.L12:
  mov DWORD [rbp-4], 5
  cmp DWORD [rbp-4], 6
  je .L12
.B2:
  mov rax, QWORD [rbp-24]
  sub rax, 1
  mov QWORD [rbp-24], rax
  mov rax, QWORD [rbp-24]
  mov rdi, rax
  mov rax, 60
  syscall
  pop rbp
  mov rax, 60
  xor rdi, rdi
  syscall
