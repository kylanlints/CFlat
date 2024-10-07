section .rodata
  FI0  dd  1094713344
  FI1  dd  1093664768
section .text
global _start
_start:
  push rbp
  mov rbp, rsp
  sub rsp, 16
  mov DWORD [rbp-16], 1082130432
  mov DWORD [rbp-12], 0
  cmp DWORD [rbp-12], 400
  jge .L0
.L3:
  movss xmm0, DWORD [rbp-16]
  mulss xmm0, DWORD [FI0]
  divss xmm0, DWORD [FI1]
  movss DWORD [rbp-16], xmm0
  mov eax, DWORD [rbp-12]
  add eax, 1
  mov DWORD [rbp-12], eax
  cmp DWORD [rbp-12], 400
  jl .L3
.L0:
  movss xmm0, DWORD [rbp-16]
  cvttss2si rax, xmm0
  mov QWORD [rbp-8], rax
  mov rax, QWORD [rbp-8]
  call print_rax_decimal
  mov rdi, rax
  mov rax, 60
  syscall
  pop rbp
  mov rax, 60
  xor rdi, rdi
  syscall

section .data
    newline db 0xA     ; Newline character (ASCII 10)

section .bss
    buffer resb 21     ; Buffer to hold the string representation of the number

section .text
global print_rax_decimal

print_rax_decimal:
    ; Preserve rax, rcx, rsi, rdi
    push rax
    push rcx
    push rsi
    push rdi

    ; Initialize registers for conversion
    mov rsi, buffer    ; Point rsi to the buffer
    mov rcx, 10        ; Base 10 for division
    xor rdi, rdi       ; Clear rdi to use it as a counter

    .convert_loop:
        xor rdx, rdx       ; Clear rdx
        div rcx            ; rax / 10, remainder in rdx, quotient in rax
        add dl, '0'        ; Convert remainder to ASCII
        mov [rsi], dl      ; Store ASCII character in buffer
        inc rsi            ; Move to next position in buffer
        inc rdi            ; Increment the digit counter
        test rax, rax      ; Check if rax is 0
        jnz .convert_loop  ; If not, continue the loop

    ; Reverse the string
    mov rbx, buffer        ; rbx points to the start of the buffer
    sub rsi, 1             ; Adjust rsi to point to the last digit in the buffer

    .reverse_loop:
        cmp rbx, rsi       ; Compare start and end pointers
        jge .done_reverse  ; If pointers meet or cross, reverse is done
        mov al, [rbx]      ; Load the byte from the start
        mov dl, [rsi]      ; Load the byte from the end
        mov [rbx], dl      ; Store the end byte at the start
        mov [rsi], al      ; Store the start byte at the end
        inc rbx            ; Move start pointer forward
        dec rsi            ; Move end pointer backward
        jmp .reverse_loop  ; Repeat until done

    .done_reverse:
    ; Print the number (now in correct order)
    mov rsi, buffer        ; Reset rsi to the start of the buffer
    mov rdx, rdi           ; Set rdx to the length of the string
    mov rax, 1             ; sys_write system call number
    mov rdi, 1             ; File descriptor 1 (stdout)
    syscall                ; Make the system call

    ; Print the newline character
    mov rax, 1             ; sys_write system call number
    mov rdi, 1             ; File descriptor 1 (stdout)
    mov rsi, newline       ; Address of the newline character
    mov rdx, 1             ; Write 1 byte
    syscall                ; Make the system call

    ; Restore registers
    pop rdi
    pop rsi
    pop rcx
    pop rax

    ret
