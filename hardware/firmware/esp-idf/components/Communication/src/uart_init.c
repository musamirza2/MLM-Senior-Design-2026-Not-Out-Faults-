#include "uart_drivers.h"
#include "board.h"
#include "peripherals.h"

static bool valid_port(uart_port_t port)
{
    return port >= UART_NUM_0 && port < UART_NUM_MAX;
}

esp_err_t uart_init(void)
{
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
    return uart_driver_install(UART_PORT_NUM, RX_BUF_SIZE, 0, 0, NULL, 0);
}

// Success means accepted by the driver, not necessarily fully sent on the wire.
esp_err_t uart_write(uart_port_t uart_num, const uint8_t *data, uint16_t length)
{
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
              TickType_t timeout_ticks)
{
    if (!valid_port(uart_num) || data == NULL || length == 0 ||
        !uart_is_driver_installed(uart_num)) {
        return -1;
    }
    return uart_read_bytes(uart_num, data, length, timeout_ticks);
}
