    .global _start

    .text

_start:

        mov $1, %rax
        mov $1, %rdi
        mov $message, %rsi
        mov $13, %rdx
        syscall

        
        mov $1, %rax
        mov $1, %rdi
        mov $msg, %rsi
        mov $27, %rdx
        syscall

        LDR r0, =vars
    

        #exit(0)
        mov $60, %rax
        xor %rdi, %rdi
        syscall
message:
        .ascii "Hello, world\n"

msg: 
        .ascii "Klarte å skrive to linjer\n"

vars:
        .word 1 
        .word 2 
        .word 0 