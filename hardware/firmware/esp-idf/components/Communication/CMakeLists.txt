idf_component_register(
    SRCS "src/uart_init.c"
    INCLUDE_DIRS "include"
    REQUIRES esp_driver_uart drivers
)
