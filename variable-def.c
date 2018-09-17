#include "header.h"

/* Lines providing variable definitions are of the form:

NAME = VALUE

where any number of whitespace characters (spaces and tabs) may appear before and after the '=' character. 

If the pattern $(NAME), known as a variable expansion, appears anywhere on any following text line, the pattern $(NAME) is replaced at that point by the characters of VALUE.

If a variable expansion requests a variable NAME that has not been previously defined in the file, then the value of NAME is sought from the bake process's environment. 

Thus the variable expansion $(HOME) might be expanded with the value /User/home/chris at that point. 

If no value of NAME can be found anywhere, the empty string is 'inserted' at the current point.

As special cases, the variable expansions $(PID), $(PPID), $(PWD), and $(RAND) are replaced by the formatted results of the function calls getpid(), getppid(), getcwd(), and rand() respectively.

Any line may have multiple variable expansions.

 Note that a variable expansion may occur at any point in the text file, including in the NAME and VALUE parts of a new variable definition line itself.

A typical expample is:

C99 = cc -std=c99 -Wall -pedantic -Werror

C99 functions and system-calls that will support variable definitions - calloc(), strdup(), and getenv().*/

