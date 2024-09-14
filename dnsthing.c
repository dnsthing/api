
/* standard gnu libraries */
#include <errno.h>
#include <locale.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* 3rd party libraries */
#include <mongoc/mongoc.h>
#include <bson/bson.h>

/* standard funcs */
void die(const char *fmt, ...)
{
        va_list args;

        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);

        if (fmt[0] && fmt[strlen(fmt)-1] == ':') {
                fputc(' ', stderr);
                perror(NULL);
        } else {
                fputc('\n', stderr);
        }

        exit(1);
}

/* primary funcs */
void startDaemon() {

}

void showHelp() {
	die(
			"Usage: dnsthing [options]\n"
			"Example: dnsthing daemon start\n\n"
			"  uninstall	- uninstall dnsthing from the current machine\n"
			"  status	- check the current status of the daemon\n"
			"  adlist	- manage currently used adlists\n"
			"  domain	- manage currently blocked domains (seperate from the adlist)\n"
			"  exceptions	- manage exceptions to domains posed from the adlist\n"
			"  daemon	- manage the current stateus of the daemon\n"
			);
}

int main(int argc, char *argv[])
{
	/* extra arguments/settings */
	if (argc >= 2 && (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0))
		die("dnsthing-%s", VER);
	else if (argc >= 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0))
		showHelp();
	else if (argc >= 2 && strcmp(argv[1], "daemon") == 0)
		startDaemon();	
	else if (argc != 1)
		die("usage");
}
