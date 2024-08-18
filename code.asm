section .data
section .rodata
  FI0  dd  1090519040
  FI1  dd  1065353216
  FI2  dd  1091567616
  FI3  dd  1088421888
section .bss
section .text
global _start
_start:
  push rbp
  mov rbp, rsp
  mov QWORD [rbp-24], 7
  mov DWORD [rbp-20], 1090519040
  mov DWORD [rbp-16], 1065353216
  movss xmm0, DWORD [rbp-20]
  ucomiss xmm0, DWORD [FI0]
  jp .FL0
  jne .FL0
  movss xmm0, DWORD [FI1]
.FL1:
  ucomiss xmm0, DWORD [rbp-20]
  jp .FL2
  jne .FL2
  movss xmm0, DWORD [FI1]
.FL3:
  movss DWORD [rbp-20], xmm0
  movss xmm0, DWORD [rbp-20]
  ucomiss xmm0, DWORD [FI2]
  setp al
  mov edx, 1
  cmovne eax, edx
  movss xmm0, DWORD [rbp-20]
  ucomiss xmm0, DWORD [FI3]
  setnp dl
  mov ecx, 0
  cmovne edx, ecx
  xor eax, edx
  movzx eax, al
  xorps xmm0, xmm0
  cvtsi2ss xmm0, eax
  movss DWORD [rbp-20], xmm0
  mov rax, 4632937379169042432
  mov QWORD [rbp-8], rax
  mov rax, 4632937379169042432
  mov QWORD [rbp-0], rax
  cmp QWORD [rbp-24], 8
  sete al
  movzx eax, al
  cmp rax, QWORD [rbp-24]
  jne .L0
  cmp QWORD [rbp-24], 8
  sete al
  movzx eax, al
  cmp rax, QWORD [rbp-24]
  sete al
  movzx eax, al
  mov QWORD [rbp-24], rax
.L0:
  mov rax, QWORD [rbp-24]
  mov rdi, rax
  mov rax, 60
  syscall
  pop rbp
  mov rax, 60
  xor rdi, rdi
  syscall
.FL0:
  xorps xmm0, xmm0
  jmp .FL1
.FL2:
  xorps xmm0, xmm0
  jmp .FL3
