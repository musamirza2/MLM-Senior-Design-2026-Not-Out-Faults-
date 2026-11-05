#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Fault_Indicators_Led.h"
#include "lcd.h"
#include "uart_drivers.h"
#include "peripherals.h"
#include "esp_log.h"

void app_main(void)
{
    led_init();
    /*
    double input_features[4] = {5.1, 3.5, 1.4, 0.2}; 
    double prediction = score(input_features);

    Serial.print("Model Prediction Output: ");
    Serial.println(prediction); */

    // lcd_init();
    esp_err_t uart_status = uart_init();
    if (uart_status != ESP_OK) {
        ESP_LOGE("main", "UART initialization failed: %s; check CH343 port/pin configuration",
                 esp_err_to_name(uart_status));
    }
    
    while (true) {
        
        if (uart_status == ESP_OK) {
            led_all_on();
            vTaskDelay(pdMS_TO_TICKS(1000));

            led_all_off();
            vTaskDelay(pdMS_TO_TICKS(1000));
            static const uint8_t message[] = "Hello, UART!\n";
            ESP_ERROR_CHECK(uart_write(UART_PORT_NUM, message, sizeof(message) - 1));
        }
        // lcd_send_command(0x80); /* Return to the start of the first row. */
        // lcd_print("Hello, World!");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
