#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "linkedList.c"

//TO-DOs
//1. string concatenation to write function
//2. linked list for alias method (struct)
//3. redirection method
//4. unalias

void printPrompt(){
    write(STDOUT_FILENO, "mysh> ", strlen("mysh> "));                
}


// int newProcess(char *argv[]){
//     int rc = fork();
//     if(rc < 0){
//         //fork failed, print error message
//     } else if(rc == 0){
//         char *myargs[3];
//         myargs[0] = strdup(argv[])
//     }
// }

// int batchMode(file fp){
//     while(fgets(out, 512, fp) != NULL){
//         char buffer[512];
//         char *str = fgets(buffer, 512, fp);
//         write(1, str, sizeof(str)); 
//                //check if command exists
//         int rc = fork();
//          if(rc == 0){ //child function
//                    //execv(command from file)
//           }

//        wait();
//      }
// }

// int interactiveMode(char *argv[]){
//     while(1){
//         char prompt[6] = "mysh> ";
//         write(1, prompt, strlen(prompt));
//         char buffer[512];
//         char* input = fgets(buffer, 512, stdin);

//     }
// }

int main(int argc, char *argv[]){
    FILE *fp;
    if(argc == 2){
        fp = fopen(argv[1], "r");
       if(fp == NULL){
           //char errorname[] = "Error: Cannot open file .\n";
          // write(2, errorname, 20);
       }
    }else{
        fp = stdin;
        printPrompt();
    }
    char buffer[512];
    while(fgets(buffer, 512, fp)){
        //case one -- check if blank line in file/stdin
        int whitespace = 0;
        for(int i = 0; i < strlen(buffer); i++){
            if(isspace(buffer[0] == 0)){
                whitespace = 1;
                break;
            }
        }
        if(whitespace == 0){
            continue;
        }
        char *myargs[sizeof(buffer)];
        myargs[0] = "hi";
        printf("%s", myargs[0]);
        //seperating the line into the args
        if(strstr(buffer, ">") != NULL){
            // MODIFIES MYARGS
            //space deliminating based on > and whitespace
            
            //redirection method
        } else if(strstr(buffer, "alias") != NULL){
            struct node *linkedList = (struct node*) malloc(sizeof(struct node));

            //deliminate based on alias -- only 3 args so only the first 2 whitespaces
            //alias method
            //check if second argument is alias, unalias, or exit -- error
            //check if arg[0] is unalias or alias, then determine if add/subtract from LL
        } else{
            // MODIFIES MYARGS
            //check if arg[0] is in the linked list of alias
            myargs[0] = strtok(buffer, " ");
            for(int i = 0; i < strlen(buffer); i++){
                i++;
                myargs[i]
            }
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
