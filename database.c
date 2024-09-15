#include "util.h"

void createDatabase(mongoc_client_t *client) {
	mongoc_collection_t *collection;
	bson_error_t error;
	bson_t *doc;
	collection = mongoc_client_get_collection(client, DATABASE_NAME, COLLECTION_NAME);
	doc = bson_new();
	bson_append_utf8(doc, "info", -1, "This document is to ensure the collection exists", -1);
	if (!mongoc_collection_insert_one(collection, doc, NULL, NULL, &error)) { die("Failed to create collection: %s\n", error.message); }
	bson_destroy(doc);
	mongoc_collection_destroy(collection);
}

void add_entry(mongoc_client_t *client, const char *random_string, const char *adlist_url, const char *adlist_name) {
	mongoc_collection_t *collection;
	bson_error_t error;
	bson_t *doc;
	time_t now;
	struct tm *tm_info;
	char time_buffer[26];
	collection = mongoc_client_get_collection(client, DATABASE_NAME, COLLECTION_NAME);
	time(&now);
	tm_info = localtime(&now);
	strftime(time_buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
	doc = bson_new();
	bson_append_utf8(doc, "random_string", -1, random_string, -1);
	bson_append_utf8(doc, "adlist_url", -1, adlist_url, -1);
	bson_append_utf8(doc, "adlist_name", -1, adlist_name, -1);
	bson_append_utf8(doc, "creation_time", -1, time_buffer, -1);
	if (!mongoc_collection_insert_one(collection, doc, NULL, NULL, &error)) { die("Failed to insert document: %s\n", error.message); }
	bson_destroy(doc);
	mongoc_collection_destroy(collection);
}

void delete_entry(mongoc_client_t *client, const char *random_string) {
	mongoc_collection_t *collection;
	bson_error_t error;
	bson_t *filter;
	collection = mongoc_client_get_collection(client, DATABASE_NAME, COLLECTION_NAME);
	filter = bson_new();
	bson_append_utf8(filter, "random_string", -1, random_string, -1);
	if (!mongoc_collection_delete_one(collection, filter, NULL, NULL, &error)) { die("Failed to delete document: %s\n", error.message); }
	bson_destroy(filter);
	mongoc_collection_destroy(collection);
}


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
