section .data
section .rodata
  FI0  dd  1065353216
section .bss
section .text
global _start
_start:
  push rbp
  mov rbp, rsp
  mov DWORD [rbp-16], 1065353216
  mov DWORD [rbp-12], 1073741824
  movss xmm0, DWORD [rbp-16]
  subss xmm0, DWORD [FI0]
  movss xmm1, xmm0
  movss xmm0, DWORD [rbp-16]
  addss xmm0, DWORD [FI0]
  ucomiss xmm0, xmm1
  jp .L0
  ucomiss xmm0,xmm1
  jne .L0
  mov DWORD [rbp-16], 1084227584
.L0:
  mov DWORD [rbp-8], 118837123
  movss xmm0, DWORD [rbp-16]
  comiss xmm0, DWORD [rbp-12]
  jb .L3
  mov DWORD [rbp-16], -1082130432
.L3:
  mov DWORD [rbp-4], 11
  movss xmm0, DWORD [rbp-16]
  comiss xmm0, DWORD [rbp-12]
  jnb .L7
  movss xmm1, DWORD [rbp-12]
  comiss xmm1, DWORD [rbp-16]
  jbe .L6
.L7:
  mov DWORD [rbp-16], 1211860160
.L6:
  mov DWORD [rbp-0], 1073741824
  movss xmm0, DWORD [rbp-12]
  subss xmm0, DWORD [FI0]
  movss xmm1, xmm0
  movss xmm0, DWORD [rbp-12]
  addss xmm0, DWORD [FI0]
  ucomiss xmm0, xmm1
  jp .L9
  ucomiss xmm0,xmm1
  jne .L9
  mov DWORD [rbp-12], 1073741824
.L9:
  pop rbp
  mov rax, 60
  xor rdi, rdi
  syscall