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
    char *argv[256];
    
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

void newProcess(char *myargs[]) {
    int rc = fork();
    int status;
    if (rc < 0) { // Fork Error
        exit(1);
    }
    else if (rc == 0) {
        execv(myargs[0], myargs);;
        _exit(0);
    }
    else {
        waitpid(rc, &status, 0);
    }
}

void alias(char *myargs[], int argc) {
    if (argc == 1) {
        printf("It's just alias\n");
    }
    else if (argc == 2) {
        printf("Check if alias exists and print it out of it does\n");
    }
    else {
        printf("Deal with adding the alias\n");
    }
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

void processCommand(char* buffer) {
    char *myargs[256];
    char *token;
    const char delim[] = " \n\t\v\t\r";

    token = strtok(buffer, delim);
    int count = 0;
    
    while (token != NULL) {
        myargs[count] = token;
        count++;
        token = strtok(NULL, delim);
    }
    myargs[count] = NULL;

    if (strcmp(myargs[0], "alias") == 0) {
        //printf("alias mode\n");
        alias(myargs, count);
    }
    else if (strcmp(myargs[0], "unalias") == 0) {
        //printf("unalias mode\n");
        unalias(myargs, count);
    }
    else {
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
