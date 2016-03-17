/* unleash.c */
#define _PROGRAM_NAME "unleash"
#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>
#include <unistd.h>

void unleash(void)
{
	register struct passwd *pw;
	register uid_t uid;
	const char *USERNAME[100];

	uid = geteuid ();
	pw = getpwuid (uid);
	if (pw)
	{
		*USERNAME = pw->pw_name;
	}
	char command[20];
	int status;
	status = 1;

	do {
		printf("%s$ ", *USERNAME);
		scanf("%s", command);
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
