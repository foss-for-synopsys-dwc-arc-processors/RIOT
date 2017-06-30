#ifndef RIOT_BOARD_H
#define RIOT_BOARD_H

#include "inc/arc/arc_feature_config.h"
#include "cpu.h"
#include "periph_conf.h"

#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @brief Use the USART1 for STDIO on this board
 */
#define UART_STDIO_DEV      UART_DEV(1)

#define XTIMER_SHIFT    0
#define XTIMER_HZ       ARC_FEATURE_CPU_CLOCK_FREQ
#define XTIMER_OVERHEAD 10000
#define XTIMER_ISR_BACKOFF 20000
#define XTIMER_PERIODIC_SPIN 20000
#define XTIMER_PERIODIC_RELATIVE 20000
/**
 * @brief   board_init for riot-os. embARC has its own board_init. so define riot_board_init here
 */
void riot_board_init(void);

#ifdef __cplusplus
}
#endif
#endif /* RIOT_BOARD_H */