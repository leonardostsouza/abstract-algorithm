#ifndef __NODE_VECTOR_H__
#define __NODE_VECTOR_H__

#define LOGARITHMIC_GROWTH

#include <stdlib.h>
#include <stddef.h>
#include "vector.h"
#include "node.h"

void addNode(node_t *node);
node_t getNode(size_t index);
void modifyEdge(size_t origNodeIndex, size_t destNodeIndex, port_t origPort, port_t destPort);
void removeNode(size_t nodeIndex);

#endif /*__NODE_VECTOR_H__*/
