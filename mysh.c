#include <stdlib.h>

void main(int argc, char **argv){
    if(argc > 2){
        printf("Usage: ./mysh [arg]");
    }
    //sample 
    // bruh

    //Bash mode - arg[1] is the name of a file
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
}
