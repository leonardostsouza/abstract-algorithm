#ifndef __NODE_H__
#define __NODE_H__

#include <stdlib.h>
#include <stdint.h>

#define KIND 0

typedef enum node_ports {
    PORT_1 = 1,
    PORT_2,
    PORT_3
} port_t;

// A node is an array of 4 uin32_t values.
// It has the format:
// KIND | PORT_1 | PORT_2 | PORT_3
//
//      KIND -> A number wich represents the kind of the node.
//              This value is importan for graph reduction.
//              The 30 Most Significant Bits (MSB) represent the 'Kind'
//              number and the 2 Least Significant Bits (LSB) represent the ???
//
//      PORT_X-> Represents a link with another node by port X. Each node
//               has exactly 3 pots. Each of them links to a
//               different node. The 2 LSB represent the port and the
//               30 MSB is a pointer to the node on the other side of
//               the edge

// <PROBLEMA> É realmente necessário armazenar o valor da porta?
// Alterar o valor de um ponteiro na mão pode causar problemas com o
// mapeamento da memória virtual do SO.
//
// <SOLUÇÃO 1> A porta poderia ser codificada pela posição em que o
// ponteiro se encontra. Ex.: EDGE_2 = porta num 2
//
// <SOLUÇÃO 2> Em vez de armazenar o ponteiro, armazenar o índice no vetor
// do "node" de destino <=== Usar esta!
typedef uint32_t node_t[4];

void nodeInit(node_t *node, uint32_t kind, uint32_t edge1, uint32_t edge2, uint32_t edge3);

void setKind(node_t *node, uint32_t kind);
uint32_t getKind(node_t node);

void setPort(node_t *orig, port_t origPort, uint32_t destIndex);
port_t getPort(node_t node, port_t port);

#endif /*__NODE_H__*/
