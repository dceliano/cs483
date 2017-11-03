#include <stdint.h>

#ifndef PEX2_LRUSTACK_H
#define PEX2_LRUSTACK_H

/* a node for use in the LRU stack, which is a doubly linked list*/
typedef struct s_node {
    struct s_node* prev;
    struct s_node* next;
    uint32_t pagenum;
} node;

/* An LRU stack consisting of head and tail pointers as well as a
 * current size and a max size. Keep the stack limited to the max
 * size or you will run out of memory trying to run the simulation! */
typedef struct s_lrus{
    node* head;
    node* tail;
    int size;
    int maxsize;
} lrustack;

/* initializes the LRU stack */
void initialize(lrustack* lrus, int maxsize);

/* use pagenum when creating a new node, which will be pushed onto
 * the LRU stack. Make sure to keep track of the LRU stack's size
 * and free and reset the tail as necessary to limit it to max size */
void push(lrustack* lrus, uint32_t pagenum);

/* look for pagenum in lrus and remove it if found. Return the depth
 * at which pagenum was found or -1 if not */
int seek_and_remove(lrustack* lrus, uint32_t pagenum);

/* print the stack (implementation is given to you). useful for debug*/
void print_stack(lrustack* lrus);

/* print the stack backward (implementaiton is given to you). useful for debug*/
void print_stack_backward(lrustack* lrus);

/* Use this function to test your LRU stack implementation. For the
 * Gate Check, you will simply call this function and I will view
 * the output */
void test_lru_stack(lrustack* lrus);

#endif //PEX2_LRUSTACK_H
