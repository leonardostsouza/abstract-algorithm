// Implements symmetric interaction combinators with infinite node colors.
#include "interaction-combinators.h"

// Allocs memory to buffer and writes 0 to all positions
void bufferInit(buffer_t buf){
    calloc(MAX_BUFFER_SIZE, sizeof(index_t));
}

void bufferReset(buffer_t buf){
    memset(buf, 0, sizeof buf);
}

index_t getKindIndex(index_t nodeIndex) {
    return (index_t)((nodeIndex << 2) | 0x3);
}

inline void setKind(buffer_t buf, index_t nodeIndex, kind_t kind){
    buf[getKindIndex(nodeIndex)] = (kind << 2);
}

kind_t getKind(buffer_t buf, index_t nodeIndex){
    return ((buf[getKindIndex(nodeIndex)]) >> 2);
}

inline void setMeta(buffer_t buf, index_t nodeIndex, meta_t meta){
    buf[getKindIndex(nodeIndex)] |= (meta & 0x3);
}

meta_t getMeta(buffer_t buf, index_t nodeIndex){
    return (buf[getKindIndex(nodeIndex)] & 0x3);
}

// returns port type (0, 1 or 2) based on the index
// WARNING! The behavior is undefined if this method receives the
// index of a node kind!
port_t getPortType(index_t bufferIndex){
    return (port_t)(bufferIndex & 0x3);
}

index_t getPortIndex(index_t nodeIndex, port_t port){
    return ((nodeIndex << 2) | (index_t)port);
}

index_t getPortValue(buffer_t buf, index_t nodeIndex, port_t port){
    return buf[getPortIndex(nodeIndex, port)];
}

// Writes a value in a node port
inline void setPortValue(buffer_t buf, index_t nodeIndex, port_t localPort, index_t remotePortIndex){
    buf[getPortIndex(nodeIndex, localPort)] = remotePortIndex;
}

// Returns what is on the other "side" of a given port of a node pair
index_t flip(buffer_t buf, index_t nodeIndex, port_t port){
    return buf[getPortValue(buf, nodeIndex, port)];
}

// returns the node index given an index in the buffer
index_t getNodeIndex(index_t bufferIndex){
    return (bufferIndex >> 2);
}

//binds two ports given their indexes in the buffer
inline void link(buffer_t buf, index_t fstPortIndex, index_t sndPortIndex){
    buf[fstPortIndex] = sndPortIndex;
    buf[sndPortIndex] = fstPortIndex;
}

// Does the same as link(buffer_t, index_t, index_t), but receives nodes indexes and ports as arguments
inline void linkNodes(buffer_t buf, index_t fstNodeIndex, port_t fstNodePort, index_t sndNodeIndex, port_t sndNodePort){
    link(buf, getPortIndex(fstNodeIndex, fstNodePort), getPortIndex(sndNodeIndex, sndNodePort));
}

// writes a node to the first unused position on the buffer
uint32_t createNode(buffer_t buf, kind_t kind){
    // check the end of the buffer to know where the new node should be placed
    index_t newNodeIndex = buf[MAX_BUFFER_SIZE];

    if (newNodeIndex < MAX_NODES){
        // All good. We can add another node

        // kind
        setKind(buf, newNodeIndex, kind);

        // loops on all ports
        linkNodes(buf, newNodeIndex, PORT_0, newNodeIndex, PORT_0);
        linkNodes(buf, newNodeIndex, PORT_1, newNodeIndex, PORT_1);
        linkNodes(buf, newNodeIndex, PORT_2, newNodeIndex, PORT_2);

        buf[MAX_BUFFER_SIZE] += 1;

        return 0; // success
    }
    return 1; // MAX_NODES reached
}

//@TODO : explain the reduce function
void reduce(buffer_t buf){
    // TODO
}

//@TODO : explain the rewrite function
void rewrite(buffer_t buf){
    // TODO
}
