#ifndef EEPROM_H
#define EEPROM_H

#include "hardware.h"

#define EEPROM_SIZE 66

void provisionDevice();
void readProvisionedData(node_object_t *handle);
void eraseProvisionedData();

#endif