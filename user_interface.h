#ifndef UI_H
#define UI_H value



#define clear() printf("\033[H\033[J")

#define reset_color() printf("\033[0m")

#define red() printf("\033[0;31m")

#define green() printf("\033[32;1m")

#define blue() printf("\033[34;m")


void init_shell()
{
	green();
    clear();
    printf("\n\n\n\n******************"
        "************************");
    printf("\n\n\n\t***** TASH *****");
    printf("\n\n\t- T4L N1R 8A5H 5HELL -");
    printf("\n\n\n\n*******************"
        "***********************");
    char* username = getenv("USER");
    printf("\n\n\nUSER is: @%s", username);
    printf("\n");
    //sleep(1);
    //clear();
    reset_color();

}

void printUI()
{
	char cwd[1024];
	
	char* username = getenv("USER");
	if (getcwd(cwd, sizeof(cwd)) == NULL) {
       perror("getcwd() error");
       return ;
	}

    reset_color();
    green();
	printf("\n%s@", username);
	red();
	printf("~%s", cwd);
	reset_color();
	printf("-> ");
}



#endif