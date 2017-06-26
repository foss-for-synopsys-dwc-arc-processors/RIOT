/* ------------------------------------------
 * Copyright (c) 2017, Synopsys, Inc. All rights reserved.

 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:

 * 1) Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.

 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.

 * 3) Neither the name of the Synopsys, Inc., nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
--------------------------------------------- */


#include "arch/thread_arch.h"
#include "sched.h"

#include "inc/arc/arc.h"
#include "inc/arc/arc_exception.h"
#include "inc/embARC_toolchain.h"

extern uint32_t _f_sdata;
extern uint32_t _e_stack;
extern uint32_t _f_stack;

#define INITIAL_STATUS32 (0x80000000 | STATUS32_RESET_VALUE | (((INT_PRI_MAX - INT_PRI_MIN) << 1) & 0x1e)) 


char *thread_arch_stack_init(thread_task_func_t task_func,
                             void *arg,
                             void *stack_start,
                             int stack_size)
{

    uint32_t *stk;
    stk = (uint32_t *)((uintptr_t)stack_start + stack_size);

    stk--;
    *stk =  INITIAL_STATUS32;

    stk--;
    *stk = (uint32_t) task_func;

#ifdef ARC_FEATURE_CODE_DENSITY
    /* code density related regs */
    stk--;
    *stk =  0x00000000; /* JLI_BASE*/
    stk--;
    *stk =  0x00000000; /* LDI_BASE*/
    stk--;
    *stk =  0x00000000; /* EI_BASE*/
#endif

    /* LP related regs */
    stk--;
    *stk =  0x00000000; /* LP_COUNT */
    stk--;
    *stk =  0x00000000; /* LP_START */
    stk--;
    *stk =  0x00000000; /* LP_END */

    stk--;
    *stk =  (uint32_t)sched_task_exit; /* R31, blink */

    stk--;
    *stk =  0x11111111; /* R11 */
    stk--;
    *stk =  0x10101010; /* R10 */
#ifndef ARC_FEATURE_RF16
    stk--;
    *stk =  0x09090909; /* R9 */
    stk--;
    *stk =  0x08080808; /* R8 */
    stk--;
    *stk =  0x07070707; /* R7 */
    stk--;
    *stk =  0x06060606; /* R6 */
    stk--;
    *stk =  0x05050505; /* R5 */
    stk--;
    *stk =  0x04040404; /* R4 */
#endif
    stk--;
    *stk =  0x03030303; /* R3 */
    stk--;
    *stk =  0x02020202; /* R2 */
    stk--;
    *stk =  0x01010101; /* R1 */
    stk--;
    *stk =  (uint32_t) arg;   /* R0 */    
    stk--;
    *stk =  0x12121212; /* R12 */
    stk--;
    *stk =  (uint32_t)&_f_sdata;  /* gp */    
    stk--;
    *stk =  0x00000000; /* fp */
    stk--;
    *stk =  0x00000000; /* ilink */
    stk--;
    *stk =  0x30303030; /* R30 */
    stk--;
    *stk =  0x00000000; /* BTA */
/** CALLEE saved regs */
    stk--;
    *stk =  0x13131313; /* R13 */
    stk--;
    *stk =  0x14141414; /* R14 */
    stk--;
    *stk =  0x15151515; /* R15 */
#ifndef ARC_FEATURE_RF16
    stk--;
    *stk =  0x16161616; /* R16 */
    stk--;
    *stk =  0x17171717; /* R17 */
    stk--;
    *stk =  0x18181818; /* R18 */
    stk--;
    *stk =  0x19191919; /* R19 */
    stk--;
    *stk =  0x20202020; /* R20 */
    stk--;
    *stk =  0x21212121; /* R21 */
    stk--;
    *stk =  0x22222222; /* R22 */
    stk--;
    *stk =  0x23232323; /* R23 */
    stk--;
    *stk =  0x24242424; /* R24 */
    stk--;
    *stk =  0x25252525; /* R25 */
#endif
    return (char *)stk;
}

/* This function returns the number of bytes used on the ISR stack */
int thread_arch_isr_stack_usage(void)
{
    return 0;
}

/* This function returns ISR stack pointer */
void *thread_arch_isr_stack_pointer(void)
{
    return 0;
}

/* Get the start of the ISR stack */
void *thread_arch_isr_stack_start(void)
{
    return (void *)&_f_stack;
}   

/* Print the current stack to stdout */
void thread_arch_stack_print(void) 
{

}