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

// TO-DOs
// 2. linked list for alias method (struct)
// 3. redirection method
// 4. unalias

struct alias {
    struct alias *nextAlias;
    char *aliasName;
    char *argv[512];
    
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

int checkWhiteSpace(char *buffer) {
    int whitespace = 0;
    for (int i = 0; i < strlen(buffer); i++) {
        if (isspace(buffer[i]) == 0) {
            whitespace = 1;
            break;
        }
    }
    return whitespace;
}

int newProcess(char *myargs[]) {
    // printf("NEW PROCESS RUN\n");
    int rc = fork();
    int status;
    if (rc < 0) { // Fork Error
        exit(1);
    }
    else if (rc == 0) {
        execv(myargs[0], myargs);
    }
    else {
        rc = waitpid(rc, &status, 0);
    }
    return 0;
}

int alias(char *myargs[], int argc) {
    if (argc == 1) {
        printf("It's just allias\n");
    }
    else if (argc == 2) {
        printf("Check if alias exists and print it out of it does");
    }
    else {
        printf("Deal with adding the alias");
    }
    return 0;
}

int unalias(char* myargs[], int argc) {
    char unaliasError[] = "unalias: Incorrect number of arguments.\n";

    if (argc <= 1 || argc > 2) {
        write(1, unaliasError, strlen(unaliasError));
        return 1;
    }
    else {
        printf("check if alias exists and delete if it does");
    }
    return 0;
}

int main(int argc, char *argv[]) {
    FILE *fp;
    int flag = 0;
    const char delim[9] = " \n\t\v\t\r";

    if (argc < 1 || argc > 2) {
        write(STDERR_FILENO, "Usage: mysh [batch-file]\n", 25);
        return (1);
    }
    else if (argc == 2) {
        fp = fopen(argv[1], "r");
        flag = 1;
        if (fp == NULL) {
            char str[] = "Error: Cannot open file ";
            strcat(str, argv[1]);
            strcat(str, ".\n");
            write(STDERR_FILENO, str, strlen(str));
            return (1);
        }
    }
    else {
        fp = stdin;
        printPrompt();
    }

    char buffer[512];
    char *myargs[sizeof(buffer)];
    char *token;
    //struct node *head = NULL;

    while (fgets(buffer, 512, fp)) {
        int j = 0;
        while (myargs[j] != NULL) {
            myargs[j] = 0;
            j++;
        }

        if (flag == 1) {
            write(1, buffer, strlen(buffer));
        }

        if (checkWhiteSpace(buffer) == 0) {
            if (flag == 0) {
                printPrompt();
            }
            continue;
        }

        token = strtok(buffer, delim);
        myargs[0] = token;
        int count = 1;
        while (token != NULL) {
            token = strtok(NULL, delim);
            myargs[count] = token;
            count++;
        }
        myargs[count] = '\0';

        if (strcmp(myargs[0], "alias") == 0) {
            printf("alias mode");
            alias(myargs, count -1);
        }
        else if (strcmp(myargs[0], "unalias") == 0) {
            printf("unalilas mode");
            unalias(myargs, count - 1);
        }
        else {
            // base case -- break the line into array of arguments
            // token = strtok(buffer, delim);
            // myargs[0] = token;
            // int i = 1;
            // while (token != NULL) {
            //     token = strtok(NULL, delim);
            //     myargs[i] = token;
            //     i++;
            // }
            // check if arg[0] belongs to the linked list
            // if (head != NULL) {
            //     struct node *current = malloc(sizeof(struct node));
            //     current = head;
            //     printf("%s Hello\n", head->key);
            //     while (current != NULL) {
            //         printf("Hello\n");
            //         if (strstr(myargs[0], current->key) != NULL) {
            //             myargs[0] = current->data;
            //             // myargs[1] = 0;
            //             printf("MYARGS0: %s MYARGS1: %s\n", myargs[0], myargs[1]);
            //             break;
            //         }
            //         current = current->next;
            //     }
            // }
        }
        // printf("reached process");
        newProcess(myargs);
        if (argc == 1) {
            printPrompt();
            continue;
        }
    }
    return (0);
}

// Batch mode - arg[1] is the name of a file
// 1. check if file is valid, output error if not
// 2. open file, read line to stdout
// 3. complete command listed in file
// 4. repeat, put into loop

// Interactive mode - output a prompt for user to type commands to
// question: which commands do we need to include?
// ls in tests,

// redirection - output goes into a file rather than stdout
// if exists, overwrite, else create a new file

// aliasing = shortcuts
