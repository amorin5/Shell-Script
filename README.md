## Custom Shell Script
This repository contains code to run a custom shell that will take the input arguments and run them as if they were typed into the command line.
To run: clone repo and run Makefile.

### Features:
Interactive Mode: when run from the command line with no arguments, a display prompt will appear allowing users to type into mysh> as if it is a command line.

Batch Mode: when run with a filename as the argument, the shell will read commands out of the file and execute them in order.

Redirection: user can redirect shell output into a file of their chosing, passed in after a > symbol. 

Alias: user can save a command under an alias and run the command through that alias. 
