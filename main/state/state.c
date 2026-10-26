#include "state.h"
#include "lcd_1602a.h"

// Logging
static const char *TAG = "BFT: State";

system_state_t system_state = {
    .current_temp = 0.0f,
    .target_temp = 4.0f
};

QueueHandle_t msg_queue = NULL;

void queue_init() {
// Create global message queue
    msg_queue = xQueueCreate(20, sizeof(telemetry_packet_t));
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
                            if (system_state.target_temp < 10.0f) {
                                system_state.target_temp += 0.1f;
                                update_target_temp(system_state.target_temp);
                            }
                            break;
                        case 2:
                            if (system_state.target_temp > 0.0f) {
                                system_state.target_temp -= 0.1f;
                                update_target_temp(system_state.target_temp);
                            }
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
    }

}