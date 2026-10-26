#ifndef TEMPPROBE_H
#define TEMPPROBE_H

#define TEMP_READ_FREQUENCY_MS 2000

void tempprobe_init(void);
void temperature_sender();

#endif // TEMPPROBE_H