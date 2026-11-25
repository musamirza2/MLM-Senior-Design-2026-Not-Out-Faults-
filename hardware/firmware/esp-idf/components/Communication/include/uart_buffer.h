#ifndef UART_BUFFER_H
#define UART_BUFFER_H

/*
 * @brief Initialize receiving ring buffer
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t RX_RING_BUFFER_Init(void);

/*
 * @brief task for receiving ring buffer
 * @return ESP_OK on success, or an error code on failure.
 */
void uart_Ring_Buffer_task(void *pvParameters);

#endif