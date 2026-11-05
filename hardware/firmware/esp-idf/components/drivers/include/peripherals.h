#ifndef PERIPHERALS_H
#define	PERIPHERALS_H

#include <stdbool.h>
#include <stdint.h>
#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "driver/uart.h"

// CH343 USB-to-UART bridge uses UART0.
#define UART_PORT_NUM    UART_NUM_0

// I2C

// Default clock speed for I2C bus. This can be overridden in the application code.
#define I2C_DRIVER_CLOCK_SPEED_DEFAULT    100000

// Define a structure to represent the I2C configuration
typedef struct {
    i2c_port_t port;
    gpio_num_t sda_io_num;
    gpio_num_t scl_io_num;
    uint32_t clk_speed_hz;
    bool pullup_enable;
} i2c_driver_config_t;


#endif // PERIPHERALS_H
