#ifndef STATE_H
#define STATE_H

#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#define MAX_TARGET_TEMP 30.0f
#define MIN_TARGET_TEMP 0.0f
#define TEMP_BUTTON_INCREMENT 0.1f
#define STARTING_TARGET_TEMP 22.0f

// System State
typedef struct {
    float current_temp;
    float target_temp;
    int call_for_cooling; // 0 = off, 1 = on
} system_state_t;

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

extern system_state_t system_state;
extern QueueHandle_t msg_queue;

void queue_init();
void queue_reader();

#endif // STATE_H