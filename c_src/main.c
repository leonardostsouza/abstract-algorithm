#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "interaction-combinators.h"

void printStats(stats_t *stats, double execTime){
    printf("- time\t\t: %fs\n- loops\t\t: %d\n- rewrites\t: %d\n  - dupls\t: %d\n  - annis\t: %d\n  - betas\t: %d\n",
            execTime, stats->loops, stats->rewrites, stats->dupls, stats->annis, stats->betas);
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
            printf("====> Calculated buffer and correct response are different at position %d\n", i);
            return;
        }
    }
    printf("====> Calculated buffer and correct response match!\n");
}

int main(int argc, char **argv){

    // argv[1] => path to Original Network csv file
    // argv[2] => path to Reduced Network cdv file

    double execTime = 0.0;
    clock_t begin, end;

    buffer_t graph, graphReduced;
    stats_t stats;

    bufferInit(&graph);
    bufferInit(&graphReduced);
    statsReset(&stats);

    if ((argc != 2) && (argc != 3)) {
        printf("Usage: %s PATH_TO_ORIGINAL_NET_FILE <PATH_TO_REDUCED_NET_FILE>", argv[0]);
        return 1;
    }

    // populate original network buffer
    populateBuffer(argv[1], graph);

    if (argc > 2) {
        // populate reduced network buffer
        populateBuffer(argv[2], graphReduced);
    }

    #ifdef __DEBUG__
    // print original network
    printf("Original Network:\n");
    printBuffer(graph);
    #endif /* __DEBUG__ */

    // reduce Network
    begin = clock();
    reduce(graph, &stats);
    end = clock();
    execTime = (double) (end - begin) / CLOCKS_PER_SEC;
    #ifdef __DEBUG__
    printf("begin = %lu, end = %lu, (e - b) = %lu, CPS = %lu, time = %fs\n",
        begin, end, (end - begin), CLOCKS_PER_SEC, execTime);
    #endif /* __DEBUG__ */

    #ifdef __DEBUG__
    // print reduced network
    printf("Reduced Network:\n");
    printBuffer(graph);
    #endif /* __DEBUG__ */

    compareBuffers(graph, graphReduced);

    // print stats
    printStats(&stats, execTime);

    // free memory
    freeBuffer(graph);
    freeBuffer(graphReduced);

    return EXIT_SUCCESS;
}
