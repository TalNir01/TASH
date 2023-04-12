#ifndef PARSING_H
#define PARSING_H value


int counterInStr(char symbol, char * line)
{
	int counter = 0;
	for (int i = 0; i < strlen(line); ++i)
	{
		if (line[i] == symbol)
		{
			counter++;
		}
	}

	return counter;
}

int searchInStr(char symbol, char * line)
{
	for (int i = 0; i < strlen(line); ++i)
	{
		if (line[i] == symbol)
		{
			return i;
		}
	}

	return -1;

}


char* findRedirectionFileAndCut(char redirectionSym, char **args) {
    int i;
    for (i = 0; args[i]; i++) {
        if(args[i][0] == redirectionSym) {
            // If there is more than one parameter after the '>/<'
            // Or the parameter where the file name should be is empty
            //if(args[i+2] || !args[i+1]){
            //    puts("Illegal file name for io redirection. Aborting...");
            //    exit(1);
            //}
            // If legal
            args[i] = NULL; // Terminate parameters to execvp before redirection
            return args[i+1];
        }
    }

    return NULL;
}


char* findRedirectionFile(char redirectionSym, char **args) {
    int i;
    for (i = 0; args[i]; i++) {
        if(args[i][0] == redirectionSym) {
            // If there is more than one parameter after the '>/<'
            // Or the parameter where the file name should be is empty
            //printf("args[i+1] - %s\n", args[i+1]);

            //if(args[i+2] || !args[i+1]){
            //    puts("Illegal file name for io redirection. Aborting...");
            //    exit(1);
            //}
            // If legal
            //args[i] = NULL; // Terminate parameters to execvp before redirection
            return args[i+1];
        }
    }

    return NULL;
}

int processRedirection(char *line, int InRedirectIndex, int OutRedirectIndex)
{
	char * redirectOutputFileName = NULL;
	char * redirectInputFileName = NULL;
	char **argsForRedirection = NULL;
	//printf("At processRedirection\n");

	char lineCopy[strlen(line)]; // will be used to process redirection information.

	strcpy(lineCopy, line);

	argsForRedirection = tokenize(" ", lineCopy);

	//printf("InRedirectIndex = %d , OutRedirectIndex = %d\n", InRedirectIndex, OutRedirectIndex);

	if (InRedirectIndex > -1 && OutRedirectIndex > -1) // There is both output and input redirection
	{
		redirectOutputFileName = findRedirectionFile('>', argsForRedirection);


		redirectInputFileName = findRedirectionFile('<', argsForRedirection);
	
		//printf("input redirection -> [%s]\n", redirectInputFileName);
		//printf("output redirection -> [%s]\n", redirectOutputFileName);

		//actual call to the redirect function...
		redirect(redirectOutputFileName, STDOUT_FILENO);
		redirect(redirectInputFileName, STDIN_FILENO);

		//cut from line the redirection...
		if (InRedirectIndex < OutRedirectIndex)
		{
			line[InRedirectIndex] = '\0';
		}
		else
		{
			line[OutRedirectIndex] = '\0';
		}


	}
	else if(InRedirectIndex > -1)//there is input redirect
	{

		redirectInputFileName = findRedirectionFile('<', argsForRedirection);
	
		//printf("Only input redirection -> [%s]\n", redirectInputFileName);
		
		redirect(redirectInputFileName, STDIN_FILENO);


		//cut from original line
		line[InRedirectIndex] = '\0';

	}
	else if(OutRedirectIndex > -1)//there is input redirect
	{

		redirectOutputFileName = findRedirectionFile('>', argsForRedirection);
	
		//printf("Only output redirection -> [%s]\n", redirectOutputFileName);
		
		redirect(redirectOutputFileName, STDOUT_FILENO);

		//cut from original line
		line[OutRedirectIndex] = '\0';

	}
	else
	{
		// NO redirection at all.
		return -1;

	}



	return 1;


}


char *str_replace(char *orig, char *rep, char *with) {
    char *result = NULL; // the return string
    char *ins = NULL;    // the next insert point
    char *tmp = NULL;    // varies
    int len_rep = 0;  // length of rep (the string to remove)
    int len_with = 0; // length of with (the string to replace rep with)
    int len_front = 0; // distance between rep and end of last rep
    int count = 0;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}







