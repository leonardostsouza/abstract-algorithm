#include <stdlib.h>
#include "headers/node.h"

void nodeInit(node_t *node, uint32_t kind, uint32_t edge_1, uint32_t edge_2, uint32_t edge_3){
    (*node)[0] = kind;
    (*node)[1] = edge_1;
    (*node)[2] = edge_2;
    (*node)[3] = edge_3;
}

void setKind(node_t *node, uint32_t kind){
    (*node)[KIND] = kind;
}

uint32_t getKind(node_t node){
    return node[KIND];
}

void setPort(node_t *orig, port_t origPort, uint32_t destIndex){
    (*orig)[origPort] = destIndex;
}

uint32_t getPort(node_t node, port_t port){
    return node[port];
}
