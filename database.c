#include "util.h"

void dbCleanUp(mongoc_database_t *database, bson_t *command, mongoc_server_api_t *api, mongoc_client_t *client) {
	bson_destroy(command);
	mongoc_database_destroy(database);
	mongoc_server_api_destroy(api);
	mongoc_client_destroy(client);
	mongoc_cleanup();
}

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
		dbCleanUp(database, command, api, client);
		die("Failed to create a MongoDB client.\n");
	}

	// Set the version of the Stable API on the client.
	api = mongoc_server_api_new(MONGOC_SERVER_API_V1);
	if (!api) {
		dbCleanUp(database, command, api, client);
		die("Failed to create a MongoDB server API.\n");
	}
	
	ok = mongoc_client_set_server_api(client, api, &error);
	if (!ok) {
		dbCleanUp(database, command, api, client);
		die("error: %s\n", error.message);
	}

	// Get a handle on the "admin" database.
	database = mongoc_client_get_database(client, "admin");
	if (!database) {
		dbCleanUp(database, command, api, client);
		die("Failed to get a MongoDB database handle.\n");
	}
	// Ping the database.
	command = BCON_NEW("ping", BCON_INT32(1));
	ok = mongoc_database_command_simple(
		database, command, NULL, &reply, &error
	);
	if (!ok) {
		dbCleanUp(database, command, api, client);
		die("error: %s\n", error.message);
	}
	bson_destroy(&reply);
	
	die("Pinged your deployment. You successfully connected to MongoDB!\n");
}
