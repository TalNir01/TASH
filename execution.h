#ifndef INCLUDERS_H
#define INCLUDERS_H value

#include "util.h"

#endif


#ifndef EXECUTION_H
#define EXECUTION_H value


int executeCommand(char *command[])// need to check redirection...
{	
	//printf("[%s][%s]\n", command[0], command[1]);
	if (execvp(command[0], command) < 0)
	{
		printf("execvp failed\n");
		exit(0);

	}
	return 0;
}

int pipedCommandExecution(structCMD * current, int numOfCommands)
{
	int numOfCmds = numOfCommands;
	int numOfPipes = numOfCmds - 1;

	int fd[numOfPipes * 2]; // Init 3 pipes, 3 * 2
	pid_t childsID[numOfCmds]; // An slot for each cmd

	int j = 0;
	int cmdIndex = 0;

	char ** temp = NULL;

	for (int i = 0; i < numOfPipes; ++i)
	{
		if (pipe(fd + i*2) < 0)
		{
			return 0;
		}
	}

	//printf("[DEBUG]-- Created Pipes.\n");
	
	while(current)
	{
	
		if ((childsID[cmdIndex] = fork()) < 0)
		{
			return 0; // Error on forking.
		}


		if (childsID[cmdIndex] == 0) // Is child
		{// Child executing
			//printf("About to tokenize @ pipedCommandExecution\n");
			temp = tokenize(" ", current->arguments);
			//printf("Done tokenize @ pipedCommandExecution\n");

			//If not the first command, set pipe STDIN_FILENO
			if (current->firstCmdInPipe == false) // not first
			{
				if (dup2(fd[j - 2], STDIN_FILENO) < 0) // Set the STDIN_FILENO     To the pipe
				{
					return 0; // Error
				}
			}

			//If not the last command
			if (current->lastCmdInPipe == false)// not last
			{
				if (dup2(fd[j + 1], STDOUT_FILENO) < 0) // Set the STDOUT_FILENO To the pipe
				{
					return 0; // Error
				}
			}


			//Closing all pipes
			for (int i = 0; i < 2*numOfPipes; ++i)
			{
				close(fd[i]);
			}
			executeCommand(temp);
    		free(temp);
		}
		//Index's increasment's.
		j += 2;
		cmdIndex++;
		current = current->next; // Moveup piped command

	}


	//Close all pipes...
	for (int i = 0; i < 2*numOfPipes; ++i)
	{
		close(fd[i]);
	}

	//Waitpid
	for (pid_t pId = 0; pId < numOfCmds; ++pId)
	{
		waitpid(childsID[pId], NULL, 0);
	}

	return 0;	
}


// (filename, STDOUT_FILENO \ STDIN_FILENO)
void redirect(char* redirectionFile, int redirectionFileno){
    int redirectionFD = 0, access = 0;
    //printf("@ redirect function\n");
    // Determine the FD to replace
    if(redirectionFileno == STDOUT_FILENO) access = O_WRONLY;
    else access = O_RDONLY;

    // Try to open the redirectionFile
    redirectionFD = open(redirectionFile, (access | O_CREAT), 0777);//(S_IWRITE | S_IREAD));

    // If failed to open
    if(redirectionFD == -1) {
        printf("Failed to open file %s for redirection.\n", redirectionFile);
        exit(1);
    }

    // Try to replace the FD
    if(dup2(redirectionFD, redirectionFileno) == -1) {
        puts("Failed to redirect io. Aborting...\n");
        exit(1);
    }

    close(redirectionFD);
}




#endif