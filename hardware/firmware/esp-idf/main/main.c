#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Fault_Indicators_Led.h"
#include "lcd.h"
#include "uart_drivers.h"
#include "peripherals.h"
#include "esp_log.h"

void app_main(void) {
    led_init();

    //double input_features[4] = {5.1, 3.5, 1.4, 0.2}; 
    //double prediction = score(input_features);

    //Serial.print("Model Prediction Output: ");
    //Serial.println(prediction);

    lcd_init();
    esp_err_t uart_status = uart_init();
    if (uart_status != ESP_OK) {
        ESP_LOGE("main", "UART initialization failed: %s; check CH343 port/pin configuration",
                 esp_err_to_name(uart_status));
    } else {
        // 2. Spawn the UART RX Task ONCE
        // This task will run in the background, constantly listening for your MATLAB simulated data
        xTaskCreate(uart_rx_task, "uart_rx_task", TASK_STACK_SIZE, NULL, 10, NULL);
        // print to lcd that the task has been spawned successfully
        xTaskCreate(lcd_prediction_task, "lcd_prediction",
            TASK_STACK_SIZE, NULL, 10, NULL);
        ESP_LOGI("main", "UART RX Task spawned successfully.");
    }
    

    while (true) {
        
        if (uart_status == ESP_OK) {
            led_all_on();
            vTaskDelay(pdMS_TO_TICKS(1000));

            led_all_off();
            vTaskDelay(pdMS_TO_TICKS(1000));



            // read write from matlab and print to console
            /*
            uint8_t rx_byte;
            int len = uart_read_bytes(
                UART_PORT_NUM,
                &rx_byte,
                1,
                pdMS_TO_TICKS(1000)
            );

            if (len > 0) {
                ESP_LOGI("main", "Received: %u (0x%02X)",
                (unsigned)rx_byte, (unsigned)rx_byte);
            } else if (len < 0) {
                ESP_LOGE("main", "UART read failed");
            } else {
                ESP_LOGW("main", "No data received within timeout");
            } */

        }

        // print features[]
        // lcd_send_command(0x80); /* Return to the start of the first row. */
        //lcd_print(features[0]);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
