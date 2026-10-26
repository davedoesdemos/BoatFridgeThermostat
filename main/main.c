#include <stdio.h>
#include <esp_err.h>
#include <esp_log.h>
#include <driver/i2c_master.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "lcd_1602a.h"
#include "state.h"
#include "i2c.h"
#include "ds18b20.h"
#include "tempprobe.h"

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

    tempprobe_init();
    queue_init();
    
    update_target_temp(2.3);

    xTaskCreate(
        temperature_sender,        // Task function
        "temperature_sender",      // Task name string
        4096,               // Stack size in bytes
        NULL,               // Parameters passed to the task
        1,                  // Task priority
        NULL               // Task handle (not needed here)
    );
        xTaskCreate(
        queue_reader,        // Task function
        "Queue Reader",      // Task name string
        4096,               // Stack size in bytes
        NULL,               // Parameters passed to the task
        1,                  // Task priority
        NULL               // Task handle (not needed here)
    );
}
