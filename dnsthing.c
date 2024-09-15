#include "util.h"

void showHelp() {
	die(
		"Usage: dnsthing [options]\n"
		"Example: dnsthing daemon start\n\n"
		"  version	- print the version of this software\n"
		"  help		- view this menu\n"
		"  adlist	- manage currently used adlists\n"
		"  daemon	- view the current stateus of the daemon\n"
	);
}

void addAdlist(int argc, char *argv[]) {
	
}

void delAdlist(int argc, char *argv[]) {

}

void viewAdlist() {
	
}

void manageAdlist(int argc, char *argv[]) { /* welcome to "else if" hell */
	mongoc_client_t *client;
	mongoc_uri_t *uri;

	if (argc >= 2 && (strcmp(argv[1], "add") == 0))
		addAdlist(argc - 1, argv + 1);
	else if (argc >= 2 && (strcmp(argv[1], "del")) == 0)
		delAdlist(argc - 1, argv + 1);
	else if (argc >= 2 && (strcmp(argv[1], "list")) == 0)
		viewAdlist();
	else if (argc == 1 || argc >= 2 ) {
		die(
			"Usage: dnsthing adlist [options]\n"
			"Example: dnsthing adlist add https://foobar/\n\n"
			"  add		- add an adlist to the database\n"
			"  del		- delete an adlist from the database\n"
			"  list		- list current adlists on the system\n"
		);
	}
}

int main(int argc, char *argv[]) {
	/* extra arguments/settings */
	if (argc >= 2 && (strcmp(argv[1], "version") == 0))
		die("dnsthing-%s", VER); /* ver is actually defined in the makefile. ignore whatever error is being told here */
	else if (argc == 1 || argc >= 2 && (strcmp(argv[1], "help") == 0))
		showHelp();
	else if (argc >= 2 && strcmp(argv[1], "daemon") == 0)
		viewDaemon();
	else if (argc >= 2 && strcmp(argv[1], "adlist") == 0)
		manageAdlist(argc - 1, argv + 1);
}
