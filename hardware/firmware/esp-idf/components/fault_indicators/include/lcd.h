#ifndef LCD_H
#define LCD_H

#include <stdint.h>


void lcd_init(void);

void lcd_send_command(uint8_t command);

/**/
void lcd_print(const char *str);

#endif // LCD_H
