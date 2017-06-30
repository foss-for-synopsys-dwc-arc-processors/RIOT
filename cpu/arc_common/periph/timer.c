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
#include "periph/timer.h"
#include "inc/arc/arc_timer.h"
#include "inc/arc/arc_exception.h"
#include "board.h"


#if ARC_FEATURE_TIMER0_PRESENT
static timer_cb_t arc_timer0_callback;
static void *   arc_timer0_cb_arg;
static unsigned int timer0_tick;

static void arc_timer0_isr(void *ptr)
{
    arc_timer_int_clear(0);
    timer0_tick += _arc_aux_read(AUX_TIMER0_LIMIT) + _arc_aux_read(AUX_TIMER0_CNT);
    arc_timer0_callback(arc_timer0_cb_arg, 0);
}
#endif


#if ARC_FEATURE_TIMER1_PRESENT
static timer_cb_t arc_timer1_callback;
static void *   arc_timer1_cb_arg;
static unsigned int timer1_tick;

static void arc_timer1_isr(void *ptr)
{
    arc_timer_int_clear(1);
    timer1_tick += _arc_aux_read(AUX_TIMER1_LIMIT) + _arc_aux_read(AUX_TIMER1_CNT);
    arc_timer1_callback(arc_timer0_cb_arg, 0);
}
#endif


/* Initialize the given timer */
int timer_init(tim_t dev, unsigned long freq, timer_cb_t cb, void *arg)
{
    if (dev >= TIMER_NUMOF) {
        return -1;
    }

    if (dev == 0) {
#if ARC_FEATURE_TIMER0_PRESENT
        arc_timer0_callback = cb;
        arc_timer0_cb_arg = arg;
        timer0_tick = 0;
        int_handler_install(ARC_FEATURE_TIMER0_VECTOR, arc_timer0_isr);
        arc_timer_start(dev, TIMER_CTRL_IE|TIMER_CTRL_NH, 0xffffffff);
        int_enable(ARC_FEATURE_TIMER0_VECTOR);
#else
        return -1;
#endif
    } else if(dev == 1) {

#if ARC_FEATURE_TIMER1_PRESENT
        arc_timer1_callback = cb;
        arc_timer1_cb_arg = arg;
        timer1_tick = 0;
        int_handler_install(ARC_FEATURE_TIMER1_VECTOR, arc_timer1_isr);
        arc_timer_start(dev, TIMER_CTRL_IE|TIMER_CTRL_NH, 0xffffffff);
        int_enable(ARC_FEATURE_TIMER1_VECTOR);
#else
        return -1;
#endif
    }

    return 0; 
}

/* Set a given timer channel for the given timer device */
int timer_set(tim_t dev, int channel, unsigned int timeout)
{
    unsigned int now = 0;

#if ARC_FEATURE_TIMER0_PRESENT
    if (dev == 0) {
        timer0_tick +=  _arc_aux_read(AUX_TIMER0_CNT);
        arc_timer_start(dev, TIMER_CTRL_IE|TIMER_CTRL_NH, timeout);
    }
#endif

#if ARC_FEATURE_TIMER1_PRESENT
    if (dev == 1) {
        timer1_tick +=  _arc_aux_read(AUX_TIMER1_CNT);
        arc_timer_start(dev, TIMER_CTRL_IE|TIMER_CTRL_NH, timeout);

    }
#endif
    return 0;
}

/* Set an absolute timeout value for the given channel of the given timer */
int timer_set_absolute(tim_t dev, int channel, unsigned int value)
{
#if ARC_FEATURE_TIMER0_PRESENT
    if (dev == 0) {
        timer0_tick +=  _arc_aux_read(AUX_TIMER0_CNT);
        value = value - timer0_tick;
        arc_timer_start(dev, TIMER_CTRL_IE|TIMER_CTRL_NH, value);
    }
#endif

#if ARC_FEATURE_TIMER1_PRESENT
    if (dev == 1) {
        timer1_tick +=  _arc_aux_read(AUX_TIMER1_CNT);
        value = value - timer1_tick;
        arc_timer_start(dev, TIMER_CTRL_IE|TIMER_CTRL_NH, value);
    }
#endif
    return 0;
}

/* Clear the given channel of the given timer device */
int timer_clear(tim_t dev, int channel)
{
#if ARC_FEATURE_TIMER0_PRESENT
    if (dev == 0) {
        timer0_tick = 0;
    }
#endif

#if ARC_FEATURE_TIMER1_PRESENT
    if (dev == 1) {
        timer1_tick = 0;
    }
#endif
    return -1;
}

/* Read the current value of the given timer device */
unsigned int timer_read(tim_t dev)
{
#if ARC_FEATURE_TIMER0_PRESENT
    if (dev == 0) {
        return timer0_tick + _arc_aux_read(AUX_TIMER0_CNT);
    }
#endif

#if ARC_FEATURE_TIMER1_PRESENT
    if (dev == 1) {
        return timer1_tick + _arc_aux_read(AUX_TIMER1_CNT);
    }
#endif
    return -1;
}

/* Start the given timer */
void timer_start(tim_t dev)
{
#if ARC_FEATURE_TIMER0_PRESENT
    if (dev == 0) {
        int_enable(ARC_FEATURE_TIMER0_VECTOR);
    }
#endif
#if ARC_FEATURE_TIMER1_PRESENT
    if (dev == 1) {
        int_enable(ARC_FEATURE_TIMER1_VECTOR);       
    }
#endif
}

/* Stop the given timer */
void timer_stop(tim_t dev)
{
#if ARC_FEATURE_TIMER0_PRESENT
    if (dev == 0) {
        int_disable(ARC_FEATURE_TIMER0_VECTOR);
    }
#endif
#if ARC_FEATURE_TIMER1_PRESENT
    if (dev == 1) {
        int_disable(ARC_FEATURE_TIMER1_VECTOR);       
    }
#endif
}
