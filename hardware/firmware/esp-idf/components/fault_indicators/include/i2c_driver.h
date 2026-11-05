/**
 * @file i2c_driver.h
 * @brief I2C driver interface for fault indicators.
 *
 * This header file defines the interface for the I2C driver used in the fault indicators project.
 * It provides functions to initialize and deinitialize the I2C driver, as well as to read from and write to I2C devices.
 *
 * @note This driver is designed to work with the ESP-IDF framework and is compatible with ESP32-S series chip.
 */

#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "esp_err.h"
#include "peripherals.h"


/**
 * @brief Initialize the I2C driver with the specified configuration.
 *
 * @param config Pointer to the I2C driver configuration structure.
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t i2c_driver_init(const i2c_driver_config_t *config);

/*
 * @brief Deinitialize the I2C driver for the specified port.
 *
 * @param port I2C port to deinitialize.
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t i2c_driver_deinit(i2c_port_t port);

/**
 * @brief Probe an I2C device at the specified address.
 *
 * @param port I2C port to use.
 * @param address Address of the I2C device to probe.
 * @return ESP_OK if the device is found, or an error code on failure.
 */
esp_err_t i2c_driver_probe(i2c_port_t port, uint8_t address);


/**
 * @brief Write data to an I2C device.
 *
 * @param port I2C port to use.
 * @param device_addr Address of the I2C device.
 * @param data Pointer to the data to write.
 * @param len Length of the data to write.
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t i2c_driver_write(i2c_port_t port, uint8_t device_addr, const uint8_t *data, size_t len);

/**
 * @brief Read data from an I2C device.
 *
 * @param port I2C port to use.
 * @param device_addr Address of the I2C device.
 * @param data Pointer to the buffer to store the read data.
 * @param len Length of the data to read.
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t i2c_driver_read(i2c_port_t port, uint8_t device_addr, uint8_t *data, size_t len);

/**
 * @brief Write data to an I2C device register.
 *
 * @param port I2C port to use.
 * @param device_addr Address of the I2C device.
 * @param reg_addr Address of the register to write.
 * @param data Pointer to the data to write.
 * @param len Length of the data to write.
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t i2c_driver_write_reg(i2c_port_t port, uint8_t device_addr, uint8_t reg_addr, const uint8_t *data, size_t len);

/**
 * @brief Read data from an I2C device register.
 *
 * @param port I2C port to use.
 * @param device_addr Address of the I2C device.
 * @param reg_addr Address of the register to read.
 * @param data Pointer to the buffer to store the read data.
 * @param len Length of the data to read.
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t i2c_driver_read_reg(i2c_port_t port, uint8_t device_addr, uint8_t reg_addr, uint8_t *data, size_t len);


#endif /* I2C_DRIVER_H */
