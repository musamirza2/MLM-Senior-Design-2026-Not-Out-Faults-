#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "uart_drivers.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "dma_buffer.h"

#define FRAME_SIZE      17
#define UART_PORT       UART_NUM_1

static const char *TAG = "UART_Buffer"; 

struct __attribute__((packed)) packet {
    uint16_t header; // header
    uint8_t seqNum;  // 
    uint16_t v_t[3]; // Payload: voltage for a,b,c
    uint16_t i_t[3]; // Payload: current for a,b, c
    uint16_t crc16; // CRC
}; CountinousFrame_t;

esp_err_t RX_RING_BUFFER_Init(void) {
    esp_err_t err = ESP_OK;
    const uart_config_t uart_cfg {
        .baud_rate  = UART_BAUD,
        .data_bits  = UART_DATA_8_BITS,
        .parity     = UART_PARITY_DISABLE,
        .stop_bits  = UART_STOP_BITS_1,
        .flow_ctrl  = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    err = uart_param_config(UART_PORT, &uart_config);
    if (err != ESP_OK) {
        ESP_LOGE("RX_RING_BUFFER_Init", "Failed to configure UART parameters: %s", esp_err_to_name(err));
        return err;
    }
    err = uart_set_pin(UART_PORT, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    if (err != ESP_OK) {
        ESP_LOGE("RX_RING_BUFFER_Init", "Failed to configure UART pins: %s", esp_err_to_name(err));
        return err;
    }

    // Allocates the 1024-byte FreeRTOS ring buffer inside uart_driver
    err = uart_driver_install(UART_PORT, RX_RING_BUFFER_SIZE, 0, 0, NULL, 0);
    if (err != ESP_OK) {
        ESP_LOGE("RX_RING_BUFFER_Init", "Failed to install UART driver: %s", esp_err_to_name(err));
        return err;
    }
    // b
    uart_intr_config_t uart_intr = {
        .intr_enable_mask = UART_INTR_RXFIFO_FULL | UART_INTR_RXFIFO_TOUT,
        .rxfifo_full_thresh = FRAME_SIZE,
        .rxtimout_thresh = 2, 
    };
    err = uart_intr_config(UART_PORT, &uart_intr);
    if (err != ESP_OK) {
        ESP_LOGE("uart_intr_config", "Failed set UART interrupt and timeouts.: %s", esp_err_to_name(err));
        return err;
    }
    return err
}

void uart_Ring_Buffer_task(void *pvParameters) 
    // 1. SETUP:
    uint8_t frame_buffer[FRAME_SIZE];
    uint8_t rx_chunk[64];

    // Create an event struct 
    uart_event_t event;

    while(1) {
        // Task should sleep until the UART driver triggers an event
        if(xQueueReceiver(uart_queue, (void *)&event, portMAX_DELAY)) {
            
            switch(event.type) {
                int len = 0;
                // 
                case UART_DATA:
                    // Check the number of bytes available in the RX FIFO buffer
                    ESP_ERROR_CHECK(uart_get_buffered_data_len(UART_PORT, (size_t*)&length));
                    // 2. BLOCKING CALL
                    len = uart_read_bytes(UART_PORT, 
                                            rx_chunk, 
                                            sizeof(rx_chunk), 
                                            pdMS_TO_TICKS(5));
                    // if len valid
                    if (len > 0) {
                        ESP_LOGI(TAG, "Received %d bytes", len);
                    } break;
                
                case UART_FIFO_OVF:
                case UART_BUFFER_FLOW:
                    ESP_LOGW(TAG, "UART overflow, flushing buffer");
                    uart_flush_input();
                    xQueueReset();
                    break;

                case UART_TIMEOUT:
                    // code
                    break;

                case UART_PARITY_ERR:
                case UART_FRAME_ERR:
                    break;
                default:
                    break;
            }
        }
    }
    vTaskDelete(NULL);
} 