#ifndef SENSIRION_SCD4X_H
#define SENSIRION_SCD4X_H

#include "hardware.h"

void probeSDC4X(node_object_t *handle);

int sdc4xUpdate(node_object_t *handle);

#endif
