section .data
section .rodata
  FI0  dq  -4502725890764963840
section .bss
section .text
global _start
_start:
  push rbp
  mov rbp, rsp
  mov QWORD [rbp-16], 7
  mov rax, 4632937379169042432
  mov QWORD [rbp-8], rax
  mov rax, 4632937379169042432
  mov QWORD [rbp-0], rax
  movsd xmm0, QWORD [rbp-8]
  ucomisd xmm0, QWORD [rbp-0]
  jp .EL0
  ucomisd xmm0, QWORD [rbp-0]
  je .EL0
.EL0:
  movsd xmm1, QWORD [FI0]
  comisd xmm1, QWORD [rbp-8]
  jnbe .L1
  comisd QWORD [rbp-8], rdx
  jbe .L0
.EL1:
.L1:
  mov QWORD [rbp-16], 27
.L0:
  movsd xmm0, QWORD [rbp-8]
  ucomisd xmm0, QWORD [rbp-0]
  jp .EL2
  ucomisd xmm0, QWORD [rbp-0]
  je .EL2
.EL2:
  ucomisd QWORD [rbp-8], rdx
  jp .EL3
  ucomisd QWORD [rbp-8], rdx
  je .L4
.EL3:
  comisd QWORD [rbp-8], rdx
  jbe .L3
.L4:
  mov QWORD [rbp-16], 27
.L3:
  mov rax, QWORD [rbp-16]
  add rax, 1
  test rax, rax
  jz .L6
  mov rax, QWORD [rbp-16]
  add rax, 1
  mov QWORD [rbp-16], rax
  jmp .L8
.L6:
  mov rax, QWORD [rbp-16]
  neg rax
  mov QWORD [rbp-16], rax
.L8:
  mov rax, QWORD [rbp-16]
  mov rdi, rax
  mov rax, 60
  syscall
  pop rbp
  mov rax, 60
  xor rdi, rdi
  syscall