; boot.s
section .text
global start
extern kernel_main

; Multiboot header (required for GRUB)
align 4
dd 0x1BADB002           ; Magic number
dd 0x00                 ; Flags
dd -(0x1BADB002 + 0x00) ; Checksum

start:
    ; Set up a stack   
    mov esp, stack_space
    call kernel_main

hlt                     ; Halt the CPU in case kernel_main returns

section .bss
align 4
resb 524288             ; Reserve 4KB for stack
stack_space:
