#ifndef ADS1115_H
#define ADS1115_H

#include "hardware.h"

#define ADS1115_1_ADDR   0x48
#define ADS1115_2_ADDR   0x49
#define ADS1115_1        0
#define ADS1115_2        1

int socketsInit(uint8_t index);
void probeADS1115(node_object_t *handle);
void socketsUpdate(node_object_t *handle, uint8_t index);

#endif
