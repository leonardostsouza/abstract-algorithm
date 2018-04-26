// Implements symmetric interaction combinators with infinite node colors.

// A node is composed of 4 uin32_t values.
// It has the format:
// PORT_0 | PORT_1 | PORT_2 | KIND
//
//      KIND -> A number wich represents the kind of the node.
//              This value is important for graph reduction.
//              The 30 Most Significant Bits (MSB) represent the 'Kind'
//              number and the 2 Least Significant Bits (LSB) represent the Met.
//
//              Meta is not an important value by itself. It is only used by the
//              reduce and rewrite functions to mark nodes it has already visited
//
//      PORT_X-> Represents a link with another node by port X. Each node
//               has exactly 3 pots. Each of them links to a port in a
//               different node. This position in memory holds the foreign port
//               index in the buffer.

// The buffer is composed by an array of index_t values. The last value in the
// array holds the index to the next "free" prosition a new node needs to occupy.
// The position right before the "NEXT" is hold information about the entry point
// of the graph. Somewhere in the graph, there is a port pointing to itself and
// it is used as a stating point for the "reduce" method.
//
// Consequently, the buffer can be epresented as:
//
// NODE_1 | NODE_2 | ... | NODE_n | ENTRY_POINT | NEXT
//
//      NODE_X      -> The representation of a node in memory. Remember each node
//                     has the format PORT_0 | PORT_1 | PORT_2 | KIND
//
//      ENTRY_POINT -> The starting point for the "reduce" function
//
//      NEXT        -> The last position of the buffer. Holds the next free index
//                     in which the buffer can be written. Also, the value of
//                     NEXT/4 represents the number of nodes in the network
//

#ifndef __INTERACTION_COMBINATORS_H__
#define __INTERACTION_COMBINATORS_H__

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MAX_NODES       1024
#define NODE_SIZE       4   // Do not change this value!
                            // Each node must have 1 KIND and 3 ports for the
                            // evaluator to run optimaly

#define MAX_BUFFER_SIZE ((MAX_NODES * NODE_SIZE) + 2)
#define NEXT            (MAX_BUFFER_SIZE - 1)
#define ENTRY_POINT     (NEXT - 1)

#define REDUCE_BUFFER_SIZE 256 // Buffer used by method reduce() to store steps

typedef enum {PORT_0 = 0, PORT_1, PORT_2} port_t;
#define kind_t uint32_t
#define meta_t uint32_t     // only the 2 LSB are relevant
#define index_t uint32_t    // Maybe will need to change to size_t* in the future
                            // in order to support bigger buffers.

typedef index_t *buffer_t;

typedef struct stats_t {
    uint32_t loops;
    uint32_t rewrites;
    uint32_t betas;
    uint32_t dupls;
    uint32_t annis;
} stats_t;

void statsReset(stats_t *stats);

// Allocs memory to buffer and writes 0 to all positions
void bufferInit(buffer_t *);
void bufferReset(buffer_t);
void freeBuffer(buffer_t);
uint8_t populateBuffer(char *netFileName, buffer_t buf);

index_t getKindIndex(index_t nodeIndex);
void setKind(buffer_t, index_t nodeIndex, kind_t kind);
kind_t getKind(buffer_t, index_t nodeIndex);

void setMeta(buffer_t, index_t nodeIndex, meta_t meta);
meta_t getMeta(buffer_t, index_t nodeIndex);

// returns port type (0, 1 or 2) based on the index
// WARNING! The behavior is undefined if this method receives the
// index of a node kind!
port_t getPortType(index_t bufferIndex);
index_t getPortIndex(index_t nodeIndex, port_t port);
index_t getPortValue(buffer_t, index_t nodeIndex, port_t port);

// Writes a value in a node port
void setPortValue(buffer_t, index_t nodeIndex, port_t localPort, index_t remotePortIndex);

// Returns what is on the other "side" of a given port index
index_t flip(buffer_t buf, index_t portIndex);

// Returns what is on the other "side" of a given node port
index_t flip_np(buffer_t, index_t nodeIndex, port_t port);

// returns the node index given an index in the buffer
index_t getNodeIndex(index_t bufferIndex);

//binds two ports given their indexes in the buffer
void link(buffer_t, index_t fstPortIndex, index_t sndPortIndex);

// Does the same as link(), but receives nodes indexes and ports as arguments
void linkNodes(buffer_t, index_t fstNodeIndex, port_t fstNodePort, index_t sndNodeIndex, port_t sndNodePort);

// writes a node to the first unused position on the buffer
uint32_t createNode(buffer_t, kind_t kind, index_t *newNodeIndex);

// This walks through the graph looking for redexes, following the logical flow
// of information, in such a way that only redexes that interfere on the normal
// form are reduced.
void reduce(buffer_t, stats_t *);

// This performs the reduction of redexes. It, thus, implements annihilation
// and commutation, as described on Lafont's paper on interaction combinators.
// It is the heart of algorithm. In theory, the reduce() function above isn't
// necessary; you could just store an array of redexes and keep applying this
// function on them. You'd lose the lazy aspect of the algorithm, but you could,
// in turn, perform reductions in parallel. There is an inherent tradeoff
// between laziness and parallelization, because, by reducing nodes in parallel,
// you inevitably reduce redexes which do not influence on the normal form.
void rewrite(buffer_t, index_t nodeAIndex, index_t nodeBIndex, stats_t *);

#endif /*__INTERACTION_COMBINATORS_H__*/
