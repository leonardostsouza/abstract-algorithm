// Implements symmetric interaction combinators with infinite node colors.
#include "interaction-combinators.h"

#ifdef __DEBUG__
#include <stdio.h>
#endif /*__DEBUG__*/

#define REDUCE_BUFFER_SIZE 256 // Buffer used by method reduce() to store steps

#ifdef __DEBUG__
static void printBuffer(buffer_t *buffer){
    index_t i;
    printf("[");
    index_t lastValidIndex = (*buffer)[NEXT];
    for (i = 0; i < lastValidIndex; i++){
        printf("%d,", (*buffer)[i]);
    }
    printf("] ===> SIZE = %d\n\n\n", lastValidIndex);
}

static void printNode(buffer_t buf, char *nodeName, index_t nodeIndex){
    printf("%s: P0 = %d, P1 = %d, P2 = %d, KIND = %d\n",
            nodeName,
            getPortValue(buf, nodeIndex, PORT_0),
            getPortValue(buf, nodeIndex, PORT_1),
            getPortValue(buf, nodeIndex, PORT_2),
            getKind(buf, nodeIndex));
}
#endif /*__DEBUG__*/

void statsReset(stats_t *stats){
    stats->loops = 0;
    stats->rewrites = 0;
    stats->betas = 0;
    stats->dupls = 0;
    stats->annis = 0;
}

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
    //buf[getKindIndex(nodeIndex)] += (meta & 0x3);
    buf[getKindIndex(nodeIndex)] = (buf[getKindIndex(nodeIndex)] & 0xFFFFFFFC) | meta;
    #ifdef __DEBUG__
    printf("Meta of node %d set to %d.\n",
            nodeIndex,
            getMeta(buf, nodeIndex));
    #endif /*__DEBUG__*/
}

