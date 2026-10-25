#ifndef LCD_1602A_H
#define LCD_1602A_H

#include "i2c.h"

// LCD Address
#define LCD_DEVICE_ADDRESS        0x27

// Public Function Prototypes
void label_current_temp();
void label_target_temp();
void update_current_temp(float current_temp);
void update_target_temp(float target_temp);
void lcd_init_sequence(i2c_master_dev_handle_t dev_handle);
void lcd_print_str(i2c_master_dev_handle_t dev_handle, const char *str);
void lcd_set_cursor(i2c_master_dev_handle_t dev_handle, uint8_t col, uint8_t row);

#endif // LCD_1602A_H