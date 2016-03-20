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
#include <signal.h>
#include <errno.h>
#include "asciiart.c"
#include "who.c"
#define MAX_LEN 128
#define MAX_PID 64

pid_t pidStack[MAX_PID];
int pidStep = 0;
char pidCommands[64][1000];

char *reserved_cmds[] = {
	"cd      Changes current working directory    $ cd ../dir/",
	"help    Opens Help section",
	"who     Displays a list of users who are currently logged into the computer",
	"jobs    Lists currently alive processes with their states",
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

int UnleashChangeDir(char **args)
{
  if (args[1] == NULL) {
	fprintf(stderr, "Unleash: expected argument to \"cd\"\n");
  } else {
	if (chdir(args[1]) != 0) {
	  perror("Unleash");
	}
  }
  return 1;
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

void  SIGTSTPhandler(int sig)
{
	 kill(pidStack[pidStep-1], SIGSTOP);
	 printf("\nProcess with PID: %d stopped. It was created by this command: %s\n", pidStack[pidStep-1], pidCommands[pidStep-1]);
}

void  SIGINThandler(int sig)
{
	 kill(pidStack[pidStep-1], SIGTERM);
	 printf("\nProcess with PID: %d terminated. It was created by this command: %s\n", pidStack[pidStep-1], pidCommands[pidStep-1]);
	 pidStack[pidStep] = 0;
	 strcpy(pidCommands[pidStep], "");
	 pidStep--;
}

void UnleashJobs(void)
{
	if (pidStep > 0) {
		printf("%16s%16s%16s\n", "PID", "STATUS", "COMMAND");
		int n;
		int status;
		char status_str[20];
		for (n = 0; pidStep > n; n++)
		{
			if (kill(pidStack[n], 0) == 0) {
				strcpy(status_str, "stopped");
			} else if (errno == ESRCH) {
				strcpy(status_str, "running");
			} else {
				strcpy(status_str, "something very bad happened to this process");
			}
			printf("%16d%16s%16s\n", pidStack[n], status_str, pidCommands[n]);
		}
	} else {
		printf("No job. Maybe Steve job?\n");
	}
}

int UnleashExecute(char **args, char *command)
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
	pidStack[pidStep] = pid;
	strcpy(pidCommands[pidStep], command);
	pidStep++;
	do {
	  signal(SIGTSTP, SIGTSTPhandler);
	  signal(SIGINT, SIGINThandler);
	  waitpid(pid, &status, WUNTRACED);
	} while (!WIFEXITED(status) && !WIFSIGNALED(status) && !WIFSTOPPED(status));
	if (WIFEXITED(status)) {
		pidStack[pidStep] = 0;
		strcpy(pidCommands[pidStep], "");
		pidStep--;
	}
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
		char *args[1000] = {};
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

		printf("_____________________________________________________________________________\n\n");

		int ac = ArgumentExtractor(command, args);
		//printf("Length of array: %d\n", (int)( sizeof(&args) ));
		//puts(&args[1]);
		//printf("%s\n",args[ac-1]);

		if(ac > 0){
			if (strcmp(args[0], "exit") == 0 || strcmp(args[0], "quit") == 0) {
				printf("%s\n\n", "You're a QUITTER!");
				return;
			}
			else if(strcmp(args[0], "help") == 0) {
				UnleashHelp();
			}
			else if(strcmp(args[0], "who") == 0) {
				Who();
			}
			else if(strcmp(args[0], "cd") == 0) {
				UnleashChangeDir(args);
			}
			else if(strcmp(args[0], "jobs") == 0) {
				UnleashJobs();
			}
			else {
				UnleashExecute(args, command);
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

		//free(args);
		printf("_____________________________________________________________________________\n\n");

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
