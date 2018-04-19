// Implements symmetric interaction combinators with infinite node colors.
#include "interaction-combinators.h"

// Allocs memory to buffer and writes 0 to all positions
void bufferInit(buffer_t *buf){
    *buf = (buffer_t) calloc(MAX_BUFFER_SIZE, sizeof(index_t));
    if (buf == NULL) exit (1);
}

void bufferReset(buffer_t buf){
    memset(buf, 0, sizeof buf);
}

index_t getKindIndex(index_t nodeIndex) {
    return (index_t)((nodeIndex << 2) | 0x3);
}

void setKind(buffer_t buf, index_t nodeIndex, kind_t kind){
    buf[getKindIndex(nodeIndex)] = (kind << 2);
}

kind_t getKind(buffer_t buf, index_t nodeIndex){
    return ((buf[getKindIndex(nodeIndex)]) >> 2);
}

void setMeta(buffer_t buf, index_t nodeIndex, meta_t meta){
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
void setPortValue(buffer_t buf, index_t nodeIndex, port_t localPort, index_t remotePortIndex){
    buf[getPortIndex(nodeIndex, localPort)] = remotePortIndex;
}

// Returns what is on the other "side" of a given port index
index_t flip(buffer_t buf, index_t portIndex){
    return buf[portIndex];
}

// Returns what is on the other "side" of a given node port
index_t flip_np(buffer_t buf, index_t nodeIndex, port_t port){
    return flip(buf, getPortIndex(nodeIndex, port));
}

// returns the node index given an index in the buffer
index_t getNodeIndex(index_t bufferIndex){
    return (bufferIndex >> 2);
}

//binds two ports given their indexes in the buffer
void link(buffer_t buf, index_t fstPortIndex, index_t sndPortIndex){
    buf[fstPortIndex] = sndPortIndex;
    buf[sndPortIndex] = fstPortIndex;
}

// Does the same as link(buffer_t, index_t, index_t), but receives nodes indexes and ports as arguments
void linkNodes(buffer_t buf, index_t fstNodeIndex, port_t fstNodePort, index_t sndNodeIndex, port_t sndNodePort){
    link(buf, getPortIndex(fstNodeIndex, fstNodePort), getPortIndex(sndNodeIndex, sndNodePort));
}

// writes a node to the first unused position on the buffer
uint32_t createNode(buffer_t buf, kind_t kind, index_t *newNodeIndex){
    // check the end of the buffer to know where the new node should be placed
    *newNodeIndex = buf[NEXT];

    if (*newNodeIndex < MAX_NODES){
        // All good. We can add another node

        // kind
        setKind(buf, *newNodeIndex, kind);

        // loops on all ports
        linkNodes(buf, *newNodeIndex, PORT_0, *newNodeIndex, PORT_0);
        linkNodes(buf, *newNodeIndex, PORT_1, *newNodeIndex, PORT_1);
        linkNodes(buf, *newNodeIndex, PORT_2, *newNodeIndex, PORT_2);

        buf[NEXT] += 1;

        return 0; // success
    }
    return 1; // MAX_NODES reached
}

// This walks through the graph looking for redexes, following the logical flow
// of information, in such a way that only redexes that interfere on the normal
// form are reduced.
void reduce(buffer_t buf /*, stats * */)){
    index_t visit = buf[ENTRY_POINT];
    index_t prev, next, back;
    //resetStats();
    while(**** ??? ****){
        //++stats.loops;
        prev = **** ??? ****;
        next = flip(buf, prev);
        prev = flip(buf, next);

        if(getMeta(buf, getNodeIndex(prev)) != 3){
            if(getPortType(prev) == PORT_0){
                if((getPortType(next) == 0) &&
                (getNodeIndex(next) != getNodeIndex(prev))) {
                    // ++stats.rewrites;
                    if((getKind(buf, getNodeIndex(next)) == 1) &&
                    (getKind(buf, getNodeIndex(prev)))) {
                        // ++stats.betas;
                    }
                    back = flip(buf, getPortIndex(getNodeIndex(next), getMeta(buf, getNodeIndex(next))));
                    rewrite(buf, getNodeIndex(next), getNodeIndex(prev)/*, &stats*/);

                    **** visit.push(flip(buf, back)); ****
                }
                else {
                    setMeta(buf, getNodeIndex(prev), 3);
                    **** visit.push(flip(buf, getPortIndex(getNodeIndex(prev), PORT_2))); ****
                    **** visit.push(flip(buf, getPortIndex(getNodeIndex(prev), PORT_1))); ****
                }
                else {
                    setMeta(buf, getNodeIndex(prev), getPortType(prev));
                    **** visit.push(flip(buf, getPortIndex(getNodeIndex(prev), PORT_0))); ****
                }
            }
        }
    }
}

