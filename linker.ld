OUTPUT_FORMAT(elf64-littleriscv)

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

    PROVIDE(_tls_start = .);    
    .tdata : {
        PROVIDE(_tdata_start = .);
        *(.tdata)
        PROVIDE(_tdata_end = .);
    }
    .tbss (NOLOAD) :  {	
        PROVIDE(_tbss_start = .);
        *(.tbss)
        PROVIDE(_tbss_end = .);
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

    . = ALIGN(8);
    PROVIDE(_tls_dyn = .);
    . = . + 4M; /* 4MB for per-hart storage (tls+stack) */
    PROVIDE( _end = . );

}
