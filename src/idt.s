section .text
global load_idt

extern pit_handler

load_idt:
    lidt [idt_ptr]   ; Load the IDT pointer
    ret

section .data
align 16
idt: times 256 dq 0  ; 256 entries for the IDT
idt_ptr:
    dw 256 * 16 - 1  ; Limit (size of IDT - 1)
    dd idt           ; Base address of the IDT

section .bss
align 8
timer_ticks: resq 1  ; 64-bit global timer ticks