// This performs the reduction of redexes. It, thus, implements annihilation
// and commutation, as described on Lafont's paper on interaction combinators.
// It is the heart of algorithm. In theory, the reduce() function above isn't
// necessary; you could just store an array of redexes and keep applying this
// function on them. You'd lose the lazy aspect of the algorithm, but you could,
// in turn, perform reductions in parallel. There is an inherent tradeoff
// between laziness and parallelization, because, by reducing nodes in parallel,
// you inevitably reduce redexes which do not influence on the normal form.
void rewrite(buffer_t buf, index_t nodeAIndex, index_t nodeBIndex){
    if (getKind(buf, nodeAIndex) == getKind(buf, nodeBIndex)) {
        //  a          b            a   b
        //   \        /              \ /
        //     A -- B       -->       X
        //   /        \              / \
        //  c          d            c   d
        link(buf, flip_np(buf, nodeAIndex, PORT_1)),  flip_np(buf, nodeBIndex, PORT_1)));
        link(buf, flip_np(buf, nodeAIndex, PORT_2)),  flip_np(buf, nodeBIndex, PORT_2)));
    }
    else {
        //  a          d       a - B1 --- A1 - d
        //   \        /              \ /
        //     A -- B     -->         X
        //   /        \              / \
        //  b          c       b - B2 --- A2 - c

        index_t *A1, *A2, *B1, *B2;

        // create new nodes
        createNode(buf, getKind(nodeAIndex), A1);
        createNode(buf, getKind(nodeAIndex), A2);
        createNode(buf, getKind(nodeBIndex), B1);
        createNode(buf, getKind(nodeBIndex), B2);

        // link new nodes with orphan nodes
        link(buf, getPortIndex(*B1, PORT_0), flip(buf, nodeAIndex, PORT_1));
        link(buf, getPortIndex(*B2, PORT_0), flip(buf, nodeAIndex, PORT_2));
        link(buf, getPortIndex(*A1, PORT_0), flip(buf, nodeBIndex, PORT_1));
        link(buf, getPortIndex(*A2, PORT_0), flip(buf, nodeBIndex, PORT_2));

        // Link new nodes with other new nodes
        linkNodes(buf, *A1, PORT_1, *B1, PORT_1);
        linkNodes(buf, *A1, PORT_2, *B2, PORT_1);
        linkNodes(buf, *A2, PORT_1, *B1, PORT_2);
        linkNodes(buf, *A2, PORT_2, *B2, PORT_2);

        // Unlink "old" nodes
        setKind(buf, nodeAIndex, 0);
        linkNodes(buf, nodeAIndex, PORT_0, nodeAIndex, PORT_0);
        linkNodes(buf, nodeAIndex, PORT_1, nodeAIndex, PORT_1);
        linkNodes(buf, nodeAIndex, PORT_2, nodeAIndex, PORT_2);

        setKind(buf, nodeBIndex, 0);
        linkNodes(buf, nodeBIndex, PORT_0, nodeBIndex, PORT_0);
        linkNodes(buf, nodeBIndex, PORT_1, nodeBIndex, PORT_1);
        linkNodes(buf, nodeBIndex, PORT_2, nodeBIndex, PORT_2);
    }
}
