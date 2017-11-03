#include <stdio.h>
#include <stdlib.h>
#include "lrustack.h"

/* initializes the LRU stack */
void initialize(lrustack* lrus, int maxsize) {
    lrus->head = NULL;
    lrus->size = 0;
    lrus->tail = NULL;
    lrus->maxsize = maxsize;
}

/* use pagenum when creating a new node, which will be pushed onto
 * the LRU stack. Make sure to keep track of the LRU stack's size
 * and free and reset the tail as necessary to limit it to max size */
void push(lrustack* lrus, uint32_t pagenum) {
    node* new_page = malloc(sizeof(node)); //create a node for the new page (need to malloc so the memory lives on). This is done regardless of if the page is already in the stack or not. Not the most efficient way of doing it, but it works.
    new_page->pagenum = pagenum;
    /* Need to handle 3 cases: the cases when the stack is empty, full, and when neither of those conditions are true.
    Note that when a new node is added, it is added to the head. The tail points to the bottom of the stack. */
    if(lrus->size <= 0){ //if the stack is empty
        lrus->head = new_page;
        lrus->tail = new_page;
        new_page->next = NULL;
        new_page->prev = NULL;
        lrus->size++;
    }
    else{ //if the LRU stack is not empty
        /* We must see if the new page is already in the LRU stack. If it is, get rid of the found instance of that page and
         * decrement the LRU stack size, and then change around the pointers. */
        lrus->size++;
        new_page->next = lrus->head;
        new_page->prev = NULL;
        lrus->head->prev = new_page;
        lrus->head = new_page; //put the new page at the head of the stack
        if(lrus->size > lrus->maxsize){ //if the stack grew past its size limit, get rid of the tail and free its memory.
            lrus->tail = lrus->tail->prev;
            free(lrus->tail->next);
            lrus->tail->next = NULL;
            lrus->size--;
        }
    }
}

/* look for pagenum in lrus and remove it if found. Return the depth
 * at which pagenum was found or -1 if not */
int seek_and_remove(lrustack* lrus, uint32_t pagenum) {
    node *current_node = lrus->head; //this pointer will be used for looking through the stack
    int current_depth = 1;
    while (current_node != NULL) { //until we reach the end of the stack
        if (current_node->pagenum == pagenum) { //if we found the page in the stack
            lrus->size--;
            if(lrus->size == 0){
                //if the LRU stack will be empty after removing this page.
                free(current_node);
                lrus->head = NULL;
                lrus->tail = NULL;
                return 1; //the node must have been found at a depth of 1
            }
            else if(current_node->prev == NULL){
                lrus->head = current_node->next;
                lrus->head->prev = NULL;
                free(current_node);
                return 1; //the node must have been found at the head of the stack (so we will return a depth of 1)
            }
            else if(current_node->next == NULL){
                //the node must have been found at the tail of the stack
                lrus->tail = current_node->prev;
                current_node->prev->next = NULL;
                free(current_node);
                return current_depth;
            }
            else {
                current_node->prev->next = current_node->next;
                current_node->next->prev = current_node->prev;
                free(current_node);
                return current_depth;
            }
        }
        current_node = current_node->next;
        current_depth++;
    }
    return -1;
}

/* print the stack (implementation is given to you). useful for debug*/
void print_stack(lrustack* lrus) {
    node* temp = lrus->head;
    int ctr = 1;

    while (temp != NULL) {
        printf("Node %d: %d\n", ctr, temp->pagenum);
        ctr++;
        temp = temp->next;
    }
}

/* print the stack backward (implementaiton is given to you). useful for debug*/
void print_stack_backward(lrustack* lrus) {
    node* temp = lrus->tail;
    int ctr = lrus->size;

    while (temp != NULL) {
        printf("Node %d: %d\n", ctr, temp->pagenum);
        ctr--;
        temp = temp->prev;
    }
}

/* Use this function to test your LRU stack implementation. For the
 * Gate Check, you will simply call this function and I will view
 * the output */
