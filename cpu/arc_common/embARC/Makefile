# define the module that is build
MODULE = embARC

SRC += arc_cxx_support.c arc_cache.c arc_exception.c arc_timer.c\
    console_io.c xprintf.c

ASSMSRC += arc_startup.S arc_exc_asm.S 

export CFLAGS += -DMID_COMMON

EMBARC_SRC_DIR += arc arc/startup  \
                board common

ifeq (emsk, $(ARC_BOARD))
    SRC += emsk_init.c emsk_timer.c mux.c emsk_gpio.c dw_gpio_obj.c dw_uart_obj.c \
            dw_gpio.c dw_uart.c 
    EMBARC_SRC_DIR += board/emsk/common board/emsk/gpio \
                    board/emsk/uart device/designware/gpio \
                    device/designware/uart
endif
	

vpath %.c $(EMBARC_SRC_DIR)
vpath %.S $(EMBARC_SRC_DIR)
vpath %.s $(EMBARC_SRC_DIR) 

include $(RIOTBASE)/Makefile.base
