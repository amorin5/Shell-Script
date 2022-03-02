#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include<sys/wait.h>
//#include "linkedList.c"


struct node {
   char *data;
   char *key;
   struct node *next;
};


void push(struct node* next, char *new_data, char *new_key){
    printf("called\n");
    struct node *new_node = malloc(sizeof(struct node));
    new_node->data  = new_data;
    new_node->key = new_key;
    new_node->next = next;
    next = new_node;
}

//TO-DOs
//1. string concatenation to write function
//2. linked list for alias method (struct)
//3. redirection method
//4. unalias
//6. the 

void printPrompt(){
    write(STDOUT_FILENO, "mysh> ", strlen("mysh> "));                
}


int newProcess(char *myargs[]){
    //write(1, myargs[0], strlen(myargs[0]));
    //write(1, myargs[1], strlen(myargs[1]));
    printf("NEW PROCESS RUN\n");
    int rc = fork();
    int status;
    if (rc < 0) {   //Fork Error
        exit(1);
    } else if(rc == 0){
        execv(myargs[0], myargs);
    }
    else {
       rc = waitpid(rc, &status, 0);
    }
    return 0;
}

int main(int argc, char *argv[]){
    FILE *fp;
    int flag = 0;
    if(argc == 3){
        write(STDERR_FILENO, "Usage: mysh [batch-file]\n", 25);
        return(1);
    } else if(argc == 2){
        fp = fopen(argv[1], "r");
        flag = 1;
        if(fp == NULL){
            char str[] = "Error: Cannot open file ";
            strcat(str, argv[1]);
            strcat(str, ".\n");
            write(STDERR_FILENO, str, strlen(str));
            return(1);
        }
    } else {
        fp = stdin;
        printPrompt();
    }
    char buffer[512];
    char *myargs[sizeof(buffer)];
    char *token;
    struct node *head = NULL;
    //struct node *head = malloc(sizeof(struct node));
    //struct node *next = malloc(sizeof(struct node));
    
    int size = 1;
    while(fgets(buffer, 512, fp)){
        if(flag == 1){
            write(1, buffer, strlen(buffer));
        }
        //seperating the line into the args -- redirection
        if(strchr(buffer, '>')){
            token = strtok(buffer, " >");
            myargs[0] = token;
            int i = 1;
            while(token != NULL){
                token = strtok(NULL, " \n");
                myargs[i] = token;
                i++;
            }
            //redirection method -- better to do here or easier to do in new call?
        } else {
            //base case -- break the line into array of arguments
            token = strtok(buffer, " \n");
            myargs[0] = token;
            while(token != NULL){
                token = strtok(NULL, " \n");
                myargs[size] = token;
                size++;
            }
                //check if arg[0] belongs to the linked list
            if(head != NULL){
                struct node *current = malloc(sizeof(struct node));
                current = head;
            //printf("%d\n", size);
                printf("%s Hello\n", head->key);
                while(current != NULL){
                    printf("Hello\n");
                    if(strstr(myargs[0], current->key) != NULL){
                        myargs[0] = current->data;
                        myargs[1] = 0;
                        printf("MYARGS0: %s MYARGS1: %s\n", myargs[0], myargs[1]);
                        break;
                    }
                current = current->next;
                }
            }   
            if(strstr(myargs[0], "alias") != NULL){
                printf("entered\n");
                if(head == NULL){
                    head = malloc(sizeof(struct node));
                    head->key = myargs[1];
                    head->data = myargs[2];
                    head->next = NULL;
                }
                // } else if(next->key == NULL) {
                //     next->key = myargs[1];
                //     next->data = myargs[2];
                // } else {
                //     push(next, myargs[1], myargs[2]);
                // }
                continue;
            }
        }
        newProcess(myargs);
        if(argc == 1){
            printPrompt(); 
            continue;
        }
        
    }
    return(0);
}

    //Batch mode - arg[1] is the name of a file
    //1. check if file is valid, output error if not
    //2. open file, read line to stdout
    //3. complete command listed in file
    //4. repeat, put into loop

    //Interactive mode - output a prompt for user to type commands to
    //question: which commands do we need to include?
    //ls in tests,

    //redirection - output goes into a file rather than stdout
    //if exists, overwrite, else create a new file

    //aliasing = shortcuts
    //


    // int whitespace = 0;
    // for(int i = 0; i < strlen(buffer); i++){
    //     if(isspace(buffer[0] == 0)){
    //         whitespace = 1;
    //         break;
    //     }
    // }
    // if(whitespace == 0){
    //     continue;
    // }

