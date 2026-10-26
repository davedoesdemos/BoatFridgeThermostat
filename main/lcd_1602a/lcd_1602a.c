#include <stdio.h>
#include <driver/i2c_master.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "lcd_1602a.h"

// Logging
static const char *TAG = "BFT: LCD 1602A";

// PCF8574 Pin Mapping Constants
#define LCD_BACKLIGHT               0x08
#define LCD_ENABLE                  0x04
#define LCD_REGISTER_SELECT         0x01

void label_target_temp(){
    lcd_set_cursor(i2c_settings.lcd_dev_handle, 0, 0);
    lcd_set_cursor(i2c_settings.lcd_dev_handle, 0, 0);
    lcd_print_str(i2c_settings.lcd_dev_handle, "Target  :");
}

void update_target_temp(float target_temp){
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "% 5.1fC", target_temp);
    lcd_set_cursor(i2c_settings.lcd_dev_handle, 10, 0);
    lcd_set_cursor(i2c_settings.lcd_dev_handle, 10, 0);
    lcd_print_str(i2c_settings.lcd_dev_handle, buffer);
}

void label_current_temp(){
    lcd_set_cursor(i2c_settings.lcd_dev_handle, 0, 1);
    lcd_set_cursor(i2c_settings.lcd_dev_handle, 0, 1);
    lcd_print_str(i2c_settings.lcd_dev_handle, "Current :");
}

void update_current_temp(float current_temp){
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "% 5.1fC", current_temp);
    lcd_set_cursor(i2c_settings.lcd_dev_handle, 10, 1);
    lcd_set_cursor(i2c_settings.lcd_dev_handle, 10, 1);
    lcd_print_str(i2c_settings.lcd_dev_handle, buffer);
}

void label_cooling(int call_for_cooling){
    lcd_set_cursor(i2c_settings.lcd_dev_handle, 10, 0);
    lcd_set_cursor(i2c_settings.lcd_dev_handle, 10, 0);
    if (call_for_cooling == 1) {
        lcd_print_str(i2c_settings.lcd_dev_handle, "*");
    } else {
        lcd_print_str(i2c_settings.lcd_dev_handle, " ");
    }
}

// Low-level function to transmit a byte to the PCF8574 expander
void lcd_send_expander(i2c_master_dev_handle_t dev_handle, uint8_t data) {
    uint8_t buffer = data | LCD_BACKLIGHT;
    i2c_master_transmit(dev_handle, &buffer, 1, 1000);
}

// Pulses the Enable pin to latch the data into the LCD
void lcd_pulse_enable(i2c_master_dev_handle_t dev_handle, uint8_t data) {
    lcd_send_expander(dev_handle, data | LCD_ENABLE);
    vTaskDelay(pdMS_TO_TICKS(1));
    lcd_send_expander(dev_handle, data & ~LCD_ENABLE);
    vTaskDelay(pdMS_TO_TICKS(1));
}

// Sends a byte split into two 4-bit nibbles
void lcd_send(i2c_master_dev_handle_t dev_handle, uint8_t value, uint8_t mode) {
    uint8_t high_nibble = value & 0xF0;
    uint8_t low_nibble = (value << 4) & 0xF0;
    
    lcd_pulse_enable(dev_handle, high_nibble | mode);
    lcd_pulse_enable(dev_handle, low_nibble | mode);
}

// Send a raw command to the LCD
void lcd_send_cmd(i2c_master_dev_handle_t dev_handle, uint8_t cmd) {
    lcd_send(dev_handle, cmd, 0);
}

// Send a character byte to the LCD
void lcd_send_data(i2c_master_dev_handle_t dev_handle, uint8_t data) {
    lcd_send(dev_handle, data, LCD_REGISTER_SELECT);
}

// Bootstraps the LCD into 4-bit operating mode
void lcd_init_sequence(i2c_master_dev_handle_t dev_handle) {
    vTaskDelay(pdMS_TO_TICKS(100)); 
    
    // Step 1: Force HD44780 into 4-bit mode via specific sequence
    lcd_pulse_enable(dev_handle, 0x30);
    vTaskDelay(pdMS_TO_TICKS(10));
    lcd_pulse_enable(dev_handle, 0x30);
    vTaskDelay(pdMS_TO_TICKS(5));
    lcd_pulse_enable(dev_handle, 0x30);
    vTaskDelay(pdMS_TO_TICKS(5));
    lcd_pulse_enable(dev_handle, 0x20); // Switch to 4-bit interface mode

    // Step 2: Configure display parameters
    lcd_send_cmd(dev_handle, 0x28); // 2 lines, 5x8 matrix format
    lcd_send_cmd(dev_handle, 0x0C); // Display ON, Cursor OFF, Blink OFF
    lcd_send_cmd(dev_handle, 0x06); // Increment cursor position automatically
    lcd_send_cmd(dev_handle, 0x01); // Clear display screen memory
    vTaskDelay(pdMS_TO_TICKS(5));

    label_target_temp();
    label_current_temp();
}

// Loops through a string to print it out
void lcd_print_str(i2c_master_dev_handle_t dev_handle, const char *str) {
    while (*str) {
    vTaskDelay(pdMS_TO_TICKS(100));
        lcd_send_data(dev_handle, (uint8_t)*str++);
    }
}

// Helper to set cursor position: row 0 (line 1) or row 1 (line 2)
void lcd_set_cursor(i2c_master_dev_handle_t dev_handle, uint8_t col, uint8_t row) {
    uint8_t row_offsets[] = {0x00, 0x40};
    lcd_send_cmd(dev_handle, 0x80 | (col + row_offsets[row]));
}