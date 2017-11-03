/*=============================================================================
 |   Assignment:  PEX 1
 |       Author:  Dominic Celiano, C1C
 |        Class:  CS483
 +-----------------------------------------------------------------------------
 |   Description:  Implements the afsh (Air Force Shell) program. Some functions that are typed
 /   into the shell are handled by this c code, and others use the OS in their implementation by
 /   using the fork/exec system calls.
 |
 |   Required Features Not Included:  None
 |
 |   Known Bugs:  None
 +-----------------------------------------------------------------------------
 |   Documentation Statement:  I took some of the code from the in-class example on L06 (where STRTOK was used).
 /   Maj Brault helped me with the cd command. C2C White introduced me to the atoi command. Maj Brault checked my code
 /   to make sure nothing with the error checking was seriously messed up.
 *===========================================================================*/

#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    /* Variables used in the shell loop */
    char cwd[50]; //used in our prompt to display the cwd
    char input_buf[127]; //holds the input command
    char *cmd_ptr_array[50]; //holds pointers to each separate command entered
    char *token; //used to split the command up
    node *history = NULL; //a pointer to the head of the history list. The list starts out empty.
    pid_t ret_val; //used for forking a child process
    int status; //also used for the forking
    int execvp_return = 0;
    char *new_dir; //used in "cd" function
    new_dir = malloc(sizeof(char) * 127); //allocate space for new_dir
    int num_tokens;
    int converted_int; //used in "recall" function
    int get_input_flag = 1;


    printf("\n. . . Air Force Shell (afsh) . . .\n");

    /* Infinite shell loop (until exit is typed as the command) */
    while(1){
        if(get_input_flag) {
            /* Print a prompt and read a command line argument. */
            getcwd(cwd, 49);
            printf("%s> ", cwd);
            fgets(input_buf, 126, stdin); //get an input from the user
            input_buf[strlen(input_buf) - 1] = '\0'; //make sure the input buffer ends with a null terminator
        }
        else{
            //We enter this only after calling recall.
            strcpy(input_buf, list_get(history, converted_int)); //get the input from the recall function.
            get_input_flag = 1; //next time, get input
        }

        /* Store the command line into the history, deleting duplicate entries if necessary. */
        history = list_remove(history, input_buf); //remove any previously entered commands which were the same
        history = list_insert_head(history, input_buf); //insert the newest command at the head

        /* Parse the command line into tokens (words) */
        token = strtok(input_buf, " \t"); //gets the first token, split at a space or a tab character.
        num_tokens = 0;
        while (token != NULL){
            cmd_ptr_array[num_tokens] = strdup(token); //duplicates the token and stores it in the cmd_ptr_array
            //printf("cmd_ptr_array[%d] = \"%s\"\n", num_tokens, cmd_ptr_array[num_tokens]); //used for debugging
            token = strtok(NULL, " \t"); //uses the same pointer as before and finds the next token
            num_tokens++;
        }
        cmd_ptr_array[num_tokens] = NULL; //now, cmd_ptr_array has all of the commands as string pointers, ended by NULL.

        /* Compare the command to the shell's built-in functions and either execute built-in functions or
         * fork a child/children which load and execute the programs, and wait for the child(ren) to terminate. */
        /* Built-in Commands */
        if(strcmp(cmd_ptr_array[0], "exit") == 0){
            printf("Terminating afsh. . .");
            break;
        }
        else if(strcmp(cmd_ptr_array[0], "cd") == 0){
            execvp_return = 0; //tell our error checking that we found a command
            if (num_tokens == 2) { //don't go in here unless there are 2 tokens
                if (cmd_ptr_array[1][0] == '~') {
                    //If we're going to the home directory
                    //note: getenv returns "/home/cs483" on my machine.
                    sprintf(new_dir, "%s%s", getenv("HOME"),
                            cmd_ptr_array[1] + 1); //offset by 1 to skip the ~ character
                } else {
                    //If we're going to a place relative to where we are
                    new_dir = cmd_ptr_array[1];
                }
                chdir(new_dir);
            }
            else{
                printf("ERROR: You had the incorrect number of arguments to the \"cd\" function.\n");
            }
        }
        else if(strcmp(cmd_ptr_array[0], "history") == 0){
            execvp_return = 0; //tell our error checking that we found a command
            //the 2 lines below make it like we just typed in history, avoiding storing terms like "history 3". Then, it redoes the deleting of duplicates.
            history = list_removen(history, 1); //remove any previously entered commands which were the same
            /* Store the command line into the history, deleting duplicate entries if necessary. */
            history = list_remove(history, "history"); //remove any previously entered commands which were the same
            history = list_insert_head(history, "history"); //insert the newest command at the head
            list_print(history);
        }
        else if(strcmp(cmd_ptr_array[0], "recall") == 0) {
            execvp_return = 0; //tell our error checking that we found a command
            if (num_tokens == 2) { //don't go in here unless there are 2 tokens
                history = list_removen(history,
                                       1); //get rid of the recall command which was just entered so it's not in the history.
                converted_int = atoi(cmd_ptr_array[1]); //returns 0 if atoi is unsuccessful (atoi converts ASCII char to int)
                if(converted_int != 0) {
                    list_printn(history, converted_int); //print the command
                    /* By setting the flag below, actually execute the command. In order to do so, we have restart the loop to go through all of the
                     * conditional statements properly. */
                    get_input_flag = 0;
                }
                else{
                    printf("ERROR: The second argument to the \"recall\" function must be an integer.\n");
                }
            }
            else{
                printf("ERROR: You had the incorrect number of arguments to the \"recall\" function.\n");
            }
        }
        /* Non-built-in commands */
        else{
            ret_val = fork();
            if(ret_val == 0){ //in the child
                execvp_return = execvp(cmd_ptr_array[0], cmd_ptr_array);
            }
            else{ //in the parent
                wait(&status); //wait for the child to finish execution
            }
        }

        if(execvp_return == -1 && ret_val == 0){ //if the command was not recognized by our loop and we're in the child
            printf("That command is not recognized. Please try again.\n");
        }
    }
    free(new_dir); //free up memory
    return 0;
}