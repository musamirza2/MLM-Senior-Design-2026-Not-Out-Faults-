#include <stdio.h>   // snprintf
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include "esp_log.h"
#include "uart_drivers.h"
#include "board.h"
#include "peripherals.h"
#include "model.h"
#include "lcd.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

typedef struct {
    double features[21];
} prediction_request_t;

static QueueHandle_t prediction_queue = NULL;

static bool valid_port(uart_port_t port) {
    return port >= UART_NUM_0 && port < UART_NUM_MAX;
}

esp_err_t uart_init(void) {
    // Reject unconfirmed wiring instead of silently retaining a pin mapping.
    if (!valid_port(UART_PORT_NUM) || !GPIO_IS_VALID_OUTPUT_GPIO(UART_TX_PIN) ||
        !GPIO_IS_VALID_GPIO(UART_RX_PIN) || UART_TX_PIN == UART_RX_PIN) {
        return ESP_ERR_INVALID_ARG;
    }
    if (uart_is_driver_installed(UART_PORT_NUM)) {
        return ESP_ERR_INVALID_STATE;
    }
    const uart_config_t config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    esp_err_t err = uart_param_config(UART_PORT_NUM, &config);
    if (err != ESP_OK) {
        return err;
    }
    err = uart_set_pin(UART_PORT_NUM, UART_TX_PIN, UART_RX_PIN,
                       UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    if (err != ESP_OK) {
        return err;
    }

    prediction_queue = xQueueCreate(4, sizeof(prediction_request_t));
    if (prediction_queue == NULL) {
        return ESP_ERR_NO_MEM;
    }

    return uart_driver_install(UART_PORT_NUM, RX_BUF_SIZE, 0, 0, NULL, 0);
}

// Success means accepted by the driver, not necessarily fully sent on the wire.
esp_err_t uart_write(uart_port_t uart_num, const uint8_t *data, uint16_t length) {
    if (!valid_port(uart_num) || data == NULL || length == 0) {
        return ESP_ERR_INVALID_ARG;
    }
    if (!uart_is_driver_installed(uart_num)) {
        return ESP_ERR_INVALID_STATE;
    }
    int written = uart_write_bytes(uart_num, data, length);
    return written == length ? ESP_OK : ESP_FAIL;
}

int uart_read(uart_port_t uart_num, uint8_t *data, uint16_t length,
              TickType_t timeout_ticks) {
    if (!valid_port(uart_num) || data == NULL || length == 0 ||
        !uart_is_driver_installed(uart_num)) {
        return -1;
    }
    return uart_read_bytes(uart_num, data, length, timeout_ticks);
}


// Accept exactly 21 finite numbers separated by commas, with optional whitespace.
static bool parse_features(const char *line, double features[21]) {
    const char *cursor = line;
    for (int i = 0; i < 21; ++i) {
        char *end;
        errno = 0;
        double value = strtod(cursor, &end);
        if (end == cursor || errno == ERANGE || !isfinite(value)) {
            return false;
        }
        features[i] = value;
        while (isspace((unsigned char)*end)) {
            ++end;
        }
        if (i < 20) {
            if (*end != ',') {
                return false;
            }
            cursor = end + 1;
        } else if (*end != '\0') {
            return false;
        }
    }
    return true;
}

void uart_rx_task(void *pvParameters) {
    (void)pvParameters;

    uint8_t rx_buffer[128];
    prediction_request_t request;
    char line[768];  // Holds 21 comma-separated doubles, including exponents.
    size_t used = 0;
    bool discarding = false;

    while (true) {
        int len = uart_read(UART_PORT_NUM, rx_buffer, sizeof(rx_buffer),
                        pdMS_TO_TICKS(1000));

        for (int i = 0; i < len; ++i) {
            char c = (char)rx_buffer[i];

            if (c == '\n') {
                if (!discarding && used > 0) {
                    line[used] = '\0';
                    if (parse_features(line, request.features)) {
                        xQueueSend(prediction_queue, &request, portMAX_DELAY);
                    } else {
                        ESP_LOGW("prediction", "Rejected input: expected 21 comma-separated finite values");
                    }
                }
            used = 0;
            discarding = false;
            } else if (!discarding) {
                if (used < sizeof(line) - 1) {
                    line[used++] = c;
                } else {
                    discarding = true;
                }
            }
        }
    }
}

void lcd_prediction_task(void *pvParameters)
{
    (void)pvParameters;
    prediction_request_t request;
    double scores[11];
    char text[24];

    // Test the compiled model/scaler independently of MATLAB and UART parsing.
    static const double healthy_test[21] = {
        131608.5624, 131656.2994, 131358.866,
        131541.2401, 80.26836693, 105.8769446,
        195.9159698, 196.8741283, 197.0134168,
        196.6001291, 0.2006591722, 666.7508644,
        671.7602582, 668.7333702, 0.7795063949,
        0.5582098138, 0.2261141782, 0.001020646187,
        0.00396493328, 0.0006102144608, 0.0008048954417
    };
    double test_scaled[21];
    for (int i = 0; i < 21; ++i) {
        test_scaled[i] = (healthy_test[i] - feature_mean[i]) / feature_scale[i];
    }
    score(test_scaled, scores);
    bool test_valid = true;
    for (int i = 0; i < 11; ++i) {
        if (!isfinite(scores[i])) test_valid = false;
    }
    int test_prediction = 0;
    for (int i = 1; i < 11; ++i) {
        if (scores[i] > scores[test_prediction]) test_prediction = i;
    }
    ESP_LOGI("prediction", "DIAG-v1: built %s %s; healthy self-test=%d (expected 10)",
             __DATE__, __TIME__, test_prediction);
    ESP_LOGI("prediction", "Self-test score[10]=%.9g (expected about 10.6761)", scores[10]);
    lcd_send_command(0x80);
    lcd_print("DIAG2 SELF TEST  ");
    lcd_send_command(0xC0);
    if (test_valid) {
        snprintf(text, sizeof(text), "Got:%2d Want:10  ", test_prediction);
        lcd_print(text);
    } else {
        lcd_print("MODEL NaN/Inf   ");
    }

    while (true) {
        if (xQueueReceive(prediction_queue, &request,
                          portMAX_DELAY) == pdTRUE) {
            double scaled[21];
            bool values_valid = true;

            for (int i = 0; i < 21; ++i) {
                scaled[i] = (request.features[i] - feature_mean[i])
                            / feature_scale[i];
                if (!isfinite(scaled[i])) values_valid = false;
                ESP_LOGI("prediction", "feature[%d]: raw=%.17g scaled=%.17g",
                         i, request.features[i], scaled[i]);
            }
            if (!values_valid) {
                lcd_send_command(0x80);
                lcd_print("SCALING NaN/Inf ");
                ESP_LOGE("prediction", "Invalid scaled input; prediction skipped");
                continue;
            }

            score(scaled, scores);
            for (int i = 0; i < 11; ++i) {
                if (!isfinite(scores[i])) values_valid = false;
                ESP_LOGI("prediction", "score[%d]=%.17g", i, scores[i]);
            }
            if (!values_valid) {
                lcd_send_command(0x80);
                lcd_print("MODEL NaN/Inf   ");
                ESP_LOGE("prediction", "Invalid model score; prediction skipped");
                continue;
            }

            int prediction = 0;
            for (int i = 1; i < 11; ++i) {
                if (scores[i] > scores[prediction]) {
                    prediction = i;
                }
            }

            lcd_send_command(0x80); // First row
            snprintf(text, sizeof(text), "Prediction: %-5d", prediction);
            lcd_print(text);

            lcd_send_command(0xC0); // Second row: first received value
            snprintf(text, sizeof(text), "X0:%12.4g", request.features[0]);
            lcd_print(text);
        }
    }
}
