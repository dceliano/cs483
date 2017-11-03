//
// Created by Dominic Celiano on 1/5/17.
//
//Documentation: No help received, but the c textbook was referenced (plus a c textbook I have - "The essentials of the C programming language").

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pex0.h"

//function prototypes
int func1(int int_array[]);

//main
int main(int argc, char* argv[]){
    if(argc >= 2) { //if we get enough inputs to print argv[1]
        printf("%s\n", argv[1]);
    }
    for(int i = 1; i <= 20; i++) {
        if ((i % 3) == 0) {
            printf("%d\n", i);
        }
    }

    int int_array[] = { 5, 3, 6, 2 };
    printf("%d\n", func1(int_array));

    struct node node1;
    strncpy(node1.name, "Node 1", 6);
    struct node node2;
    strncpy(node2.name, "Node 2", 6);
    node1.next = &node2;
    node2.next = NULL;
    printf("%s\n", node1.name);
    printf("%s\n", node1.next->name);

    struct node *node3;
    node3 = (struct node *) malloc(sizeof(struct node)); //dynamically allocates memory. node3 is the address of the node
    strncpy(node3->name, "Node 3", 6);
    node3->next = NULL;
    printf("%s\n", node3->name);

    return 0;
}

int func1(int int_array[]){
    return(int_array[0] * 2);
}
