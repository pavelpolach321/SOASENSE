#ifndef OPCN3_H
#define OPCN3_H

#include "hardware.h"

void probeOPCN3(node_object_t *handle);
void opcn3_reset();
int opcn3Init(node_object_t *handle);
void opcn3PowerOn(void); 
void opcn3PowerOff(void) ;
float calculateFloat(uint32_t val);
void opcn3ReadPm(node_object_t *handle);
void opcn3ReadFlush(void);

void opcn3ReadAll(node_object_t *handle);

#endif