meta_t getMeta(buffer_t buf, index_t nodeIndex){
    return (meta_t)(buf[getKindIndex(nodeIndex)] & 0x3);
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
uint32_t createNode(buffer_t buf, kind_t kind, index_t *newNodeIndexPtr){
    // check the end of the buffer to know where the new node should be placed
    index_t newNodeIndex = getNodeIndex(buf[NEXT]);

    if(newNodeIndexPtr != NULL) {
        *newNodeIndexPtr = newNodeIndex;
    }

    if (newNodeIndex < MAX_NODES){
        // All good. We can add another node
        // kind
        setKind(buf, newNodeIndex, kind);

        // loops on all ports
        linkNodes(buf, newNodeIndex, PORT_0, newNodeIndex, PORT_0);
        linkNodes(buf, newNodeIndex, PORT_1, newNodeIndex, PORT_1);
        linkNodes(buf, newNodeIndex, PORT_2, newNodeIndex, PORT_2);

        buf[NEXT] += NODE_SIZE;

        return 0; // success
    }
    return 1; // MAX_NODES reached
}

static inline index_t pop(index_t *buffer, uint32_t *stack_size){
    *stack_size -= 1;
    return buffer[*stack_size];
}

static inline void push(index_t *buffer, uint32_t *stack_size, index_t value){
    buffer[*stack_size] = value;
    *stack_size += 1;
}

// This walks through the graph looking for redexes, following the logical flow
// of information, in such a way that only redexes that interfere on the normal
// form are reduced.
void reduce(buffer_t buf , stats_t *stats) {
    index_t prev = 0, next = 0, back = 0;
    uint32_t stack_size = 0;
    index_t stack[REDUCE_BUFFER_SIZE];

    #ifdef __DEBUG__
    printf("Original buffer:\n");
    printBuffer(&buf);
    #endif /*__DEBUG__*/

    statsReset(stats);
    push(stack, &stack_size, buf[ENTRY_POINT]);

    while(stack_size > 0){
        ++stats->loops;

        prev = pop(stack, &stack_size);
        next = flip(buf, prev);
        prev = flip(buf, next);

        #ifdef __DEBUG__
        printf("===========> Loop number %d: prev = %d, next = %d\n",
                stats->loops, prev, next);
        #endif /*__DEBUG__*/

        if(getMeta(buf, getNodeIndex(prev)) == (meta_t)0x3) {
            #ifdef __DEBUG__
            printf("Node %d meta == %d. Proceeding to next loop...\n",
                    getNodeIndex(prev),
                    getMeta(buf, getNodeIndex(prev)));
            #endif /*__DEBUG__*/
        }
        else {
            if(getPortType(prev) == PORT_0) {
                if((getPortType(next) == 0) &&
                (getNodeIndex(next) != getNodeIndex(prev))) {
                    ++stats->rewrites;

                    #ifdef __DEBUG__
                    printf("Executing rewrite number %d\n", stats->rewrites);
                    #endif /*__DEBUG__*/

                    if ((getKind(buf, getNodeIndex(next)) == 1) &&
                        (getKind(buf, getNodeIndex(prev)) == 1)) {
                        ++stats->betas;
                        #ifdef __DEBUG__
                        printf("Beta number %d found\n", stats->betas);
                        #endif /*__DEBUG__*/
                    }
                    back = flip(buf, getPortIndex(getNodeIndex(next), getMeta(buf, getNodeIndex(next))));
                    rewrite(buf, getNodeIndex(next), getNodeIndex(prev), stats);

                    #ifdef __DEBUG__
                    printf("New \"back\" = %d\n", back);
                    #endif /*__DEBUG__*/

                    push(stack, &stack_size, flip(buf, back));
                }
                else {
                    setMeta(buf, getNodeIndex(prev), 3);
                    push(stack, &stack_size, flip(buf, getPortIndex(getNodeIndex(prev), PORT_2)));
                    push(stack, &stack_size, flip(buf, getPortIndex(getNodeIndex(prev), PORT_1)));
                }
            }
            else {
                setMeta(buf, getNodeIndex(prev), getPortType(prev));
                push(stack, &stack_size, flip(buf, getPortIndex(getNodeIndex(prev), PORT_0)));
            }
        }
    }
    #ifdef __DEBUG__
    printf("\n\nReduced buffer:\n");
    printBuffer(&buf);
    #endif /*__DEBUG__*/
}

// This performs the reduction of redexes. It, thus, implements annihilation
// and commutation, as described on Lafont's paper on interaction combinators.
// It is the heart of algorithm. In theory, the reduce() function above isn't
// necessary; you could just store an array of redexes and keep applying this
// function on them. You'd lose the lazy aspect of the algorithm, but you could,
// in turn, perform reductions in parallel. There is an inherent tradeoff
// between laziness and parallelization, because, by reducing nodes in parallel,
// you inevitably reduce redexes which do not influence on the normal form.

void rewrite(buffer_t buf, index_t A, index_t B, stats_t *stats){
    if (getKind(buf, A) == getKind(buf, B)) {
        /*/  a          b            a   b
        //   \        /              \ /
        //     A -- B       -->       X
        //   /        \              / \
        //  c          d            c   d
        */

        /*link(buf, flip_np(buf, A, PORT_1),  flip_np(buf, B, PORT_1));
        link(buf, flip_np(buf, A, PORT_2),  flip_np(buf, B, PORT_2));*/
        link(buf, flip(buf, getPortIndex(A, PORT_1)), flip(buf, getPortIndex(B, PORT_1)));
        link(buf, flip(buf, getPortIndex(A, PORT_2)), flip(buf, getPortIndex(B, PORT_2)));
        ++stats->annis;

        #ifdef __DEBUG__
        printf("Executed annis number %d\n", stats->annis);
        #endif /*__DEBUG__*/
    }
    else {
        /*/  a          d       a - B1 --- A1 - d
        //   \        /              \ /
        //     A -- B     -->         X
        //   /        \              / \
        //  b          c       b - B2 --- A2 - c
        */
        index_t A1, A2, B1, B2;

        // create new nodes
        createNode(buf, getKind(buf, A), &A1);
        createNode(buf, getKind(buf, A), &A2);
        createNode(buf, getKind(buf, B), &B1);
        createNode(buf, getKind(buf, B), &B2);

        // link new nodes with orphan nodes
        link(buf, getPortIndex(B1, PORT_0), flip_np(buf, A, PORT_1));
        link(buf, getPortIndex(B2, PORT_0), flip_np(buf, A, PORT_2));
        link(buf, getPortIndex(A1, PORT_0), flip_np(buf, B, PORT_1));
        link(buf, getPortIndex(A2, PORT_0), flip_np(buf, B, PORT_2));
        /*link(buf, flip(buf, getPortIndex(B1, PORT_0)), flip(buf, getPortIndex(A, PORT_1)));
        link(buf, flip(buf, getPortIndex(B2, PORT_0)), flip(buf, getPortIndex(A, PORT_2)));
        link(buf, flip(buf, getPortIndex(A1, PORT_0)), flip(buf, getPortIndex(B, PORT_1)));
        link(buf, flip(buf, getPortIndex(A2, PORT_0)), flip(buf, getPortIndex(B, PORT_2)));*/

        // Link new nodes with other new nodes
        linkNodes(buf, A1, PORT_1, B1, PORT_1);
        linkNodes(buf, A1, PORT_2, B2, PORT_1);
        linkNodes(buf, A2, PORT_1, B1, PORT_2);
        linkNodes(buf, A2, PORT_2, B2, PORT_2);
        /*link(buf, flip(buf, getPortIndex(A1, PORT_1)), flip(buf, getPortIndex(B1, PORT_1)));
        link(buf, flip(buf, getPortIndex(A1, PORT_2)), flip(buf, getPortIndex(B2, PORT_1)));
        link(buf, flip(buf, getPortIndex(A2, PORT_1)), flip(buf, getPortIndex(B1, PORT_2)));
        link(buf, flip(buf, getPortIndex(A2, PORT_2)), flip(buf, getPortIndex(B2, PORT_2)));*/

        // Unlink "old" nodes
        /*setKind(buf, A, 0);
        linkNodes(buf, A, PORT_0, A, PORT_0);
        linkNodes(buf, A, PORT_1, A, PORT_1);
        linkNodes(buf, A, PORT_2, A, PORT_2);

        setKind(buf, B, 0);
        linkNodes(buf, B, PORT_0, B, PORT_0);
        linkNodes(buf, B, PORT_1, B, PORT_1);
        linkNodes(buf, B, PORT_2, B, PORT_2);*/
        ++stats->dupls;
        #ifdef __DEBUG__
        printf("Executed dupls number %d\n", stats->dupls);
        //printNode(buf, "A1", A1);
        //printNode(buf, "A2", A2);
        //printNode(buf, "B1", B1);
        //printNode(buf, "B2", B2);
        #endif /*__DEBUG__*/
    }
}
