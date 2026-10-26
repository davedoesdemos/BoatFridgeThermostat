#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "onewire_bus.h"
#include "ds18b20.h"
#include "tempprobe.h"
#include "lcd_1602a.h"
#include "state.h"

// Logging
static const char *TAG = "BFT: Temp Probe";

#define ONEWIRE_BUS_GPIO    20   // Assigned data pin

static ds18b20_device_handle_t ds18b20_handle = NULL;

void tempprobe_init(void)
{
    // Configure the 1-Wire Bus
    onewire_bus_config_t bus_config = {
        .bus_gpio_num = ONEWIRE_BUS_GPIO,
    };
    // Set up RMT configuration for timing precision
    onewire_bus_rmt_config_t rmt_config = {
        .max_rx_bytes = 10, // Sufficient for scratchpad reads (9 bytes needed)
    };
    // Create the 1-Wire bus handle using the RMT backend
    onewire_bus_handle_t bus_handle;
    ESP_ERROR_CHECK(onewire_new_bus_rmt(&bus_config, &rmt_config, &bus_handle));
    ESP_LOGI(TAG, "1-Wire bus created successfully on GPIO %d", ONEWIRE_BUS_GPIO);
    // Create a 1-Wire device iterator to discover connected sensors
    onewire_device_iter_handle_t iter_handle;
    ESP_ERROR_CHECK(onewire_new_device_iter(bus_handle, &iter_handle));
    onewire_device_t next_onewire_device;
    ds18b20_handle = NULL;
    // Search the bus for the first available device
    if (onewire_device_iter_get_next(iter_handle, &next_onewire_device) == ESP_OK) {
        ds18b20_config_t ds18b20_config = {};
        
        // FIX: The library validates the device family code directly within this call.
        // If it returns ESP_OK, a matching DS18B20 has been verified and instantiated.
        if (ds18b20_new_device_from_enumeration(&next_onewire_device, &ds18b20_config, &ds18b20_handle) == ESP_OK) {
            
            // Fetch the verified unique address out of the generated handle
            onewire_device_address_t device_address;
            ESP_ERROR_CHECK(ds18b20_get_device_address(ds18b20_handle, &device_address));
            ESP_LOGI(TAG, "Found DS18B20 sensor with address: %016llX", device_address);
            // Set resolution to maximum precision (12-bit)
            ESP_ERROR_CHECK(ds18b20_set_resolution(ds18b20_handle, DS18B20_RESOLUTION_12B));
        }
    }

    // Clean up the iterator structure once discovery is finished
    ESP_ERROR_CHECK(onewire_del_device_iter(iter_handle));
    if (ds18b20_handle == NULL) {
        ESP_LOGE(TAG, "No valid DS18B20 sensor detected. Verify wiring or pull-up strength.");
        return;
    }

}

void temperature_sender() {
    telemetry_packet_t packet;
    packet.id = TOPIC_TEMPERATURE_READING;
    float temperature = 0.0f;
    while (1) {
        // Trigger a global temperature conversion sequence on the bus
        ESP_ERROR_CHECK(ds18b20_trigger_temperature_conversion(ds18b20_handle));
        vTaskDelay(pdMS_TO_TICKS(800));

        // Read out the calculated temperature from the scratchpad
        if (ds18b20_get_temperature(ds18b20_handle, &temperature) == ESP_OK) {
            packet.value.value_float = temperature;
            xQueueSend(msg_queue, &packet, portMAX_DELAY);
        } else {
            ESP_LOGE(TAG, "Failed to read data. Checking CRC...");
        }

        vTaskDelay(pdMS_TO_TICKS(TEMP_READ_FREQUENCY_MS)); // Read data every 2 seconds
    }
}
