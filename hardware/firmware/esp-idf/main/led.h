#ifndef FAULT_INDICATORS_LED_H
#define FAULT_INDICATORS_LED_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LED_1 = 0,
    LED_2,
    LED_3,
    LED_4,
    LED_COUNT
} led_id_t;

void led_init(void);
void led_set(led_id_t led, bool on);
void led_on(led_id_t led);
void led_off(led_id_t led);
void led_all_on(void);
void led_all_off(void);

#ifdef __cplusplus
}
#endif

#endif // FAULT_INDICATORS_LED_H
