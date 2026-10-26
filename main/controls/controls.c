#include <esp_err.h>
#include <esp_log.h>
#include "controls.h"
#include "iot_button.h"
#include "state.h"

// Logging
static const char *TAG = "BFT: Controls";

button_handle_t button_plus = NULL;
button_handle_t button_minus = NULL;
button_handle_t button_light = NULL;

static void button_plus_click_cb(void *arg, void *usr_data)
{
    telemetry_packet_t packet;
    packet.id = TOPIC_BUTTON_PRESS;
    packet.value.value_int = 1;
    xQueueSend(msg_queue, &packet, portMAX_DELAY);
}

static void button_minus_click_cb(void *arg, void *usr_data)
{
    telemetry_packet_t packet;
    packet.id = TOPIC_BUTTON_PRESS;
    packet.value.value_int = 2;
    xQueueSend(msg_queue, &packet, portMAX_DELAY);
}

static void button_light_click_cb(void *arg, void *usr_data)
{
    telemetry_packet_t packet;
    packet.id = TOPIC_BUTTON_PRESS;
    packet.value.value_int = 3;
    xQueueSend(msg_queue, &packet, portMAX_DELAY);
}
static void button_light_long_click_cb(void *arg, void *usr_data)
{
    telemetry_packet_t packet;
    packet.id = TOPIC_BUTTON_PRESS;
    packet.value.value_int = 4;
    xQueueSend(msg_queue, &packet, portMAX_DELAY);
}

void buttons_init()
{
    // create gpio button
    const button_config_t btn_cfg = {0};
    const button_gpio_config_t btn_plus_cfg = {
        .gpio_num = BUTTON_PLUS_GPIO_NUM,
        .active_level = 0,
    };
    const button_gpio_config_t btn_minus_cfg = {
        .gpio_num = BUTTON_MINUS_GPIO_NUM,
        .active_level = 0,
    };
    const button_gpio_config_t btn_light_cfg = {
        .gpio_num = BUTTON_LIGHT_GPIO_NUM,
        .active_level = 0,
    };
    
    esp_err_t ret = iot_button_new_gpio_device(&btn_cfg, &btn_plus_cfg, &button_plus);
    if(NULL == button_plus) {
        ESP_LOGE(TAG, "Button create failed");
    }
    
    esp_err_t ret2 = iot_button_new_gpio_device(&btn_cfg, &btn_minus_cfg, &button_minus);
    if(NULL == button_minus) {
        ESP_LOGE(TAG, "Button create failed");
    }
    
    esp_err_t ret3 = iot_button_new_gpio_device(&btn_cfg, &btn_light_cfg, &button_light);
    if(NULL == button_light) {
        ESP_LOGE(TAG, "Button create failed");
    }

    iot_button_register_cb(button_plus, BUTTON_SINGLE_CLICK, NULL, button_plus_click_cb,NULL);
    iot_button_register_cb(button_plus, BUTTON_LONG_PRESS_HOLD, NULL, button_plus_click_cb,NULL);
    iot_button_register_cb(button_minus, BUTTON_SINGLE_CLICK, NULL, button_minus_click_cb,NULL);
    iot_button_register_cb(button_minus, BUTTON_LONG_PRESS_HOLD, NULL, button_minus_click_cb,NULL);
    iot_button_register_cb(button_light, BUTTON_SINGLE_CLICK, NULL, button_light_click_cb,NULL);
    iot_button_register_cb(button_light, BUTTON_LONG_PRESS_UP, NULL, button_light_long_click_cb,NULL);
}