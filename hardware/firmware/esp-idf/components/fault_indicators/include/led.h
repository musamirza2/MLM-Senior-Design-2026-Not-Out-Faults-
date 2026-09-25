#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "driver/gpio.h"

#define LED_COUNT 5

#ifndef LED1_GPIO
#define LED1_GPIO GPIO_NUM_2
#endif
#ifndef LED2_GPIO
#define LED2_GPIO GPIO_NUM_4
#endif
#ifndef LED3_GPIO
#define LED3_GPIO GPIO_NUM_5
#endif
#ifndef LED4_GPIO
#define LED4_GPIO GPIO_NUM_18
#endif
#ifndef LED5_GPIO
#define LED5_GPIO GPIO_NUM_19
#endif

static const gpio_num_t led_pins[LED_COUNT] = {
	LED1_GPIO, LED2_GPIO, LED3_GPIO, LED4_GPIO, LED5_GPIO
};

static inline void led_init(void)
{
	for (int i = 0; i < LED_COUNT; ++i) {
		gpio_reset_pin(led_pins[i]);
		gpio_set_direction(led_pins[i], GPIO_MODE_OUTPUT);
		gpio_set_level(led_pins[i], 0);
	}
}

static inline void led_set(uint8_t index, bool on)
{
	if (index < LED_COUNT) {
		gpio_set_level(led_pins[index], on ? 1 : 0);
	}
}

static inline void led_set_all(bool on)
{
	for (uint8_t i = 0; i < LED_COUNT; ++i) {
		led_set(i, on);
	}
}
