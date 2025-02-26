ENTRY(start)

SECTIONS
{
    /* The Multiboot header must be located at the beginning of the file */
    . = 1M; /* Start at 1MB for kernel */
    
    .text :
    {
        *(.multiboot)  /* Include Multiboot header */
        *(.text)       /* Include all text sections */
    }

    .rodata :
    {
        *(.rodata*)    /* Include read-only data sections */
    }

    .data :
    {
        *(.data*)      /* Include data sections */
    }

    .bss :
    {
        *(.bss*)       /* Include uninitialized data sections */
        *(COMMON)
        . = ALIGN(4);
    }

    /* Ensure the binary ends on a page boundary */
    /DISCARD/ :
    {
        *(.comment)
        *(.eh_frame)
    }
}
