#include "dma_buffer.h"


#define DMA_BUFFER_SIZE 1024
#define RING_BUFFER_MASK (DMA_BUFFER_SIZE - 1)

struct dma_buffer {
    uint8_t buffer[DMA_BUFFER_SIZE];
    uint16_t head;
    uint16_t tail;
};

esp_err_t dma_buffer_init(void) {
    // Allocate memory on the heap for DMA buffer structure   
    struct dma_buffer *dma_buf = malloc(sizeof(struct dma_buffer));
    if (dma_buf == NULL) {
        return ESP_ERR_NO_MEM;
    }
    dma_buf->head = 0;
    dma_buf->tail = 0;
    return ESP_OK;
}

esp_err_t is_buffer_full(void) {
    struct dma_buffer *dma_buf = malloc(sizeof)struct dma_buffer);
    if (dma_buf == NULL) {
        return ESP_ERR_NO_MEM;
    }

    return ((dma_buf->head - dma_buf->tail) == DMA_BUFFER_SIZE) ? ESP_OK : ESP_FAIL;
}

esp_err_t is_buffer_empty(void) {
    struct dma_buffer *dma_buf = malloc(sizeof(struct dma_buffer));
    if (dma_buf == NULL) {
        return ESP_ERR_NO_MEM;
    }

    return (dma_buf->head == dma_buf->tail) ? ESP_OK : ESP_FAIL;
}
