#ifndef UART_DRIVERS_H
#define UART_DRIVERS_H

#include <stdint.h>
#include "driver/uart.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"

// for freeRTOS TickType_t
#ifndef TASK_STACK_SIZE
#define TASK_STACK_SIZE 4096 
#endif

esp_err_t uart_init(void);
esp_err_t uart_write(uart_port_t uart_num, const uint8_t *data, uint16_t length);
// Returns actual bytes read, zero on timeout, or -1 on error.
int uart_read(uart_port_t uart_num, uint8_t *data, uint16_t length,
              TickType_t timeout_ticks);




void uart_rx_task(void *pvParameters);

void lcd_prediction_task(void *pvParameters);


#endif
