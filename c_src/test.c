// This file's only purpose is to aid in he development. Is will not serve as a "unit test" library for the methods

#include <stdio.h>
#include "interaction-combinators.h"

void printNodeState(buffer_t buf, index_t nodeIndex){
  printf("NODE %d:\tP0 = %d | P1 = %d | P2 = %d | KIND = %d",
          nodeIndex,
          getPortValue(buf, nodeIndex, PORT_0),
          getPortValue(buf, nodeIndex, PORT_1),
          getPortValue(buf, nodeIndex, PORT_2),
          getKind(buf, nodeIndex));
}

int main() {

    buffer_t buffer;

    printf("\n=========== START ===========\n");
    bufferInit(&buffer);
    //printf("bufferInit complete! sizeof(buffer) = %lu\n", sizeof buffer);

    printf("\n=========== TEST BUFFER ===========\n");
    printf("bufferInit complete. buffer = 0x%x\n", buffer);

    createNode(buffer, 5);
    createNode(buffer, 10);
    printf("New node created!\n");

    printf("buffer[0] = %d\tbuffer[1] = %d\tbuffer[2] = %d\tbuffer[3] = %d\n",
            buffer[0], buffer[1], buffer[2], buffer[3]);
    printNodeState(buffer, 0);
    puts("\n");
    printNodeState(buffer, 1);
    puts("\n");

    printf("\n=========== TEST \"KIND\" METHODS ===========\n");
    printf("getKindIndex() = %u\n", getKindIndex(0));
    setKind(buffer, 0, 6);
    printf("getKind() = %u\n", getKind(buffer, 0));

    printf("\n=========== TEST \"META\" METHODS ===========\n");
    printf("getMeta() = %d\n", getMeta(buffer, 0));
    setMeta(buffer, 0, 1);
    printf("getMeta() = %d\n", getMeta(buffer, 0));
    printf("buffer[%d] = %d\n", getKindIndex(0), buffer[getKindIndex(0)]);

    printf("\n=========== TEST \"PORT\" METHODS ===========\n");
    printf("PORT_0 type = %d\n", getPortType(getPortIndex(0, PORT_0)));
    printf("PORT_1 type = %d\n", getPortType(getPortIndex(0, PORT_1)));
    printf("PORT_2 type = %d\n", getPortType(getPortIndex(0, PORT_2)));

    printf("buffer[%d] = %d == %d\n", getPortIndex(0, PORT_0), buffer[getPortIndex(0, PORT_0)], getPortValue(buffer, 0, PORT_0));
    printf("buffer[%d] = %d == %d\n", getPortIndex(0, PORT_1), buffer[getPortIndex(0, PORT_1)], getPortValue(buffer, 0, PORT_1));
    printf("buffer[%d] = %d == %d\n", getPortIndex(0, PORT_2), buffer[getPortIndex(0, PORT_2)], getPortValue(buffer, 0, PORT_2));

    printf("setPortValue(0, PORT_0, 5)\n");
    setPortValue(buffer, 0, PORT_0, 5);
    printf("buffer[%d] = %d == %d\n", getPortIndex(0, PORT_0), buffer[getPortIndex(0, PORT_0)], getPortValue(buffer, 0, PORT_0));

    printf("\n=========== TEST \"LINK\" & \"FLIP\" METHODS ===========\n");

    printf("flip_o() == %d\n", flip(buffer, 0, PORT_1));
    printf("flip_d() == %d\n", flip(buffer, 1, PORT_2));

    printNodeState(buffer, 0);
    puts("\n");
    printNodeState(buffer, 1);
    puts("\n");

    linkNodes(buffer, 0, PORT_1, 1, PORT_2);
    link(buffer, getPortIndex(0, PORT_0), getPortIndex(1, PORT_0));

    printNodeState(buffer, 0);
    puts("\n");
    printNodeState(buffer, 1);
    puts("\n");

    printf("flip_o() == %d\n", flip(buffer, 0, PORT_1));
    printf("flip_d() == %d\n", flip(buffer, 1, PORT_2));

    free(buffer);
    printf("\n=========== END ==========\n");

    return 0;
}
