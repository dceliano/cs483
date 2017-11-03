#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "byutr.h"
#include "lrustack.h"

/*=============================================================================
 |   Assignment:  PEX2
 |       Author:  Dominic Celiano
 |        Class:  CS483
 +-----------------------------------------------------------------------------
 |   Description:  This program simulates a number of memory references held in a BYU Trace File.
 /                 First, a menu is displayed in which the user inputs the name of the trace file
 /                 as well as the desired page size. Then, an LRU stack is implemented to keep track
 /                 of the memory references. The output of the program is the number of accesses and the
 /                 miss rate for the different page allocations.
 |
 |   Required Features Not Included: None
 |
 |   Known Bugs:  None
 +-----------------------------------------------------------------------------
 |   Documentation Statement: On 24 Apr 2017, C1C Nick Harron explained what the different frame allocation sizes meant
 /   and what exactly my output from my final program was supposed to look like (he didn't show me any code, though).
 *===========================================================================*/

int main(int argc, char **argv) {
    FILE *ifp;

    // If the number of arguments is wrong, then quit.
    if (argc != 2 && argc != 3) {
        fprintf(stderr, "Error! You're supposed to call the executable like this:%s input_byutr_file [frame_size]\n", argv[0]);
        fprintf(stderr, "where frame_size is a digit corresponding to the following menu:\n\t1: 512 bytes\n\t2: 1KB\n\t3: 2KB\n\t4: 4KB\n");
        exit(1);
    }
    // If the file cannot be opened, then quit.
    if ((ifp = fopen(argv[1], "rb")) == NULL) {
        fprintf(stderr, "Error! Cannot open %s for reading\n", argv[1]);
        exit(1);
    }
    // No errors were encountered, so go ahead and read the BYU trace file.
    int menu_option = 0;
    if (argc == 3) {
        //If the user entered a frame size when they called the executable.
        menu_option = atoi(argv[2]);
    }
    while (menu_option < 1 || menu_option > 4) {
        //If a menu option still needs to be selected
        char input[128];
        printf("Select a frame size:\n\t1: 512 bytes\n\t2: 1KB\n\t3: 2KB\n\t4: 4KB\n");
        fgets(input, 128, stdin);
        menu_option = atoi(input);
    }
    int OFFSET_BITS;
    int MAX_FRAMES; //this is used as the size of the array
    // Use the menu option selected to determine the number of OFFSET_BITS and the number of MAX_FRAMES.
    // Keep in mind that the size of the physical memory being simulated is 4MB, which corresponds to 22 bits (2^22 = 4MB).
    switch (menu_option) {
        case 1:
            OFFSET_BITS = 9;
            MAX_FRAMES = 8192;
            break;
        case 2:
            OFFSET_BITS = 10;
            MAX_FRAMES = 4096;
            break;
        case 3:
            OFFSET_BITS = 11;
            MAX_FRAMES = 2048;
            break;
        case 4:
            OFFSET_BITS = 12;
            MAX_FRAMES = 1024;
            break;
    }

    uint64_t num_accesses = 0;
    p2AddrTr trace_record;
    lrustack lrus; //LRU stack used to keep track of what frames are in physical memory
    initialize(&lrus, MAX_FRAMES);
    //In the array below, the number of faults for each allocation size is stored. Initially, all allocation sizes have 0 faults, so the array is initialized to 0.
    int num_faults_array[MAX_FRAMES];
    for(int j = 1; j <= MAX_FRAMES; j++){
        num_faults_array[j] = 0;
    }

    while (!feof(ifp)) //until we reach the EOF (end of file)
    {
        fread(&trace_record, sizeof(p2AddrTr), 1, ifp); // Read the next trace record in the file.
        uint32_t page_num = trace_record.addr >> OFFSET_BITS; // This is the page number which was requested. To get the page number, we shift the offset bits off of the address.
        //printf("page#: %d\n", page_num); // This line prints the page number that was referenced for debugging - needs to be commented out when submitted
        int page_depth = seek_and_remove(&lrus, page_num); //If the page number was linked to a page already be in memory, remove that page from the stack and return the depth at which it was found.
        //printf("page depth: %d\n", page_depth); //print the page depth for debugging.
        num_accesses++; //since we are accessing memory, increment the number of accesses
        if (page_depth == -1){ //if the page was not found in the stack (i.e. we need to access disk to find it)
            for(int j = 1; j <= MAX_FRAMES; j++){ // for all of the different possible numbers of frames
                num_faults_array[j] += 1; //increment the number of page faults since, no matter what the number of frames, this would be a page fault
            }
        }
        // If the seek_and_remove function reports back a valid depth d, then that memory access is a hit for all allocation sizes greater than or
        //equal to d, but a miss for all frame sizes less than d. Therefore, increment the array elements for sizes 0 to d-1.
        else{
            for(int j = 1; j < page_depth; j++){ //for all frame sizes less than d, the page depth
                num_faults_array[j] += 1; //increment the number of page faults since all of these frame allocation sizes would have to get the frame from memory.
            }
        }
        push(&lrus, page_num); //add the page reference to the top of the LRU stack since it was either removed from the stack or was never in the stack. Note that "push" automatically limits the size of the LRU stack to MAX_FRAMES.
    }

    printf("Total accesses: %d\n", num_accesses);
    printf("Frames\tMiss Rate\n");
    for(int j = 1; j <= MAX_FRAMES; j++){
        float miss_rate = (float)num_faults_array[j] / num_accesses; //calculate the miss rate by dividing the number of misses by the total number of accesses
        printf("%d\t%f\n", j, miss_rate);
    }

    fclose(ifp);
    return (0);
}