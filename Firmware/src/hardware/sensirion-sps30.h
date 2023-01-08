#ifndef SENSIRION_SPS30_H
#define SENSIRION_SPS30_H

void probeSPS30(node_object_t *handle);
int sps30Init(node_object_t *handle);
int sps30Update(node_object_t *handle);

#endif