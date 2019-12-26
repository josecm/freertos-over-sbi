
#define STACK_SIZE  (0x4000)

.section .boot, "ax" 
_boot:
    /* Expecting hart id in a0 */
.option push
.option norelax   
    /* Initialize stack pointer and thread pointer*/

    la      t0, _tls_start
    la      t1, _tls_end
    sub     t0, t1, t0
    li      t1, STACK_SIZE  
    add     t0, t0, t1      //t0 = tls_size + stack_size

    mul     t1, t0, a0      //t1 = hart offset (hartid*t0)
    la      tp, _tls_dyn
    add     tp, tp, t1
    add     sp, tp, t0

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
    or      t0, t0, 1
    csrw    stvec, t0

    la      t0, wait_for_primary
1:
    lw      t1, 0(t0)
    beqz    t1, 1b

    mv  s0, a0
	/* Call global constructors */
	la a0, __libc_fini_array
	call atexit
	call __libc_init_array
    mv a0, s0
    
    /* Jump to main */
    call _init
    j .

.global _fini
_fini: ret

.balign 4
exception_vector:
    j   .  //syncronous exceptions
.balign 0x4
    j   ipi_handler   //software interrupt
.balign 0x4
.rept   (3)
    j   .   //reserved 2-4
.endr
.balign 0x4
    j   timer_handler   //timer interrupt
.balign 0x4
.rept   (3)
    j   .   //reserved 6-8
.endr
.balign 0x4
    j   plic_handle   //external interrupt
.balign 0x4
.rept   (6)
    j   .   //reserved 10-15
.endr