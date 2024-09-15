#include "util.h"

void showHelp() {
	die(
		"Usage: dnsthing [options]\n"
		"Example: dnsthing daemon\n\n"
		"  download	- download and setup the adlists\n"
		"  version	- print the version of this software\n"
		"  help		- view this menu\n"
		"  adlist	- manage currently used adlists\n"
		"  daemon	- view the current stateus of the daemon\n"
	);
}

void downloadLists() {
	mongoc_client_t *client;
	mongoc_uri_t *uri;
	mongoc_init();
	uri = mongoc_uri_new_with_error("mongodb://localhost:27017", NULL);
	client = mongoc_client_new_from_uri(uri);

	const char *jsonEntries = listDatabaseEntries(client);
        


	mongoc_client_destroy(client);
        mongoc_uri_destroy(uri);
        mongoc_cleanup();
}

void addAdlist(int argc, char *argv[], mongoc_client_t *client, mongoc_uri_t *uri) {
	if (argc == 1) {
		printf("Usage: dnsthing adlist add [URL] [NAME]\nExample: dnsthing adlist add https://foobar/ foobar\n");
	} else {
		srand((unsigned int)time(NULL));
		char randomString[16];
		generateRandomString(randomString, 15);
		addDatabaseEntry(client, randomString, argv[1], argv[2]);
	}
	mongoc_client_destroy(client);
	mongoc_uri_destroy(uri);
	mongoc_cleanup();
}

void delAdlist(int argc, char *argv[], mongoc_client_t *client, mongoc_uri_t *uri) {
	deleteDatabaseEntry(client, argv[1]); 
	mongoc_client_destroy(client);
	mongoc_uri_destroy(uri);
	mongoc_cleanup();
}

void viewAdlist(mongoc_client_t *client, mongoc_uri_t *uri) {
	printf("%s\n",listDatabaseEntries(client));	
	mongoc_client_destroy(client);
	mongoc_uri_destroy(uri);
	mongoc_cleanup();
}

void manageAdlist(int argc, char *argv[]) { /* welcome to "else if" hell */
	mongoc_client_t *client;
	mongoc_uri_t *uri;
	
	mongoc_init();

	uri = mongoc_uri_new_with_error("mongodb://localhost:27017", NULL);
	client = mongoc_client_new_from_uri(uri);

	if (argc >= 2 && (strcmp(argv[1], "add") == 0))
		addAdlist(argc - 1, argv + 1, client, uri);
	else if (argc >= 2 && (strcmp(argv[1], "del")) == 0)
		delAdlist(argc - 1, argv + 1, client, uri);
	else if (argc >= 2 && (strcmp(argv[1], "list")) == 0)
		viewAdlist(client, uri);
	else if (argc == 1 || argc >= 2 ) {
		mongoc_client_destroy(client);
		mongoc_uri_destroy(uri);
		mongoc_cleanup();
		die(
			"Usage: dnsthing adlist [options]\n"
			"Example: dnsthing adlist list\n\n"
			"  add	[URL] [NAME]	- add an adlist to the database\n"
			"  del  [KEY]		- delete an adlist from the database\n"
			"  list			- list current adlists on the system\n"
		);
	}
}

void main(int argc, char *argv[]) {
	/* extra arguments/settings */
	if (argc >= 2 && (strcmp(argv[1], "version") == 0))
		die("dnsthing-%s", VER); /* ver is actually defined in the makefile. ignore whatever error is being told here */
	else if (argc == 1 || argc >= 2 && (strcmp(argv[1], "help") == 0))
		showHelp();
	else if (argc >= 2 && strcmp(argv[1], "download") == 0)
		downloadLists();
	else if (argc >= 2 && strcmp(argv[1], "daemon") == 0)
		viewDaemon(argc - 1, argv + 1);
	else if (argc >= 2 && strcmp(argv[1], "adlist") == 0)
		manageAdlist(argc - 1, argv + 1);
}
