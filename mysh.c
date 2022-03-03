#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>

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

void printPrompt() {
    write(STDOUT_FILENO, "mysh> ", strlen("mysh> "));
}

void newProcess(char *myargs[]) {
    int rc = fork();
    int status;
    if (rc < 0) { 
        exit(1);
    }
    else if (rc == 0) {
        // TODO: handle redirection
        execv(myargs[0], myargs);
        // TODO: handle if execv fails print error message
        _exit(0);
    }
    else {
        waitpid(rc, &status, 0);
    }
}

struct alias *head = NULL;

void printAlias(struct alias *current) {
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
    write(1, "\n", 1);
}

void alias(char *myargs[], int argc) {
    if (argc == 1) {
        // print aliases
        struct alias *current = head;
        while (current != NULL) {
            printAlias(current);
            current = current->nextAlias;
        }
    }
    else if (argc == 2) {
        // check if alias exists and print it if it does
        struct alias *current = head;
        while (current != NULL) {
            if(strcmp(myargs[1], current->aliasName) == 0) {
                printAlias(current);
                return;
            }
            current = current->nextAlias;
        }
    }
    else {
        // go through aliases, check if it exists and override if it does
        if (head == NULL) {
            head = malloc(sizeof(struct alias));
            head->aliasName = strdup(myargs[1]);
            int i = 2;
            while(myargs[i] != NULL) {
                head->argv[i-2] = strdup(myargs[i]);
                i++;
            }
            head->argv[i-2] = NULL;
            head->nextAlias = NULL;
        }
        else {
            struct alias *current = head;
            while (current != NULL) {
                if(strcmp(current->aliasName, myargs[1]) == 0) {
                    int i = 2;
                    while(myargs[i] != NULL) {
                        head->argv[i-2] = strdup(myargs[i]);
                        i++;
                    }
                    head->argv[i-2] = NULL;
                    return;
                }
                if (current->nextAlias == NULL) {
                    break;
                }
                current = current->nextAlias;
            }
            struct alias* newAlias = malloc(sizeof(struct alias));
            newAlias->aliasName = strdup(myargs[1]);
            int i = 2;
            while(myargs[i] != NULL) {
                newAlias->argv[i-2] = strdup(myargs[i]);
                i++;
            }
            newAlias->argv[i-2] = NULL;
            newAlias->nextAlias = NULL;
            current->nextAlias = newAlias;
        }
    }
}

void unalias(char* myargs[], int argc) {
    char unaliasError[] = "unalias: Incorrect number of arguments.\n";

    if (argc <= 1 || argc > 2) {
        write(1, unaliasError, strlen(unaliasError));
    }
    else {
        struct alias *current = head;
        struct alias *prev = NULL;
        
        while (current != NULL) {
            if(strcmp(current->aliasName, myargs[1]) == 0) {
                if (prev == NULL && current->nextAlias == NULL) {
                    free(current);
                    head = NULL;
                    return; 
                }
                else if(prev == NULL) {
                    head = head->nextAlias;
                    free(current);
                    return; 
                }
                else if(current->nextAlias == NULL) {
                    prev->nextAlias = NULL;
                    free(current);
                    return;
                }
                else {
                    prev->nextAlias = current->nextAlias;
                    free(current);
                    return;
                }
            }
            prev = current;
            current = current->nextAlias;
        }
    }
}

int checkAlias(char* myargs[]) {
    struct alias* current = head;
    
    while (current != NULL) {
        if (strcmp(current->aliasName, myargs[0]) == 0) {
            char* newArgs[256];
            int i = 0;
            while (current->argv[i] != NULL) {
                newArgs[i] = current->argv[i];
                i++;
            }
            int j = 1;
            while (myargs[j] != NULL) {
                newArgs[i] = myargs[j];
                i++;
                j++;
            }
            newArgs[i] = NULL;
            newProcess(newArgs); 
            return 1;  
        }
        current = current->nextAlias;
    }
    return 0;
}
void exitShell() {
    //printf("User typed exit");
    exit(0);
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

    if (checkAlias(myargs) == 1) {
        return;
    }

    if (strcmp(myargs[0], "alias") == 0) {
        alias(myargs, count);
    }
    else if (strcmp(myargs[0], "unalias") == 0) {
        unalias(myargs, count);
    }
    else if (strcmp(myargs[0], "exit") == 0) {
        exitShell();
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
        }
        fclose(fp);
    }
    return 0;
}
