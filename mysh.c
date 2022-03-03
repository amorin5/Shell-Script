#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include<fcntl.h>

// alias method functionality --
// CHECKS -- buffer contains alias OR unalias, check if buffer ONLY contains alias, check if alias
// already exists, check if key == unalias, alias, or exit
// 1. add nodes to the linked list for multiple aliases, override for keys
// 2. remove nodes for unaliasing
// 3. print method for printing all keys (cmd alias)
//* special case * alias redirection


struct alias {
    char *aliasName;
    char *argv[256];
    struct alias *nextAlias;
    
};

// void push(struct node *next, char *new_data, char *new_key) {
//     printf("called\n");
//     struct node *new_node = malloc(sizeof(struct node));
//     new_node->data = new_data;
//     new_node->key = new_key;
//     new_node->next = next;
//     next = new_node;
// }

void printPrompt() {
    write(STDOUT_FILENO, "mysh> ", strlen("mysh> "));
}

int redirection(char* myargs[]){
   int i = 0;
   while(myargs[i] != NULL){
        if(strchr(myargs[i], '>')){
            if(strlen(myargs[i]) != 1){
                myargs[i] = strrchr(myargs[i], '>');
                //TODO handle for whitespace
            }
            if(myargs[i + 2] != NULL){
                write(STDERR_FILENO, "Redirection misformatted.\n", 26);
                return 1;
            }
            myargs[i] = '\0';
            break;
        }
        i++;
    }

    char *fn = myargs[i + 1];
   // char *writeTo = myargs[i - 1];
    FILE *fd = fopen(fn, "wb+");
    if(fd == NULL){
        perror("Error in redirecting and opening file: ");
        return 1;
    }
    //write(1, fn, strlen(fn));
    dup2(fileno(fd), STDOUT_FILENO);
    // fork();
    // execv(myargs[0], myargs);
    //write(fd, writeTo, strlen(writeTo));
    //fopen(fn, "w+");
    return 0;
}


void newProcess(char *myargs[]) {
    int rc = fork();
    int status;
    if (rc < 0) { 
        exit(1);
    }
    else if (rc == 0) {
        // TODO: handle redirection
        int i = 0;
        while(myargs[i] != NULL){
            if(strchr(myargs[i], '>')){
                if(redirection(myargs) == 1){
                    _exit(0);
                }
                break;
            }
            i++;
        }
        execv(myargs[0], myargs);
        // TODO: handle if execv fails print error message
        _exit(0);
    }
    else {
        waitpid(rc, &status, 0);
    }
}

struct alias *head = NULL;

void printAlias() {
    struct alias *current = malloc(sizeof(struct alias));
    current = head;
    while (current != NULL) {
        write(1, current->aliasName, strlen(current->aliasName));
        write(1, " ", 1);
        int i = 0;
        while(current->argv[i] != NULL) {
            write(1, current->argv[i], strlen(current->argv[i]));
            if(current->argv[i + 1] != NULL) {
                write(1, " ", 1);
            }
            i++;
        }
        current = current->nextAlias;
    }
    free(current);
}

void alias(char *myargs[], int argc) {
    struct alias *current = malloc(sizeof(struct alias));
    current = head;

    if (argc == 1) {
        // print aliases
        while (current != NULL) {
            printAlias();
            current = current->nextAlias;
        }
    }
    else if (argc == 2) {
        // check if alias exists and print it if it does
        while (current != NULL) {
            if(myargs[1] == current->aliasName) {
                printAlias();
                return;
            }
            current = current->nextAlias;
        }
    }
    else {
        // go through aliases, check if it exists and override if it does
        // add to the end if doesn't
        printf("Deal with adding the alias\n");
        if (head == NULL) {
            head = malloc(sizeof(struct alias));
            head->aliasName = strdup(myargs[1]);
            //char *aliasArgs[256];
            int i = 2;
            while(myargs[i] != NULL) {
                head->argv[i-2] = strdup(myargs[i]);
                i++;
            }
            head->argv[i-2] = NULL;
            //head->argv = aliasArgs;
            head->nextAlias = NULL;
        }

    }

    free(current);
}

void unalias(char* myargs[], int argc) {
    char unaliasError[] = "unalias: Incorrect number of arguments.\n";

    if (argc <= 1 || argc > 2) {
        write(1, unaliasError, strlen(unaliasError));
    }
    else {
        printf("check if alias exists and delete if it does\n");
    }
}

void exitShell() {
    return;
    // TODO: clean up all the memory
}


void processCommand(char* buffer) {
    char *myargs[256];
    char *token;
    const char delim[] = " \n\t\v\t\r";

    // TODO: Redirection code

    token = strtok(buffer, delim);
    int count = 0;
    
    while (token != NULL) {
        myargs[count] = token;
        count++;
        token = strtok(NULL, delim);
    }
    myargs[count] = NULL;

    // check if alias exists
    // if alias exists, run command there
    // if (checkAlias(myArgs) == 1) {
    //     return;
    // }
    if (strcmp(myargs[0], "alias") == 0) {
        alias(myargs, count);
    }
    else if (strcmp(myargs[0], "unalias") == 0) {
        unalias(myargs, count);
    }
    else if (strcmp(myargs[0], "exit") == 0) {
        exitShell();
        return;
    }
    else {
        newProcess(myargs);
    }  
}

void interactive(int argc, char *argv[]) {
    FILE *fp;
    fp = stdin;
    char buffer[512];

    printPrompt();

    while (fgets(buffer, 512, fp) != NULL) {
        processCommand(buffer);
        if(strstr(buffer, "exit")){
            return;
        }
        printPrompt();
    }
}

int main(int argc, char *argv[]) {
    FILE *fp;
    char buffer[512];
    //int flag = 0;

    if (argc < 1 || argc > 2) {
        write(STDERR_FILENO, "Usage: mysh [batch-file]\n", 25);
        return 1;
    }
    else if (argc == 1) {
        interactive(argc, argv);
    }
    else {
        fp = fopen(argv[1], "r");
        if (fp == NULL) {
            char str[] = "Error: Cannot open file ";
            strcat(str, argv[1]);
            strcat(str, ".\n");
            write(STDERR_FILENO, str, strlen(str));
            return 1;
        }

        while (fgets(buffer, 512, fp) != NULL) {
            write(1, buffer, strlen(buffer));
            processCommand(buffer);
            if(strstr(buffer, "exit")){
                return 0;
            }
        }
        fclose(fp);
    }
    return 0;
}
