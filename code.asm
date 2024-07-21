section .data
section .rodata
  FI0  dq  -4502725890764963840
section .bss
section .text
global _start
_start:
  push rbp
  mov rbp, rsp
  mov QWORD [rbp-24], 15
  mov rdx, 181238019231
  mov QWORD [rbp-16], rdx
  mov rax, QWORD [rbp-16]
  cqo
  idiv QWORD [rbp-24]
  imul rax, QWORD [rbp-16]
  imul rax, QWORD [rbp-24]
  mov rcx, rax
  mov rax, 99
  sub rax, rcx
  mov QWORD [rbp-24], rax
  mov rax, 4632937379169042432
  mov QWORD [rbp-8], rax
  mov rax, 4632937379169042432
  mov QWORD [rbp-0], rax
  movsd xmm0, QWORD [rbp-8]
  ucomisd xmm0, QWORD [rbp-0]
  jnp .L0
  ucomisd xmm0, QWORD [rbp-0]
  je .L1
  movsd xmm1, QWORD [FI0]
  comisd xmm1, QWORD [rbp-8]
  jbe .L0
.L1:
  mov QWORD [rbp-24], 27
.L0:
  mov rax, QWORD [rbp-24]
  add rax, 1
  test rax, rax
  jz .L3
  mov rax, QWORD [rbp-24]
  add rax, 1
  mov QWORD [rbp-24], rax
  jmp .L5
.L3:
  mov rax, QWORD [rbp-24]
  neg rax
  mov QWORD [rbp-24], rax
.L5:
  mov rax, QWORD [rbp-24]
  mov rdi, rax
  mov rax, 60
  syscall
  pop rbp
  mov rax, 60
  xor rdi, rdi
  syscall