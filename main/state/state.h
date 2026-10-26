#ifndef STATE_H
#define STATE_H

#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// Unique identifiers for each packet type
typedef enum {
// -----Sensors-----
    TOPIC_TEMPERATURE_READING,
// -----Buttons-----
    TOPIC_BUTTON_PRESS
} telemetry_id_t;

// Telemetry Packet Struct
typedef struct {
    telemetry_id_t id;
    union {
        float value_float;
        int value_int;
    } value;
} telemetry_packet_t;

extern QueueHandle_t msg_queue;

void queue_init();
void queue_reader();

#endif // STATE_H