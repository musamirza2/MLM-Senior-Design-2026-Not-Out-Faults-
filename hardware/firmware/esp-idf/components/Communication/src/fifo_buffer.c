#include <stdint.h>
#include "driver/uart.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "uart_drivers.h"

#define FIFO_BUFFER_SIZE 1024
#define FIFO__READ_LENGTH 128
#define FRAME_length 17

esp_err_t fifo_buffer_write(const uint8_t *data, uint16_t length) {

    uint16_t bytes_written = uart_write(UART_NUM_0, data, length);
    if (bytes_written != length) {
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t fifo_buffer_read(uint8_t *data, uint16_t length) {
    uint16_t bytes_read = uart_read(UART_NUM_0, data, length, portMAX_DELAY);
    if (bytes_read == 0) {
        return ESP_FAIL;
    }
    return ESP_OK;
}

uint16_t fifo_rx_task(uint8_t *data, uint16_t length) {
    uint8_t frame[FRAME_length];

    if (length < FRAME_length) {
        // Handle error: provided buffer is too small
        return 0; // Indicate failure due to insufficient buffer size
    }

    while (1) {
        // Read a frame from the FIFO buffer
        if (fifo_buffer_read(frame, FRAME_length) == ESP_OK) {
            // Process the received frame
            // For example, you can copy it to the provided data buffer
            if (length >= FRAME_length) {
                memcpy(data, frame, FRAME_length);
                return FRAME_length; // Return the number of bytes read
            } else {
                // Handle error: provided buffer is too small
                return 0; // Indicate failure due to insufficient buffer size
            }
        } else {
            // Handle error: failed to read from FIFO buffer
            return 0; // Indicate failure
        }
        vTaskDelay(pdMS_TO_TICKS(10)); // Delay to prevent busy waiting
    }
}
