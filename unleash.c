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
	const char *HOSTNAME[150];
	char _hostname[150];
	const char *USERNAME[100];
	char command[20];
	int status = 1;

	gethostname(_hostname, 150);
	*HOSTNAME = _hostname;

	*USERNAME = GetUsername();

	do {
		printf("%s@%s:$ ", *USERNAME, *HOSTNAME);
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
