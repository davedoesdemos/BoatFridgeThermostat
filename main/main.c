#include <stdio.h>
#include <esp_err.h>
#include <esp_log.h>
#include <driver/i2c_master.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "lcd_1602a.h"
#include "i2c.h"

static const char *TAG = "LCD_1602A";

void app_main(void)
{
    // 1. Initialize Bus
    if (init_i2c_bus(&i2c_settings.global_bus_handle) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize I2C Master Bus!");
        return;
    }
    ESP_LOGI(TAG, "I2C Master Bus initialized successfully.");

    // 2. Initialize Device
    if (init_i2c_device(LCD_DEVICE_ADDRESS ,i2c_settings.global_bus_handle, &i2c_settings.lcd_dev_handle) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to attach I2C peripheral device!");
        return;
    }
    ESP_LOGI(TAG, "I2C Device (0x%02X) registered.", LCD_DEVICE_ADDRESS);
        // 3. Initialize the actual screen hardware using the handle
    lcd_init_sequence(i2c_settings.lcd_dev_handle);
    ESP_LOGI(TAG, "LCD sequence completed.");
    // 4. Print text out to the physical display

    update_target_temp(2.3);
    update_current_temp(14.5);
    update_current_temp(-13.4);
    update_current_temp(-3.3);
    update_current_temp(-3.2);
    update_current_temp(-3.1);
    update_current_temp(-3.0);
    update_current_temp(-2.9);
    update_current_temp(-2.8);
    update_current_temp(-2.7);
    update_current_temp(-2.6);
    update_current_temp(-2.5);
    update_current_temp(-2.4);
    update_current_temp(-2.3);
    update_current_temp(-2.2);
    update_current_temp(-2.1);
    //lcd_set_cursor(i2c_settings.lcd_dev_handle, 0, 0); // Column 0, Row 0 (Line 1)
    //lcd_print_str(i2c_settings.lcd_dev_handle, "Hello Jude");
    //lcd_set_cursor(i2c_settings.lcd_dev_handle, 0, 1); // Column 0, Row 1 (Line 2)
    //lcd_print_str(i2c_settings.lcd_dev_handle, "Dave did this");
}
