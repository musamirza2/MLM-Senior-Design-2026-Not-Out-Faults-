#ifndef UART_DRIVERS_H
#define	UART_DRIVERS_H



uint8_t uart_init(uint8_t uart_num, uint32_t baud_rate);

uint8_t uart_write(uint8_t uart_num, const uint8_t *data, uint16_t length);

uint8_t uart_read(uint8_t uart_num, uint8_t *data, uint16_t length);

#endif	/* UART_DRIVERS_H */