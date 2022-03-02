#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
//#include "linkedList.c"

//TO-DOs
//1. string concatenation to write function
//2. linked list for alias method (struct)
//3. redirection method
//4. unalias
//5. write the errors

void printPrompt(){
    write(STDOUT_FILENO, "mysh> ", strlen("mysh> "));                
}


int newProcess(char *myargs[]) {
    int rc = fork();
    int status;
    if (rc < 0) {   //Fork Error
        exit(1);
    } else if(rc == 0){
        int i = 0;
        while(myargs[i] != NULL){
            if(strchr(myargs[i], '>')){
                if(strlen(myargs[i]) != 1){
                    myargs[i] = strrchr(myargs[i], '>');
                    //*(myargs[i] + 0) = '\0';
                    printf("%s\n", myargs[i]);
                }
                break;
            }
            i++;
        }
        myargs[i] = '\0';
        printf("%s\n", myargs[i + 1]);
        execv(myargs[0], myargs);
    }
    else {
        rc = waitpid(rc, &status, 0);
    }
    return 0;
}

int checkWhiteSpace(char* buffer){
    int whitespace = 0;
    for(int i = 0; i < strlen(buffer); i++){
        if(isspace(buffer[i]) == 0){
            whitespace = 1;
            break;
        }
    }        
    return whitespace;
}     

int main(int argc, char *argv[]){
    //printf("%d", argc);
    FILE *fp;
    int flag = 0;
    const char delim[9] = " \n\t\v\t\r";

    if(argc < 0 || argc > 2) {
        write(STDERR_FILENO, "Usage: mysh [batch-file]\n", 25);
        return(1);
    }
    if(argc == 2){
        fp = fopen(argv[1], "r");
        flag = 1;
        if(fp == NULL){
            char str[] = "Error: Cannot open file ";
            strcat(str, argv[1]);
            strcat(str, ".\n");
            write(STDERR_FILENO, str, strlen(str));
            return(1);
       }
    }else {
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
        if (checkWhiteSpace(buffer) == 0) {
            if (flag == 0) {
                printPrompt();
            }   
            continue;
        }
        //seperating the line into the args -- redirection
        //if(strchr(buffer, '>')){
            // char * ptr = buffer;
            // char * second half
            // for (int i = 0; i < strlen(buffer); i++) {
            //     if (*ptr == ">") {
            //         *ptr = 0;
            //     }
            //     ptr++;
            // }
            //char errorString[] = "Redirection misformatted.\n";
            // if(buffer[0] == '>') {
            //     //write(1, errorString, strlen(errorString));
            //     write(1, "error1", 7);
            // }
            // token = strtok(buffer, ">");
            // printf("token 1:%s\n", token);
            // //printf("token:%s\n", token);
            // if (checkWhiteSpace(token) == 0) {
            //     //write(1, errorString, strlen(errorString));
            //     write(1, "error2", 7);
            // }
            
            // //char* firstHalf = strdup(token);
            // token = strtok(NULL, ">");
            // printf("token 2:%s\n", token);
            // token = strtok(NULL, ">");
            // printf("token 3:%s\n", token);
            // token = strtok(NULL, ">");
            // printf("token 4:%s\n", token);
            // if(token == NULL) {
            //     //write(1, errorString, strlen(errorString));
            //     write(1, "error3", 7);
            // }
            
            // if (token == NULL) {
                
            // }
        //     myargs[0] = token;
        //     int i = 1;
        //     while(token != NULL){
        //         token = strtok(NULL, delim);
        //         myargs[i] = token;
        //         printf("ARGUMENT: %s\n", myargs[i]);
        //         i++;
        //     }
        //     //redirection method -- better to do here or easier to do in new call?
        // } else if(strstr(buffer, "alias") != NULL){
        //     //struct node *linkedList = (struct node*) malloc(sizeof(struct node));

        //     //deliminate based on alias -- only 3 args so only the first 2 whitespaces
        //     //alias method
        //     //check if second argument is alias, unalias, or exit -- error
        //     //check if arg[0] is unalias or alias, then determine if add/subtract from LL
        // } 
        else {
            // MODIFIES MYARGS
            //check if arg[0] is in the linked list of alias

            //base case -- break the line into array of arguments
            token = strtok(buffer, delim);
            printf("%s", token);
            myargs[0] = token;
            int i = 1;
            while(token != NULL){
                token = strtok(NULL, delim);
                myargs[i] = token;
                printf("ARG: %s\n", myargs[i]);
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
    //1. check if
    // file is valid, output error if not
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
