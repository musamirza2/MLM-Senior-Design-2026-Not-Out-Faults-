#include "i2c_driver.h"
#include <stdlib.h>
#include <string.h>

#define I2C_DRIVER_TIMEOUT_MS 1000
static i2c_master_bus_handle_t buses[I2C_NUM_MAX];
static uint32_t bus_speeds[I2C_NUM_MAX];

// check if the given I2C port is in the valid range
static bool valid_port(i2c_port_t port) {
    return port >= I2C_NUM_0 && port < I2C_NUM_MAX;
}

esp_err_t i2c_driver_init(const i2c_driver_config_t *config) {
    // Validate the configuration parameters
    if (config == NULL || !valid_port(config->port) ||
        !GPIO_IS_VALID_OUTPUT_GPIO(config->sda_io_num) ||
        !GPIO_IS_VALID_OUTPUT_GPIO(config->scl_io_num) ||
        config->sda_io_num == config->scl_io_num || config->clk_speed_hz == 0) {
        return ESP_ERR_INVALID_ARG;
    }
    if (buses[config->port] != NULL) return ESP_ERR_INVALID_STATE;
    
    const i2c_master_bus_config_t bus_config = {
        .i2c_port = config->port,
        .sda_io_num = config->sda_io_num,
        .scl_io_num = config->scl_io_num,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = config->pullup_enable,
    };
    esp_err_t err = i2c_new_master_bus(&bus_config, &buses[config->port]);
    if (err == ESP_OK) bus_speeds[config->port] = config->clk_speed_hz;
    return err;
}

esp_err_t i2c_driver_deinit(i2c_port_t port) {
    if (!valid_port(port)) return ESP_ERR_INVALID_ARG;
    if (buses[port] == NULL) return ESP_ERR_INVALID_STATE;
    esp_err_t err = i2c_del_master_bus(buses[port]);
    if (err == ESP_OK) {
        buses[port] = NULL;
        bus_speeds[port] = 0;
    }
    return err;
}

esp_err_t i2c_driver_probe(i2c_port_t port, uint8_t address) {
    if (!valid_port(port) || address > 0x7F) {
        return ESP_ERR_INVALID_ARG;
    }

    if (buses[port] == NULL) {
        return ESP_ERR_INVALID_STATE;
    }
    return i2c_master_probe(buses[port], address, I2C_DRIVER_TIMEOUT_MS);
}

/* Callers must serialize transfers with bus initialization/deinitialization. */
static esp_err_t transfer(i2c_port_t port, uint8_t address,
                          const uint8_t *tx, size_t tx_len, uint8_t *rx, size_t rx_len) {

    if (!valid_port(port) || address > 0x7F || (tx_len == 0 && rx_len == 0) ||
        (tx_len > 0 && tx == NULL) || (rx_len > 0 && rx == NULL)) {
        return ESP_ERR_INVALID_ARG;
    }
    if (buses[port] == NULL) return ESP_ERR_INVALID_STATE;
    const i2c_device_config_t config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = address,
        .scl_speed_hz = bus_speeds[port],
    };
    i2c_master_dev_handle_t device;
    esp_err_t err = i2c_master_bus_add_device(buses[port], &config, &device);
    if (err != ESP_OK) return err;
    if (tx_len && rx_len) {
        err = i2c_master_transmit_receive(device, tx, tx_len, rx, rx_len, I2C_DRIVER_TIMEOUT_MS);
    } else if (tx_len) {
        err = i2c_master_transmit(device, tx, tx_len, I2C_DRIVER_TIMEOUT_MS);
    } else {
        err = i2c_master_receive(device, rx, rx_len, I2C_DRIVER_TIMEOUT_MS);
    }
    esp_err_t cleanup_err = i2c_master_bus_rm_device(device);
    return err == ESP_OK ? cleanup_err : err;
}

esp_err_t i2c_driver_write(i2c_port_t port, uint8_t address, const uint8_t *data, size_t len) {
    return transfer(port, address, data, len, NULL, 0);
}

esp_err_t i2c_driver_read(i2c_port_t port, uint8_t address, uint8_t *data, size_t len) {
    return transfer(port, address, NULL, 0, data, len);
}

esp_err_t i2c_driver_write_reg(i2c_port_t port, uint8_t address, uint8_t reg, const uint8_t *data, size_t len) {
    if (!valid_port(port) || address > 0x7F || data == NULL || len == 0 || len == SIZE_MAX) return ESP_ERR_INVALID_ARG;
    uint8_t *buffer = malloc(len + 1);
    if (buffer == NULL) return ESP_ERR_NO_MEM;
    buffer[0] = reg;
    memcpy(buffer + 1, data, len);
    esp_err_t err = i2c_driver_write(port, address, buffer, len + 1);
    free(buffer);
    return err;
}

esp_err_t i2c_driver_read_reg(i2c_port_t port, uint8_t address, uint8_t reg, uint8_t *data, size_t len) {
    if (data == NULL || len == 0) return ESP_ERR_INVALID_ARG;
    return transfer(port, address, &reg, sizeof(reg), data, len);
}
