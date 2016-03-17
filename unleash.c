/* unleash.c */
#define _PROGRAM_NAME "unleash"
#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>

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

void unleash(void)
{
	const char *USERNAME[100];

	*USERNAME = GetUsername();

	char command[20];
	int status;
	status = 1;

	do {
		printf("%s$ ", *USERNAME);
		scanf("%s", command);
		if (strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0) {
			printf("%s\n", "You're a QUITTER!");
			return;
		}
	} while (status);
}

int main(int argc, char **argv)
{
	// Load config files, if any.

	// Run command loop.
	unleash();

	// Perform any shutdown/cleanup.
	return EXIT_SUCCESS;
}
