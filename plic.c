/** 
 * baohu separation kernel
 *
 * Copyright (c) Jose Martins, Sandro Pinto
 *
 * Authors:
 *      Jose Martins <josemartins90@gmail.com>
 *
 * baohu is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 2 as published by the Free
 * Software Foundation, with a special exception exempting guest code from such
 * license. See the COPYING file in the top-level directory for details. 
 *
 */

#include <plic.h>
#include <stddef.h>


#define PRIV_U  (0)
#define PRIV_S  (1)
#define PRIV_M  (3)

extern int hart_id;
volatile plic_global_t * const plic_global = (void*)PLIC_BASE;
volatile plic_hart_t *plic_hart;

void plic_probe(){
    volatile uint32_t *ptr = &plic_global->enbl[0][0];

    for(int i = 0; i < 512; i++){
        *ptr++ = i;
    }
}

static int plic_hartidpriv_to_context(int hartid, int mode){
    if(mode != PRIV_M && mode != PRIV_S) return -1;
    return (hartid*2) + (mode == PRIV_M ? 0 : 1);
}

void plic_init(){
    int cntxt = plic_hartidpriv_to_context(hart_id, PRIV_S);
    plic_hart = (void*) PLIC_HART_BASE + (cntxt*sizeof(plic_hart_t));
    plic_hart->threshold = 0;
    //printf("hartd%d: plic_hart at 0x%lx\n", hart_id, plic_hart);
}

void plic_enable_interrupt(int hid, int int_id, bool en){

    int reg_ind = int_id/(sizeof(uint32_t)*8);
    uint32_t mask = 1U << (int_id%(sizeof(uint32_t)*8));
    
    int cntxt = plic_hartidpriv_to_context(hid, PRIV_S);
    if(cntxt < 0) return;
    //printf("hartd%d: enable interrupt\n", hart_id, plic_hart);
    if(en){
        plic_global->enbl[cntxt][reg_ind] |= mask;
    } else {
        plic_global->enbl[cntxt][reg_ind] &= ~mask;
    }

}

void plic_set_prio(int int_id, int prio){
    plic_global->prio[int_id] = prio;
}

int plic_get_prio(int int_id){
    return plic_global->prio[int_id];
}

void (*plic_handler_table[PLIC_MAX_INTERRUPTS])(void);

void plic_register(unsigned long id, void (*handler)()){
    if(id >= PLIC_MAX_INTERRUPTS) return;
    if(plic_handler_table[id] == NULL){
        plic_handler_table[id] = handler;
    }
}

void plic_handle(){

    uint32_t id = plic_hart->claim;

    if(plic_handler_table[id])
        plic_handler_table[id]();

    plic_hart->complete = id;
}
