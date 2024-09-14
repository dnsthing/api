
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

void cleanup(mongoc_database_t *database, bson_t *command, mongoc_server_api_t *api, mongoc_client_t *client) {
	bson_destroy(command);
	mongoc_database_destroy(database);
	mongoc_server_api_destroy(api);
	mongoc_client_destroy(client);
	mongoc_cleanup();
}

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
void viewDaemon() {
	mongoc_client_t *client = NULL;
	bson_error_t error = {0};
	mongoc_server_api_t *api = NULL;
	mongoc_database_t *database = NULL;
	bson_t *command = NULL;
	bson_t reply = BSON_INITIALIZER;
	bool ok = true;
	
	// Initialize the MongoDB C Driver.
	mongoc_init();

	client = mongoc_client_new("mongodb://127.0.0.1:27017");
	if (!client) {
		cleanup(database, command, api, client);
		die("Failed to create a MongoDB client.\n");
	}

	// Set the version of the Stable API on the client.
	api = mongoc_server_api_new(MONGOC_SERVER_API_V1);
	if (!api) {
		cleanup(database, command, api, client);
		die("Failed to create a MongoDB server API.\n");
	}
	
	ok = mongoc_client_set_server_api(client, api, &error);
	if (!ok) {
		cleanup(database, command, api, client);
		die("error: %s\n", error.message);
	}

	// Get a handle on the "admin" database.
	database = mongoc_client_get_database(client, "admin");
	if (!database) {
		cleanup(database, command, api, client);
		die("Failed to get a MongoDB database handle.\n");
	}
	// Ping the database.
	command = BCON_NEW("ping", BCON_INT32(1));
	ok = mongoc_database_command_simple(
		database, command, NULL, &reply, &error
	);
	if (!ok) {
		cleanup(database, command, api, client);
		die("error: %s\n", error.message);
	}
	bson_destroy(&reply);
	
	die("Pinged your deployment. You successfully connected to MongoDB!\n");
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
		"  daemon	- view the current stateus of the daemon\n"
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
		viewDaemon();	
	else if (argc != 1)
		die("usage");
}
