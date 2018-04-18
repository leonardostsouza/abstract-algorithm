// Implements symmetric interaction combinators with infinite node colors.
#include "interaction-combinators.h"

// Allocs memory to buffer and writes 0 to all positions
void bufferInit(buffer_t){
    // TODO
}

void setKind(buffer_t, index_t nodeIndex, kind_t kind){
    // TODO
}

kind_t getKind(buffer_t, index_t nodeIndex){
    // TODO
}

void setMeta(buffer_t, index_t nodeIntex, meta_t meta){
    // TODO
}

meta_t getMeta(buffer_t, index_t nodeIndex){
    // TODO
}

// returns port type (0, 1 or 2) based on the index
port_t getPort(index_t bufferIndex){
    // TODO
}

index_t readPort(buffer_t, index_t nodeIndex, port_t port){
    // TODO
}

index_t getPortIndex(index_t nodeIndex, port_t port){
    // TODO
}

index_t getNodeIndex(index_t vectorIndex){
    // TODO
}

// Writes a value in a node port
void setPort(buffer_t, index_t nodeIndex, port_t localPort, index_t remotePortIndex){
    // TODO
}

// Returns what is on the other "side" of a given port of a node pair
index_t flip(buffer_t, index_t nodeIndex, port_t port){
    // TODO
}

// returns the node index given the port index in the buffer
index_t getNodeIndex(index_t portIndex){
    // TODO
}

// writes a node to the first unused position on the buffer
void createNode(buffer_t, kind_t kind){
    // TODO
}

//binds two ports given their indexes in the buffer
void link(buffer_t, index_t fstPortIndex, index_t sndPortIndex){
    // TODO
}

// Does the same as link(), but receives nodes indexes and ports as arguments
void createWire(buffer_t, index_t fstNodeIndex, port_t fstNodePort, index_t sndNodeIndex, port_t sndNodePort){
    // TODO
}

//@TODO : explain the reduce function
void reduce(buffer_t){
    // TODO
}

//@TODO : explain the rewrite function
void rewrite(buffer_t){
    // TODO
}
