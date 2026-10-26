#ifndef TEMPPROBE_H
#define TEMPPROBE_H

#define TEMP_READ_FREQUENCY_MS 2000
#define ONEWIRE_BUS_GPIO    20   // Assigned data pin

void tempprobe_init(void);
void temperature_sender();

#endif // TEMPPROBE_H