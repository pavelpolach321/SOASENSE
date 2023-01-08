#ifndef BME280_H
#define BME280_H

#include "hardware.h"

#define BMEADDR                    0x76

int bme280Init(void);
void probeBME280(node_object_t *handle);
int bme280Update(node_object_t *handle);

#endif
