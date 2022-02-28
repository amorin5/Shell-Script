#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include<sys/wait.h>
//#include "linkedList.c"
b
//TO-DOs
//1. string concatenation to write function
//2. linked list for alias method (struct)
//3. redirection method
//4. unalias
//5. write the errors

void printPrompt(){
    write(STDOUT_FILENO, "mysh> ", strlen("mysh> "));                
}


int newProcess(char *myargs[]){
    //write(1, myargs[0], strlen(myargs[0]));
    //write(1, myargs[1], strlen(myargs[1]));
    int rc = fork();
    if (rc < 0) {   //Fork Error
        exit(1);
    } else if(rc == 0){
        execv(myargs[0], myargs);
    }
    else {
       rc = wait(NULL);
    }
    return 0;
}

int main(int argc, char *argv[]){
    FILE *fp;
    int flag = 0;
    if(argc == 2){
        fp = fopen(argv[1], "r");
        flag = 1;
       if(fp == NULL){
           //char errorname[] = "Error: Cannot open file .\n";
          // write(2, errorname, 20);
       }
    }else{
        fp = stdin;
        printPrompt();
    }
    char buffer[512];
    char *myargs[sizeof(buffer)];
    char *token;
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
        } else if(strstr(buffer, "alias") != NULL){
            //struct node *linkedList = (struct node*) malloc(sizeof(struct node));

            //deliminate based on alias -- only 3 args so only the first 2 whitespaces
            //alias method
            //check if second argument is alias, unalias, or exit -- error
            //check if arg[0] is unalias or alias, then determine if add/subtract from LL
        } else {
            // MODIFIES MYARGS
            //check if arg[0] is in the linked list of alias

            //base case -- break the line into array of arguments
            token = strtok(buffer, " \n");
            myargs[0] = token;
            int i = 1;
            while(token != NULL){
                token = strtok(NULL, " \n");
                myargs[i] = token;
                i++;
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

