section .data
section .rodata
  FI0  dq  4721981012836684716
  FI1  dq  -4502725890764963840
section .bss
section .text
global _start
_start:
;s
  push rbp
  mov rbp, rsp
;s
  mov QWORD [rbp-24], 7
;s
  mov QWORD [rbp-16], 8
;s
  cmp QWORD [rbp-24], 8
  sete al
  mov QWORD [rbp-16], rax
;s
  mov rax, 4632937379169042432
  mov QWORD [rbp-8], rax
;s
  mov rax, 4632937379169042432
  mov QWORD [rbp-0], rax
;s
  movsd xmm1, QWORD [FI0]
  comisd xmm1, QWORD [rbp-8]
  jbe .L0
;s
;s
  mov rax, 4632937379169042432
  mov QWORD [rbp-8], rax
.L0:
;s
  movsd xmm0, QWORD [rbp-8]
  ucomisd xmm0, QWORD [rbp-0]
  jp .EL0
  ucomisd xmm0, QWORD [rbp-0]
  je .L4
.EL0:
  movsd xmm1, QWORD [FI1]
  comisd xmm1, QWORD [rbp-8]
  jnbe .L4
  movsd xmm0, QWORD [rbp-8]
  comisd xmm0, QWORD [FI0]
  jbe .L3
.L4:
;s
;s
  mov QWORD [rbp-24], 6
.L3:
;s
  movsd xmm0, QWORD [rbp-8]
  ucomisd xmm0, QWORD [rbp-0]
  jp .EL1
  ucomisd xmm0, QWORD [rbp-0]
  je .EL2
.EL1:
  movsd xmm0, QWORD [rbp-8]
  ucomisd xmm0, QWORD [FI1]
  jp .L6
  ucomisd xmm0, QWORD [FI1]
  jne .L6
.EL2:
  movsd xmm0, QWORD [rbp-8]
  comisd xmm0, QWORD [FI0]
  jbe .L6
;s
;s
  mov QWORD [rbp-24], 27
.L6:
;s
  mov rax, QWORD [rbp-24]
  mov rdi, rax
  mov rax, 60
  syscall
  pop rbp
  mov rax, 60
  xor rdi, rdi
  syscall