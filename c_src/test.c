#include <stdio.h>
#include "headers/node.h"

void printNodeState(node_t *node){
  printf("KIND = %d | P1 = %d | P2 = %d | P3 = %d",
          getKind(*node),
          getPort(*node, PORT_1),
          getPort(*node, PORT_2),
          getPort(*node, PORT_3));
}


int main() {
  node_t myNode1, myNode2;
  nodeInit(&myNode1, 1, 0, 0, 0);
  nodeInit(&myNode2, 123456, 1, 2, 3);

  //node_t myNode2 = createNode(2, 1, 2, 3);

  printf("myNode1:\t");
  printNodeState(&myNode1);
  printf("\nmyNode2:\t");
  printNodeState(&myNode2);
  printf("\n");

  return 0;
}
