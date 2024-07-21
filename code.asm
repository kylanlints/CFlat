section .data
section .rodata
section .bss
section .text
global _start
_start:
  push rbp
  mov rbp, rsp
  mov QWORD [rbp-0], 15
  mov rax, QWORD [rbp-0]
  shl rax, 3
  add rax, 10
  mov QWORD [rbp-0], rax
  mov rax, QWORD [rbp-0]
  add rax, 1
  test rax, rax
  jz .L0
  mov rax, QWORD [rbp-0]
  add rax, 1
  mov QWORD [rbp-0], rax
  jmp .L2
.L0:
  mov rax, QWORD [rbp-0]
  neg rax
  mov QWORD [rbp-0], rax
.L2:
  mov rax, QWORD [rbp-0]
  mov rdi, rax
  mov rax, 60
  syscall
  pop rbp
  mov rax, 60
  xor rdi, rdi
  syscall