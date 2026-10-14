
#ifndef PERIPHERALS_H
#define	PERIPHERALS_H

#include <avr/io.h>

typedef struct UART {
    volatile uint8_t *UCSRA;
    volatile uint8_t *UCSRB;
    volatile uint8_t *UCSRC;
    volatile uint8_t *UBRRH;
    volatile uint8_t *UBRRL;
    volatile uint8_t *UDR;
} UART_t;