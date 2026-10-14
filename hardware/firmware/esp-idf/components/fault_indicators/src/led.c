#include "led.h"

#include <stdint.h>
#include "esp_err.h"
#include "driver/gpio.h"

#define LED_GPIO_16 GPIO_NUM_16
#define LED_GPIO_17 GPIO_NUM_17
#define LED_GPIO_18 GPIO_NUM_18
#define LED_GPIO_7 GPIO_NUM_7
#define LED_GPIO_6  GPIO_NUM_6

static const gpio_num_t led_gpios[LED_COUNT] = {
    LED_GPIO_16,
    LED_GPIO_17,
    LED_GPIO_18,
    LED_GPIO_7,
    LED_GPIO_6
};

void led_init(void)
{
    uint64_t pin_mask = 0;

    for (int i = 0; i < LED_COUNT; i++) {
        pin_mask |= (1ULL << led_gpios[i]);
    }

    gpio_config_t config = {
        .pin_bit_mask = pin_mask,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    ESP_ERROR_CHECK(gpio_config(&config));
    led_all_off();
}

void led_set(led_id_t led, bool on)
{
    if (led < 0 || led >= LED_COUNT) {
        return;
    }

    ESP_ERROR_CHECK(gpio_set_level(led_gpios[led], on ? 1 : 0));
}

void led_on(led_id_t led)
{
    led_set(led, true);
}

void led_off(led_id_t led)
{
    led_set(led, false);
}

void led_all_on(void)
{
    for (int i = 0; i < LED_COUNT; i++) {
        led_on((led_id_t)i);
    }
}

void led_all_off(void)
{
    for (int i = 0; i < LED_COUNT; i++) {
        led_off((led_id_t)i);
    }
}
