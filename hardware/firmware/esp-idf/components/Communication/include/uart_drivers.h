#ifndef UART_DRIVERS_H
#define UART_DRIVERS_H

#include <stdint.h>
#include "driver/uart.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"

esp_err_t uart_init(void);
esp_err_t uart_write(uart_port_t uart_num, const uint8_t *data, uint16_t length);
// Returns actual bytes read, zero on timeout, or -1 on error.
int uart_read(uart_port_t uart_num, uint8_t *data, uint16_t length,
              TickType_t timeout_ticks);

#endif
