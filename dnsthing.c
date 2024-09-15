#include "util.h"

void showHelp() {
	die(
		"Usage: dnsthing [options]\n"
		"Example: dnsthing daemon start\n\n"
		"  install	- install dnsthing to the current machine\n"
		"  uninstall	- uninstall dnsthing from the current machine\n"
		"  status	- check the current status of the daemon\n"
		"  adlist	- manage currently used adlists\n"
		"  domain	- manage currently blocked domains (seperate from the adlist)\n"
		"  exceptions	- manage exceptions to domains posed from the adlist\n"
		"  daemon	- view the current stateus of the daemon\n"
	);
}
void addAdlist() {
	die("bruh");
}

void delAdlist() {
	die("bruh2");
}

void viewAdlist() {
	die("bruh3");
}

void manageAdlist(int argc, char *argv[]) {
	if (argc >= 2 && (strcmp(argv[1], "add") == 0))
		addAdlist();
	else if (argc >= 2 && (strcmp(argv[1], "del")) == 0)
		delAdlist();
	else if (argc >= 2 && (strcmp(argv[1], "list")) == 0)
		viewAdlist();
	else if (argc == 1 || argc >= 2 ) {
		die(
			"Usage: dnsthing adlist [options]\n"
			"Example: dnsthing adlist add https://foobar/\n"
			"  add		- add an adlist to the database\n"
			"  del		- delete an adlist from the database\n"
			"  list		- list current adlists on the system"
		);
	}
}

int main(int argc, char *argv[])
{
	/* extra arguments/settings */
	if (argc >= 2 && (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0))
		die("dnsthing-%s", VER); /* ver is actually defined in the makefile. ignore whatever error is being told here */
	else if (argc >= 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0))
		showHelp();
	else if (argc >= 2 && strcmp(argv[1], "daemon") == 0)
		viewDaemon();
	else if (argc >= 2 && strcmp(argv[1], "adlist") == 0)
		manageAdlist(argc - 1, argv + 1);
	else if (argc == 1)
		die("usage");
}
