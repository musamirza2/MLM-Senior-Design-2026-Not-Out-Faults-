
#ifndef BOARD_H
#define BOARD_H

#include "driver/gpio.h"
#include "driver/i2c_master.h"

// Define GPIO pins for LEDs
#define BOARD_LED_1_GPIO GPIO_NUM_6
#define BOARD_LED_2_GPIO GPIO_NUM_7
#define BOARD_LED_3_GPIO GPIO_NUM_16
#define BOARD_LED_4_GPIO GPIO_NUM_17
#define BOARD_LED_5_GPIO GPIO_NUM_18

//

#define UART_BAUD_RATE   115200 // Match the console during initial CH343 testing.
#define RX_BUF_SIZE      1024

// ESP32-S3 UART0 connected to the CH343 USB-to-UART bridge.
#define UART_TX_PIN      GPIO_NUM_43
#define UART_RX_PIN      GPIO_NUM_44

// Default I2C master port
#define I2C_DRIVER_MASTER_PORT_DEFAULT    I2C_NUM_0

// Pins for SCL and SDA. These can be overridden in the application code.
#define I2C_DRIVER_SCL_IO_DEFAULT         GPIO_NUM_9
#define I2C_DRIVER_SDA_IO_DEFAULT         GPIO_NUM_21


#endif // BOARD_H
