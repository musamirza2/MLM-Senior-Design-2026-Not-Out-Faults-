#include "dma_buffer.h"


#define DMA_BUFFER_SIZE 1024
#define RING_BUFFER_MASK (DMA_BUFFER_SIZE - 1)

struct dma_buffer {
    uint8_t buffer[DMA_BUFFER_SIZE];
    uint16_t head;
    uint16_t tail;
};

