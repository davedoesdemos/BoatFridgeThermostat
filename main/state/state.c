#include "state.h"
#include "lcd_1602a.h"

// Logging
static const char *TAG = "BFT: State";

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
                    update_current_temp(packet.value.value_float); // Update the LCD with the current temperature
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