#ifndef FIFO_BUFFER_H
#define FIFO_BUFFER_H

#include <stdint.h>
#include "driver/uart.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "uart_drivers.h"

#define FIFO_BUFFER_SIZE 1024
#define FIFO__READ_LENGTH 128

esp_err_t fifo_buffer_read(uint8_t *data, uint16_t length);


esp_err_t fifo_buffer_write(const uint8_t *data, uint16_t length);

uint16_t fifo_rx_task(uint8_t *data, uint16_t length);