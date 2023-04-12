#ifndef UTILITIES_H
#define UTILITIES_H

// C Program to design a shell in Linux

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

// BOOLEAN VALUES
#define true            1
#define false           0
#define boolean unsigned char
#define REDIRECTION_INPUT '<'
#define REDIRECTION_OUTPUT '>'
#define SPACE ' '
#define PIPE_SYMBOL '|'

//PARSING CONST
#define COMMAND_MAX_LENGTH 1024
#define EXIT_COMMAND "exit"


typedef struct command2
{
	boolean firstCmdInPipe : 1; // 0 - False, 1 - True
	boolean lastCmdInPipe : 1; // 0 - False, 1 - True

	// Pointer...
	struct command2 * next; // Pointer to the next piped command

	char *arguments; // {"app arg1 arg2"}
} structCMD;

char ** tokenize(char * d, const char *toParse) {
    int inputLength = strlen(toParse), i = 0,
        maxArgsNum = inputLength/2 + inputLength%2; // Assuming every param is 1 character long
    char *tmp = NULL, **args = (char **)calloc(maxArgsNum + 1, sizeof(char *)); // The +1 is for the NULL terminetor

    // Get the tokens
    tmp = strtok(toParse, d);

    while(tmp != NULL) {
        args[i++] = tmp;
        tmp = strtok(NULL, d);
    }

    // Null terminate the array
    args[i++] = NULL;

    return args;
}



#endif