structCMD * parseCommand(char * line, int numOfCommands)
{
//lets start by spliting the line by " "
	char lineBackUp[strlen(line)];
	strcpy(lineBackUp, line);
	char *res = NULL;
	char **commands = NULL;
	//char **args = tokenize(" ", line);
	res = str_replace(line, "| ", "@");

	commands = tokenize("@", res); // char ** array of commands
	
	//let's start building the linked-list
	structCMD *headNode = NULL, *currentNode = NULL, *temp = NULL;

	//printf("About to enter the loop, numOfCommands == [%d]\n", numOfCommands);
	for (int i = 0; i < numOfCommands; ++i)
	{
		//printf("Start loop at i=%d\n", i);

		
		//printf("About to allocate memory\n");
		//allocate memory
		currentNode = (structCMD *)malloc(sizeof(structCMD));

		currentNode->next = (structCMD *)malloc(sizeof(structCMD *));

		currentNode->arguments = (char*)malloc(sizeof(commands[i]));
		strcpy(currentNode->arguments, commands[i]);
		
		//printf("Finished allocating memory\n");		

		//Set Pipe status
		if (i == 0)
		{
			currentNode->firstCmdInPipe = true;
		}

		if (i == numOfCommands - 1)
		{
			currentNode->lastCmdInPipe = true;
		}

		//printf("About to connect the list\n");
		// Build the Linked-List
		if (i == 0)
		{
			headNode = currentNode; // create list head
			temp = currentNode;
			//printf("Set head to -> temp @ 300 = {%s}\n", temp->arguments);

		}
		else{
			// link the list
			temp->next = currentNode;
			temp = currentNode;
	 		//printf("newNode temp = {%s}\n", temp->arguments);

		}
		//printf("Done linking at i=%d\n", i);
 		

	}
	//9
    temp->next = NULL;

    //printf("Finished the list\n");

    //10
    //temp = headNode;

    //printf("About to return head\n");


   
	return headNode;
}




void HandleParsing(char * line)
{
	/*** Init ***/
	//init:
	//printf("@ init\n");
	int numberOfCmd = 0;

	int InRedirectIndex = searchInStr(REDIRECTION_INPUT, line);
	int OutRedirectIndex = searchInStr(REDIRECTION_OUTPUT, line);
	//printf("%d\n", OutRedirectIndex);
	structCMD * list_head = NULL; // create pointer
	structCMD * tempFree = NULL;
	/*** create undirection backup ***/
	//create_unredirection_backup:
	//printf("@ create_unredirection_backup\n");
	int stdoutCopy = dup(STDOUT_FILENO);
	int stdinCopy = dup(STDIN_FILENO);

	/*** redirect (process redirection...) ***/
	//redirect_section:
	processRedirection(line, InRedirectIndex, OutRedirectIndex); // change line
	//printf("@ redirect_section\n");

	/*** calculate numberOfCommands ***/
	//calc_num_of_cmds:
	//printf("@ calc_num_of_cmds\n");
	numberOfCmd = counterInStr(PIPE_SYMBOL, line) + 1;
	//Add better calaculation
	//Add Later...


	/*** execution / processCommand ***/
	//process_cmd_execute:
	//printf("@ process_cmd_execute\n");
	//printf("[%s]\n", line);
	list_head = parseCommand(line, numberOfCmd);
	// Calling the execute pipe command...
	pipedCommandExecution(list_head, numberOfCmd);
	//printf("come back\n");


	/*** un-redirect ***/
	//unredirect:
	//printf("@ unredirect\n");
	//Fixing STDOUT
	if (dup2(stdoutCopy, STDOUT_FILENO) < 0){
		printf("dup error @ fixing STDOUT\n");
	}

	close(stdoutCopy);

	//Fixing STDIN
	if (dup2(stdinCopy, STDIN_FILENO) < 0)
	{
		printf("dup error @ fixing STDOUT\n");
	}

	close(stdinCopy);

	/*** cleanup - free allocated memory ***/
	//cleanup:
	while(list_head != NULL)
	{
		tempFree = list_head;
		list_head = list_head->next;
		free(tempFree);
	}
	//finish
	//finish:
	//printf("@ finish\n");

}







#endif