#include "state.h"
#include "lcd_1602a.h"

// Logging
static const char *TAG = "BFT: State";

system_state_t system_state = {
    .current_temp = 0.0f,
    .target_temp = STARTING_TARGET_TEMP,
    .call_for_cooling = 0
};

QueueHandle_t msg_queue = NULL;

void queue_init() {
// Create global message queue
    msg_queue = xQueueCreate(20, sizeof(telemetry_packet_t));
}

void cooling_check() {
    //ESP_LOGI(TAG, "Cooling Check: Current Temp = %.4f, Target Temp = %.4f", system_state.current_temp, system_state.target_temp);
    if (system_state.current_temp >= system_state.target_temp + TEMP_HYSTERESIS) {
        system_state.call_for_cooling = 1;
    } else if (system_state.current_temp <= system_state.target_temp - TEMP_HYSTERESIS) {
        system_state.call_for_cooling = 0;
    }
    label_cooling(system_state.call_for_cooling);
}

void queue_reader() 
{
    telemetry_packet_t packet;
        // Pull every single pending message off the queue
    while (1){
        while (xQueueReceive(msg_queue, &packet, 0) == pdTRUE) {
            // Route data using the telemetry ID
            switch (packet.id) {
    // -----Sensors-----
                case TOPIC_TEMPERATURE_READING:
                    system_state.current_temp = packet.value.value_float;
                    update_current_temp(system_state.current_temp);
                    break;
    // -----Buttons-----
                case TOPIC_BUTTON_PRESS:
                    switch (packet.value.value_int) {
                        case 1:
                            if (system_state.target_temp < MAX_TARGET_TEMP) {
                                system_state.target_temp += TEMP_BUTTON_INCREMENT;
                                update_target_temp(system_state.target_temp);
                            }
                            break;
                        case 2:
                            if (system_state.target_temp > MIN_TARGET_TEMP) {
                                system_state.target_temp -= TEMP_BUTTON_INCREMENT;
                                update_target_temp(system_state.target_temp);
                            }
                            break;
                        case 3:
                            backlight_timer(i2c_settings.lcd_dev_handle);
                            break;
                        case 4:
                            backlight_toggle(i2c_settings.lcd_dev_handle);
                            break;
                    }
                    break;
    // -----Default-----
                default:
                    // Unknown packet ID safety catch
                    ESP_LOGW(TAG, "Unknown packet topic found");
                    break;
            }
        }
        cooling_check();
    }

}