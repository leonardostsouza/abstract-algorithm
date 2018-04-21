#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "interaction-combinators.h"

#define FILE_LINE_BUFFER_SIZE 2048

void printBuffer(buffer_t buffer){
    index_t i;
    printf("[");
    index_t lastValidIndex = buffer[NEXT];
    for (i = 0; i < lastValidIndex; i++){
        printf("%d,", buffer[i]);
    }
    printf("] ===> SIZE = %d\n\n\n", lastValidIndex);
}

void printStats(stats_t *stats){
    printf("loops: %d\nrewrites: %d\nbetas: %d\ndupls: %d\nannis: %d\n",
            stats->loops, stats->rewrites, stats->betas, stats->dupls, stats->annis);
}

uint32_t isOpen(FILE *fileName){
    if(fileName == NULL) {
        printf("Could not open file...\n");
        return 0;
    }
    return 1;
}

void compareBuffers(buffer_t buf1, buffer_t buf2){
    index_t i;
    for (i = 0; i < MAX_BUFFER_SIZE; i++){
        if(buf1[i] != buf2[i]){
            printf("====> Buffers are different\n");
            return;
        }
    }
    printf("====> Buffers are equal\n");
}

int main(int argc, char **argv){

    // argv[1] => path to Original Network csv file
    // argv[2] => path to Reduced Network cdv file

    FILE *origNet;
    FILE *reducNet;
    char *line, *pch, *reducLine, *reducPch;
    size_t line_len = 0, reduc_line_len = 0;

    buffer_t graph, graphReduced;
    index_t index = 0, reducIndex = 0;
    stats_t stats;

    bufferInit(&graph);
    bufferInit(&graphReduced);
    statsReset(&stats);

    if (argc != 3) {
        printf("Usage: %s <PATH_TO_ORIGINAL_NET_FILE> <PATH_TO_REDUCED_NET_FILE>", argv[0]);
        return 1;
    }

    // open files
    origNet = fopen(argv[1], "r");
    reducNet = fopen(argv[2], "r");
    if(!(isOpen(origNet) && isOpen(reducNet))){
        return EXIT_FAILURE;
    }

    // populate original network buffer
    getline(&line, &line_len, origNet);
    pch = strtok(line, ","); // get "entry_point" value
    graph[ENTRY_POINT] = (index_t) strtoul(pch, NULL, 10);

    pch = strtok(NULL, ","); // other values
    while(pch != NULL){
        graph[index] = (index_t) strtoul(pch, NULL, 10);
        ++index;
        pch = strtok(NULL, ",");
    }

    graph[NEXT] = index;

    // populate reduced network buffer
    getline(&reducLine, &reduc_line_len, reducNet);

    reducPch = strtok(reducLine, ","); // get "entry_point" value
    graphReduced[ENTRY_POINT] = (index_t) strtoul(reducPch, NULL, 10);

    reducPch = strtok(NULL, ","); // other values
    while(reducPch != NULL){
        graphReduced[reducIndex] = (index_t) strtoul(reducPch, NULL, 10);
        ++reducIndex;
        reducPch = strtok(NULL, ",");
    }

    graphReduced[NEXT] = reducIndex;

    //compareBuffers(graph, graphReduced);

    // print original network
    //printBuffer(graph);

    // reduce Network
    reduce(graph, &stats);

    // print reduced network
    //printBuffer(graph);

    // print correct answer
    //printBuffer(graphReduced);

    // print stats
    printStats(&stats);

    return EXIT_SUCCESS;
}
