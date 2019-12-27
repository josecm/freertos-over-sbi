OUTPUT_FORMAT(elf64-littleriscv)

HEAP_SIZE = 4M;
STACK_SIZE = 4M;

MEMORY {
    RAM (rwx)    : ORIGIN = 0x80200000, LENGTH = 64M    
}

SECTIONS {

    .boot : {
        *(.boot)
    } 

    .text :  {
        *(.text*)
    } 

    .rodata :  {
        *(.rodata*)
    }

    PROVIDE(_tls_end = .);

    .data : {
        *(.data .data.*)
        PROVIDE(__global_pointer$ = . + 0x800);
        *(.sdata .sdata.* .sdata2.*)
    }

    .bss (NOLOAD) :  {	
        __bss_start = .;
        *(.bss* .sbss*) 
        *(COMMON)	
        __bss_end = .;
    }

    . = ALIGN(16) + STACK_SIZE;
    PROVIDE( __freertos_irq_stack_top = . );
    PROVIDE( __heap_end = . );
}
