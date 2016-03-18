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
#define MAX_LEN 128

char *reserved_cmds[] = {
	"cd",
	"help",
	"exit"
};

int UnleashReservedCmdCounter() {
	return sizeof(reserved_cmds) / sizeof(char *);
}

const char * GetUsername()
{
	register struct passwd *pw;
	register uid_t uid;
	char *USERNAME[100];

	uid = geteuid ();
	pw = getpwuid (uid);
	if (pw)
	{
		*USERNAME = pw->pw_name;
		return *USERNAME;
	}
}

const char * GetHomeDir()
{
	struct passwd *pw = getpwuid(getuid());
	const char *homedir = pw->pw_dir;
	return homedir;
}

void print_image(FILE *fptr);

void print_image(FILE *fptr)
{
	char read_string[MAX_LEN];

	while(fgets(read_string,sizeof(read_string),fptr) != NULL)
		printf("%s",read_string);
}

int UnleashHelp()
{
	char *filename = "ascii-art.txt";
	FILE *fptr = NULL;

	if((fptr = fopen(filename,"r")) == NULL)
	{
		fprintf(stderr,"ASCII art loading failed!\n");
	} else {
		print_image(fptr);
		fclose(fptr);
	}

	int i;
	printf("--------------------------------------------------------------\n");
	printf("WELCOME TO UNLEASH SHELL\n");
	printf("Type program names and arguments, and hit enter.\n");
	printf("The following commands are reserved:\n");

	for (i = 0; i < UnleashReservedCmdCounter(); i++) {
		printf("  %s\n", reserved_cmds[i]);
	  }

	printf("Use the man command for information on other programs.\n");
	printf("--------------------------------------------------------------\n");
	return 1;
}

void Unleash(void)
{
	const char *HOSTNAME[150];
	char _hostname[150];
	const char *USERNAME[100];
	const char *HOMEDIR[100];
	char cwd[1024];
	char command[20];
	int status = 1;
	char outcwd[1024];

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

		printf("%s@%s:%s$ ", *USERNAME, *HOSTNAME, outcwd);
		scanf("%s", command);
		//printf("%s", command);

		if (strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0) {
			printf("%s\n", "You're a QUITTER!");
			return;
		}
		else if(strcmp(command, "help") == 0) {
			UnleashHelp();
		}

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
