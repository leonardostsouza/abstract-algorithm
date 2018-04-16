#include <stdlib.h>
#include "headers/node.h"

node_t createNode(uint32_t kind, uint32_t edge_1, uint32_t edge_2, uint32_t edge_3){
    node_t newNode = {kind, edge_1, edge_2, edge_3};
    return newNode;
}

void setKind(node_t *node, uint32_t kind){
    *node[KIND] = kind;
}

uint32_t getKind(node_t node){
    return node[kind];
}

void setPort(node_t *orig, uint32_t destIndex, port_t origPort){
    *orig[origPort] = destIndex;
}

uint32_t getPort(node_t node, port_t port){
    return node[port];
}
