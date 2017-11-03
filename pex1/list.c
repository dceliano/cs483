#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Your implementation of the methods in list.h goes in this file

/* list_insert_tail: create a new node at the end of the linked list
   Parameters
   - node*: a pointer to the head of a linked list
   - char*: data to be stored in the new node
   Return: a pointer to the head of the linked list */
node* list_insert_tail(node* head, char* data){
    /* First, create a new node at the tail. */
    node* new_node = malloc(sizeof(node)); //returns a pointer to our new node
    new_node->data = malloc(strlen(data) + 1); //We have to copy the entire text using strcpy, not a pointer to it.
    strcpy(new_node->data, data); //uses pointers to perform the copy
    new_node->next = NULL;
    /* Second, find the pointer to the end of the linked list, or initialize the linked list.*/
    node* current_node = head; //a pointer to the node we're currently looking at
    if(current_node == NULL){ //if we're initializing a linked list
        head = new_node;
    }
    else { //if we're adding to an existing linked list
        while (current_node->next != NULL) {
            //keep looping until the end of the linked list
            current_node = current_node->next;
        }
        current_node->next = new_node;
    }
    return head;
}

/* list_insert_head: create a new node and insert it at the head of the linked
    list
   Parameters
   - node*: a pointer to the head of a linked list
   - char*: data to be stored in the new node
   Return: a pointer to the new head of the linked list */
node* list_insert_head(node* head, char* data){
    /* First, create a new node at the head. */
    node* new_node = malloc(sizeof(node)); //returns a pointer to our new node
    new_node->data = malloc(strlen(data) + 1);
    strcpy(new_node->data, data);
    new_node->next = head;
    /* Second, make the new node our new head and return the new head.*/
    head = new_node;
    return head;
}

/* list_insertn: create a new node and insert it at the <n>th position,
    where the head is at position 1
   Parameters
   - node*: a pointer to the head of a linked list
   - char*: data to be stored in the new node
   - int: the position in which to insert the new node
   Return: a pointer to the head of the linked list */
node* list_insertn(node* head, char* data, int position){
    /* First, create a new node at the head. */
    node* new_node = malloc(sizeof(node)); //returns a pointer to our new node
    new_node->data = malloc(strlen(data) + 1);
    strcpy(new_node->data, data);
    /* Second, insert the new node at the correct position. */
    int node_num = 1;
    node* current_node = head; //a pointer to the node we're currently looking at
    node* prev_node = NULL;
    while(current_node != NULL){
        //keep looping until the end of the linked list
        if(node_num == position){ //if this is where we should be inserting the node
            if(current_node == head){
                new_node->next = current_node;
                head = new_node;
            }
            else{
                prev_node->next = new_node;
                new_node->next = current_node;
            }
            break;
        }
        //move onto the next node if we didn't break.
        prev_node = current_node;
        current_node = current_node->next;
        node_num++;
    }
    if(node_num == position){ //this is for adding a node to the end of the linked list
        prev_node->next = new_node;
        new_node->next = current_node;
    }
    return head;
}

/* list_remove: remove the node containing specific data; if multiple nodes
    contain the data, remove the node nearest to the head
   Parameters
   - node*: a pointer to the head of a linked list
   - char*: data that, if found in one of the nodes in the list, indicates
      the node to remove (in the case of multiple nodes containing the data,
	  remove the node nearest to the head)
   Return: a pointer to the head of the linked list */
node* list_remove(node* head, char* data){
    node* current_node = head; //a pointer to the node we're currently looking at
    node* prev_node = NULL;
    while (current_node != NULL) {
        //keep looping until the end of the linked list
        if (strcmp(current_node->data, data) == 0) { //if the strings are equal
            if(current_node == head){
                head = current_node->next;
            }
            else {
                prev_node->next = current_node->next; //point around the node which we are removing
            }
            free(current_node->data);
            free(current_node); //deallocate the memory for the removed node
            break;
        }
        prev_node = current_node;
        current_node = current_node->next;
    }
    return head;
}

/* list_removen: remove the <n>th node in the list
   Parameters
   - node*: a pointer to the head of a linked list
   - int: the position of the node to remove, where the head is at position 1
   Return: a pointer to the head of the linked list */
node* list_removen(node* head, int position){
    int node_num = 1;
    node* current_node = head; //a pointer to the node we're currently looking at
    node* prev_node = NULL;
    while (current_node != NULL) {
        //keep looping until the end of the linked list
        if (node_num == position) {
            if(current_node == head){
                head = current_node->next;
            }
            else {
                prev_node->next = current_node->next; //point around the node which we are removing
            }
            free(current_node->data);
            free(current_node); //deallocate the memory for the removed node
            break;
        }
        prev_node = current_node;
        current_node = current_node->next;
    }
    return head;
}

/* list_print: print the data contained in each node in the linked list,
    starting with the head, where each node's data is printed on a separate
    line
   Parameter: node*: a pointer to the head of a linked list */
void list_print(node* head){
    int node_num = 1;
    node* current_node = head;
    while(current_node != NULL) {
        printf("%d %s\n", node_num, current_node->data);
        current_node = current_node->next;
        node_num++;
    }
}

/* list_printn: print the data contained in the <n>th node of the list
   Parameters:
   - node*: a pointer to the head of a linked list
   - int: the position of the node with the data to print, where the head is
       at position 1 */
void list_printn(node* head, int position){
    int node_num = 1;
    node* current_node = head; //a pointer to the node we're currently looking at
    while(current_node != NULL){
        //keep looping until the end of the linked list
        if(node_num == position){
            printf("%s\n", current_node->data);
        }
        current_node = current_node->next;
        node_num++;
    }
}

/* list_get: retrieve the data stored in the <n>th node in the list
   Parameters:
   - node*: a pointer to the head of a linked list
   - int: the position of the node with the data to retrieve, where the head is
       at position 1
   Return: the data */
char* list_get(node* head, int position){
    int node_num = 1;
    node* current_node = head; //a pointer to the node we're currently looking at
    while(current_node != NULL){
        //keep looping until the end of the linked list
        if(node_num == position){
            return current_node->data;
        }
        current_node = current_node->next;
        node_num++;
    }
}

/* list_destroy: free all malloc'd memory used in the linked list,
    including char*'s and node*'s
   Parameter: node*: a pointer to the head of a linked list */
void list_destroy(node* head){
    node* current_node = head; //a pointer to the node we're currently looking at
    while(current_node != NULL) {
        free(current_node->data);
        free(current_node);
        current_node = current_node->next;
    }
}