void test_lru_stack(lrustack* lrus) {
    initialize(lrus, 5);   // create stack of size 5

    if (lrus->size != 0) printf("Failed: size = 0\n");     // size = 0?
    if (lrus->maxsize != 5) printf("Failed: maxsize = 5\n");  // maxsize = 5?

    // test size limiting - size should not grow > 5 nodes
    push(lrus, 1);
    push(lrus, 2);
    push(lrus, 3);
    push(lrus, 4);
    push(lrus, 5);
    push(lrus, 6);
    if (lrus->size != 5) printf("Failed: Size != 5 Check #1\n");

    // stack should be 6-5-4-3-2
    printf("Stack (forward) should be: 6-5-4-3-2:\n");
    print_stack(lrus);
    printf("\n");
    printf("Stack (backward) should be: 2-3-4-5-6:\n");
    print_stack_backward(lrus);
    printf("\n");

    // test removal of tail
    if ((seek_and_remove(lrus, 2) != 5) || (lrus->size != 4)) {
        printf("Failed: Tail removal 1.\n");
    }

    // stack should be 6-5-4-3
    printf("Stack (forward) should be: 6-5-4-3:\n");
    print_stack(lrus);
    printf("\n");
    printf("Stack (backward) should be: 3-4-5-6:\n");
    print_stack_backward(lrus);
    printf("\n");

    // test removal of middle
    if ((seek_and_remove(lrus, 5) != 2)  || (lrus->size != 3)) {
        printf("Failed: Middle Removal 1.\n");
    }
    if ((seek_and_remove(lrus, 4) != 2)  || (lrus->size != 2)) {
        printf("Failed: Middle Removal 2.\n");
    }

    // stack should be 6-3
    printf("Stack (forward) should be: 6-3:\n");
    print_stack(lrus);
    printf("\n");
    printf("Stack (backward) should be: 3-6:\n");
    print_stack_backward(lrus);
    printf("\n");

    // test removal of head with > 1 item
    if ((seek_and_remove(lrus, 6) != 1)  || (lrus->size != 1)) {
        printf("Failed: Head removal > 1 item in stack.\n");
    }

    // stack should be 3
    printf("Stack (forward) should be: 3:\n");
    print_stack(lrus);
    printf("\n");
    printf("Stack (backward) should be: 3:\n");
    print_stack_backward(lrus);
    printf("\n");

    // test removal of head with 1 item
    if ((seek_and_remove(lrus, 3) != 1) || (lrus->size != 0)) {
        printf("Failed: Head removal with 1 item in stack.\n");
    }

    // stack should be empty
    printf("Stack (forward) should be: [empty]:\n");
    print_stack(lrus);
    printf("\n");
    printf("Stack (backward) should be: [empty]:\n");
    print_stack_backward(lrus);
    printf("\n");

    // test size limiting again
    push(lrus, 2);
    push(lrus, 3);
    push(lrus, 4);

    // test removal of tail
    if ((seek_and_remove(lrus, 2) != 3) || (lrus->size != 2)) {
        printf("Failed: Tail Removal 2.\n");
    }

    push(lrus, 5);
    push(lrus, 6);
    push(lrus, 7);
    push(lrus, 8);

    if (lrus->size != 5) printf("Failed: Size != 5 Check #2\n");

    // Stack should be 8-7-6-5-4
    printf("Stack (forward) should be: 8-7-6-5-4:\n");
    print_stack(lrus);
    printf("\n");
    printf("Stack (backward) should be: 4-5-6-7-8:\n");
    print_stack_backward(lrus);
    printf("\n");

    // search for non-existent
    if ((seek_and_remove(lrus, 2) != -1) || (lrus->size != 5)) {
        printf("Failed: Non-existent seek and remove.\n");
    }

    // search/remove tail
    if ((seek_and_remove(lrus, 4) != 5) || (lrus->size != 4)) {
        printf("Failed: Tail Removal 3.\n");
    }

    // search/remove head
    if ((seek_and_remove(lrus, 8) != 1) || (lrus->size != 3)) {
        printf("Failed: Head Removal Check 3.\n");
    }

    // Stack should be 7-6-5
    printf("Stack (forward) should be: 7-6-5:\n");
    print_stack(lrus);
    printf("\n");
    printf("Stack (backward) should be: 5-6-7:\n");
    print_stack_backward(lrus);
    printf("\n");

    printf("test_lru() complete.\n");
}