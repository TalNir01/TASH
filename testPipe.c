#include "util.h"
#include "execution.h"
#include "parsing.h"
#include "user_interface.h"
//#include "user_interface.h"


// This function receives the next input
void getInputString(char *dest) {
    int i = 0;
    char curr = 0;

    // get up to COMMAND_MAX_LENGTH characters
    for (i = 0; i < COMMAND_MAX_LENGTH - 1; ++i) {
        curr = getchar();
        if(curr == '\n') break;

        *dest = curr;
        dest++;
    }

    // NULL terminate
    *dest = '\0';
}




int main(int argc, char const *argv[])
{
	char line[1024];
	boolean exitFlag = false;
	init_shell();
	while(exitFlag == false){
		printUI();
		getInputString(line);
		reset_color();

		if (strcmp(line, EXIT_COMMAND) == 0)
		{
			printf("Exit...\n");
			exitFlag = true;
		}
		else{
			HandleParsing(line);
		}
	}
	return 0;
}
