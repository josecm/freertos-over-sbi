
#define STACK_SIZE  (0x4000)

.section .boot, "ax" 
_boot:
    /* Expecting hart id in a0 */
    bnez a0, .

.option push
.option norelax   
    /* Initialize stack pointer and thread pointer*/
    la sp, __freertos_irq_stack_top

    /* Initialize global_pointer */
	la gp, __global_pointer$
.option pop

    bnez     a0, skip

    /* Clear bss */ 
    la t0, __bss_start
    la t1, __bss_end
    bgeu t0, t1, 2f
1:
    sw zero, (t0)
    addi t0, t0, 4
    bltu t0, t1, 1b
2:

    .pushsection .data
    .align 3
wait_for_primary:
    .word 0x0
    .popsection

    la  t0, wait_for_primary
    li  t1, 1
    sw  t1, 0(t0)

skip:

    la      t0, exception_vector
    csrw    stvec, t0

    la      t0, wait_for_primary
1:
    lw      t1, 0(t0)
    beqz    t1, 1b

    mv  s0, a0
	/* Call global constructors */
/* 	la a0, __libc_fini_array
	call atexit
	call __libc_init_array
    mv a0, s0 */
    
    /* Jump to main */
    call _init
    j .

.global _fini
_fini: ret

.balign 4
exception_vector:
    j   .  //syncronous exceptions