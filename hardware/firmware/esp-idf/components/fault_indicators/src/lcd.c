#include "lcd.h"

#include <stdbool.h>
#include "driver/i2c_master.h"
#include "esp_check.h"
#include "esp_log.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "board.h"
#include "i2c_driver.h"

#define LCD_RS 0x01
#define LCD_ENABLE 0x04
#define LCD_BACKLIGHT 0x08
#define LCD_TIMEOUT_MS 100



static const char *TAG = "lcd";
static i2c_port_t lcd_port;
static uint8_t lcd_address;
static bool lcd_ready;

/* Standard PCF8574 backpack mapping. RW stays low for all writes. */
static void lcd_write_nibble(uint8_t nibble, bool is_data) {
    uint8_t value = (uint8_t)((nibble & 0x0F) << 4) |
                    LCD_BACKLIGHT | (is_data ? LCD_RS : 0);
    const uint8_t pulse[] = {value, value | LCD_ENABLE, value};
    ESP_ERROR_CHECK(i2c_driver_write(
        lcd_port,
        lcd_address,
        pulse,
        sizeof(pulse)
    ));

    esp_rom_delay_us(50);
}

static void lcd_write_byte(uint8_t value, bool is_data) {
    lcd_write_nibble(value >> 4, is_data);
    lcd_write_nibble(value & 0x0F, is_data);
}

static esp_err_t lcd_connect(void) {
    const i2c_driver_config_t config = {
        .port = I2C_DRIVER_MASTER_PORT_DEFAULT,
        .sda_io_num = I2C_DRIVER_SDA_IO_DEFAULT,
        .scl_io_num = I2C_DRIVER_SCL_IO_DEFAULT,
        .clk_speed_hz = I2C_DRIVER_CLOCK_SPEED_DEFAULT,
        .pullup_enable = true,
    };

    esp_err_t err = i2c_driver_init(&config);
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        return err;
    }

    const uint8_t addresses[] = {0x27, 0x3F};

    for (size_t i = 0; i < sizeof(addresses); i++) {
        err = i2c_driver_probe(config.port, addresses[i]);
        if (err == ESP_OK) {
            lcd_port = config.port;
            lcd_address = addresses[i];
            return ESP_OK;
        }
    }

    return ESP_ERR_NOT_FOUND;
}

void lcd_init(void) {
    if (lcd_ready) return;
    vTaskDelay(pdMS_TO_TICKS(50));
    esp_err_t err = lcd_connect();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "LCD unavailable (%s): check wiring, power and backpack address", esp_err_to_name(err));
        return;
    }
    lcd_write_nibble(0x03, false);
    vTaskDelay(pdMS_TO_TICKS(10));
    lcd_write_nibble(0x03, false);
    esp_rom_delay_us(150);
    lcd_write_nibble(0x03, false);
    lcd_write_nibble(0x02, false);
    lcd_ready = true;
    lcd_send_command(0x28); /* 4-bit, two lines, 5x8 font. */
    lcd_send_command(0x08);
    lcd_send_command(0x01);
    lcd_send_command(0x06);
    lcd_send_command(0x0C);
}

void lcd_send_command(uint8_t command) {
    if (!lcd_ready) return;
    lcd_write_byte(command, false);
    if (command == 0x01 || command == 0x02) esp_rom_delay_us(2000);
}

void lcd_print(const char *str) {
    if (!lcd_ready || str == NULL) return;
    while (*str != '\0') lcd_write_byte((uint8_t)*str++, true);
}
