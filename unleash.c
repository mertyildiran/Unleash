/************************************************************************
	@file				unleash.c
	@author 			Mehmet Mert Yildiran	mert.yildiran@bil.omu.edu.tr
	@date				Thursday, March 17, 2016
	@brief				Main program of Unleash Shell
	@reservedcommands	help, exit, cd
************************************************************************/
#define _PROGRAM_NAME "unleash"
#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include "asciiart.h"
#define MAX_LEN 128

char *reserved_cmds[] = {
	"cd      Changes current working directory    $ cd ../dir/",
	"help    Opens Help section",
	"exit    Exits Unleash Shell",
	"quit    Exits Unleash Shell"
};

int UnleashReservedCmdCounter(void) {
	return sizeof(reserved_cmds) / sizeof(char *);
}

const char *GetUsername(void)
{
	register struct passwd *pw;
	register uid_t uid;
	char *USERNAME[100];

	uid = geteuid();
	pw = getpwuid(uid);
	if (pw)
	{
		*USERNAME = pw->pw_name;
		return *USERNAME;
	}
}

const char *GetHomeDir(void)
{
	struct passwd *pw = getpwuid(getuid());
	const char *homedir = pw->pw_dir;
	return homedir;
}


int UnleashHelp(void)
{
	int i;
	printf("WELCOME TO UNLEASH SHELL HELP SECTION\n\n");
	printf("Type program names and arguments, and hit enter.\n");
	printf("The following commands are reserved:\n\n");

	for (i = 0; i < UnleashReservedCmdCounter(); i++) {
		printf("  %s\n\n", reserved_cmds[i]);
	  }

	printf("Use the man command for information on other programs.\n");
	return 1;
}

float RandomFloat(void)
{
	return (float)rand()/(RAND_MAX+1.0);
}

int ArgumentExtractor(char* string, char* argv[])
{
	char* p = string;
	int argc = 0;

	while(*p != '\0')
	{
		while(isspace(*p))
			++p;

		if(*p == '#') break;
		if(*p != '\0')
			argv[argc++] = p;
		else
			break;

		while(*p != '\0' && !isspace(*p))
			p++;
		if(*p != '\0')
		{
			*p = '\0';
			++p;
		}

	}

	return argc;
}

int UnleashExecute(char **args)
{
  pid_t pid;
  int status;

  pid = fork();
  if (pid == 0) {
	// Child process
	if (execvp(args[0], args) == -1) {
	  perror("Unleash");
	}
	exit(EXIT_FAILURE);
  } else if (pid < 0) {
	// Error forking
	perror("Unleash");
  } else {
	// Parent process
	do {
	  waitpid(pid, &status, WUNTRACED);
	} while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

void Unleash(void)
{
	const char *HOSTNAME[150];
	char _hostname[150];
	const char *USERNAME[100];
	const char *HOMEDIR[100];
	char cwd[1024];
	char command[50];
	int status = 1;
	char outcwd[1024];
	char *args[1000];
	srand(time(NULL));

	AsciiArtPrinter();

	gethostname(_hostname, 150);
	*HOSTNAME = _hostname;

	*USERNAME = GetUsername();
	*HOMEDIR = GetHomeDir();

	do {

		getcwd(cwd, sizeof(cwd));
		if (strstr(cwd, *HOMEDIR) != NULL) {
			int homelen = strlen(*HOMEDIR);
			char *chopcwd[1024];
			*chopcwd = &cwd[homelen-1];
			strcpy(outcwd, *chopcwd);
			outcwd[0] = '~';
		}

		printf("%s@%s:%s| ", *USERNAME, *HOSTNAME, outcwd);
		fgets(command,150,stdin);
		//gets(command);

		printf("-----------------------------------------------------------------------------\n");

		int ac = ArgumentExtractor(command, args);
		//printf("Length of array: %d\n", (int)( sizeof(&args) ));
		//puts(&args[1]);
		//printf("%s\n",args[ac-1]);

		if(ac > 0){
			if (strcmp(args[0], "exit") == 0 || strcmp(args[0], "quit") == 0) {
				printf("%s\n", "You're a QUITTER!");
				return;
			}
			else if(strcmp(args[0], "help") == 0) {
				UnleashHelp();
			}
			else if(strcmp(args[0], "who") == 0) {
				time_t rawtime;
				struct tm * timeinfo;
				time ( &rawtime );
				timeinfo = localtime ( &rawtime );
				printf("%s        %s\n", *USERNAME, asctime(timeinfo));
			}
			else {
				UnleashExecute(args);
			}
		}
		else {
			float r = RandomFloat();
			if(r < 0.25) {
				printf("%s\n", "What are you trying to do with empty commands?");
			}
			else if(r < 0.5){
				printf("%s\n", "Are you serious?");
			}
			else if(r < 0.75){
				printf("%s\n", "Full of emptiness...");
			}
			else {
				printf("%s\n", "You gotta be kidding me.");
			}
		}

		printf("-----------------------------------------------------------------------------\n");

	} while (status);
}

int main(int argc, char **argv)
{
	// Load config files, if any.

	// Run command loop.
	Unleash();

	// Perform any shutdown/cleanup.
	return EXIT_SUCCESS;
}
