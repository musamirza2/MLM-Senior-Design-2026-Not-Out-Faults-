#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"

void app_main(void)
{
    led_init();

    while (true) {
        led_all_on();
        vTaskDelay(pdMS_TO_TICKS(1000));

        led_all_off();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
