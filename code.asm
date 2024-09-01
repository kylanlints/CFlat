section .data
  newline db 0xA     ; Newline character (ASCII 10)
section .rodata
section .bss
  buffer resb 21     ; Buffer to hold the string representation of the number
section .text
global _start
_start:
  push rbp
  mov rbp, rsp
  sub rsp, 20
  mov DWORD [rbp-24], 0
  mov QWORD [rbp-20], 0
.LINF0:
  mov DWORD [rbp-24], 1
  mov QWORD [rbp-12], 2
  mov rax, QWORD [rbp-20]
  shr rax, 1
  cmp rax, QWORD [rbp-12]
  jb .L0
.L3:
  mov rax, QWORD [rbp-20]
  cqo
  div QWORD [rbp-12]
  mul QWORD [rbp-12]
  mov rcx, rax
  mov rax, QWORD [rbp-20]
  sub rax, rcx
  cmp rax, 0
  jne .L6
  mov DWORD [rbp-24], 0
  jmp .L0
.L6:
  mov rax, QWORD [rbp-12]
  add rax, 1
  mov QWORD [rbp-12], rax
  mov rax, QWORD [rbp-20]
  shr rax, 1
  cmp rax, QWORD [rbp-12]
  jae .L3
.L0:
  cmp DWORD [rbp-24], 0
  jz .L9
  mov rax, QWORD [rbp-20]
  call print_rax_decimal
.L9:
  mov rax, QWORD [rbp-20]
  add rax, 1
  mov QWORD [rbp-20], rax
  jmp .LINF0
.B1:
  mov rax, QWORD [rbp-20]
  sub rax, 1
  mov QWORD [rbp-20], rax
  mov rax, QWORD [rbp-20]
  mov rdi, rax
  mov rax, 60
  syscall
  pop rbp
  mov rax, 60
  xor rdi, rdi
  syscall

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