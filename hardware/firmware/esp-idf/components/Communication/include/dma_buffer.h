#ifndef DMA_BUFFER_H
#define DMA_BUFFER_H

#include <stdint.h>
#include "esp_err.h"

esp_err_t dma_buffer_init(void);

esp_err_t is_buffer_full(void);

esp_err_t is_buffer_empty(void);

esp_err_t dma_buffer_write(const uint8_t *data, uint16_t length);

esp_err_t dma_buffer_read(uint8_t *data, uint16_t length);

esp_err_t dma_task(uint8_t *data, uint16_t length);

