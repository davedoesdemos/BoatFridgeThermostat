#ifndef CONTROLS_H
#define CONTROLS_H

#include "button_gpio.h"

#define BUTTON_PLUS_GPIO_NUM 36
#define BUTTON_MINUS_GPIO_NUM 37
#define BUTTON_LIGHT_GPIO_NUM 35
#define RELAY_GPIO_NUM 17

void buttons_init();
void call_cooling(int call_for_cooling);
void relay_init();

#endif // CONTROLS_H