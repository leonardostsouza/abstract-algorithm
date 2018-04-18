// Implements symmetric interaction combinators with infinite node colors.

#ifndef __INTERACTION_COMBINATORS_H__
#define __INTERACTION_COMBINATORS_H__

#include <stdlib.h>
#include <stdint.h>
//#include <stddef.h>   // <---- Need to uncomment this line if buffet_t changes
                        // to size_t* type

#define MAX_NODES       10
#define NODE_SIZE       4   // Do not change this value!
                            // Each node must have 1 KIND and 3 ports for the
                            // evaluator to run optimaly

#define BUFFER_MAX_SIZE     (MAX_NODES * NODE_SIZE)

typedef enum {PORT_0 = 0, PORT_1, PORT_2} port_t;
typedef uint32_t kind_t;    // This value is shifted to the left so the 2 LSB may
                            // contain the "meta"
typedef uint8_t meta_t      // only the 2 LSB are relevant
typedef uint32_t index_t    // Maybe will need to change to size_t* in the future
                            // in order to support bigger buffers.
/*typedef struct {
    index_t next; // first "never writen" buffer position
    index_t buffer[BUFFER_MAX_SIZE];
} buffer_t;*/
typedef index_t* buffer_t;


// Allocs memory to buffer and writes 0 to all positions
void bufferInit(buffer_t);

void setKind(buffer_t, index_t nodeIndex, kind_t kind);
kind_t getKind(buffer_t, index_t nodeIndex);

void setMeta(buffer_t, index_t nodeIntex, meta_t meta);
meta_t getMeta(buffer_t, index_t nodeIndex);

// returns port type (0, 1 or 2) based on the index
port_t getPort(index_t bufferIndex);
index_t readPort(buffer_t, index_t nodeIndex, port_t port);
index_t getPortIndex(index_t nodeIndex, port_t port);

index_t getNodeIndex(index_t vectorIndex);

// Writes a value in a node port
void setPort(buffer_t, index_t nodeIndex, port_t localPort, index_t remotePortIndex);

// Returns what is on the other "side" of a given port of a node pair
index_t flip(buffer_t, index_t nodeIndex, port_t port);

// returns the node index given the port index in the buffer
index_t getNodeIndex(index_t portIndex);

// writes a node to the first unused position on the buffer
void createNode(buffer_t, kind_t kind);

//binds two ports given their indexes in the buffer
void link(buffer_t, index_t fstPortIndex, index_t sndPortIndex);

// Does the same as link(), but receives nodes indexes and ports as arguments
void createWire(buffer_t, index_t fstNodeIndex, port_t fstNodePort, index_t sndNodeIndex, port_t sndNodePort);

//@TODO : explain the reduce function
void reduce(buffer_t);

//@TODO : explain the rewrite function
void rewrite(buffer_t);

#endif /*__INTERACTION_COMBINATORS_H__*/
