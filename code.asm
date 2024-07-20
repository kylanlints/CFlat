section .data
section .rodata
  FI0  dd  1065353216
section .bss
section .text
global _start
_start:
  push rbp
  mov rbp, rsp
  mov DWORD [rbp-0], 1065353216
  movss xmm0, DWORD [rbp-0]
  subss xmm0, DWORD [FI0]
  movss xmm1, DWORD [rbp-0]
  addss xmm1, DWORD [FI0]
  ucomiss xmm1, xmm0
  jp .L0
  ucomiss xmm1, xmm0
  jne .L0
  mov DWORD [rbp-0], 1084227584
.L0:
  pop rbp
  mov rax, 60
  xor rdi, rdi
  syscall