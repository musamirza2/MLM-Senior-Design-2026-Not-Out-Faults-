idf_component_register(
    SRCS "src/Fault_Indicators_Led.c" "src/i2c_driver.c" "src/lcd.c"
    INCLUDE_DIRS "include"
    REQUIRES esp_driver_gpio esp_driver_i2c drivers
    PRIV_REQUIRES esp_rom
)
