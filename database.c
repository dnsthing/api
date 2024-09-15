#include "util.h"

void createDatabase(mongoc_client_t *client) {
	mongoc_collection_t *collection;
	bson_error_t error;
	bson_t *doc;
	bson_t *filter;

	collection = mongoc_client_get_collection(client, DATABASE_NAME, COLLECTION_NAME);
	if (!collection) {
		fprintf(stderr, "Failed to get collection\n");
		return;
	}

	doc = bson_new();
	filter = bson_new();

	bson_append_utf8(doc, "info", -1, "This document is to ensure the collection exists", -1);

	if (mongoc_collection_insert_one(collection, doc, NULL, NULL, &error)) {
		printf("Document inserted successfully.\n");

		bson_append_utf8(filter, "info", -1, "This document is to ensure the collection exists", -1);

		if (mongoc_collection_delete_one(collection, filter, NULL, NULL, &error)) {
			printf("Document deleted successfully.\n");
		} else {
			fprintf(stderr, "Failed to delete document: %s\n", error.message);
		}
	} else {
		fprintf(stderr, "Failed to insert document: %s\n", error.message);
	}

	bson_destroy(doc);
	bson_destroy(filter);
	mongoc_collection_destroy(collection);
}

char *listDatabaseEntries(mongoc_client_t *client) {
	mongoc_collection_t *collection;
	mongoc_cursor_t *cursor;
	const bson_t *doc;
	bson_error_t error;
	bson_t *bson_result = bson_new();
	collection = mongoc_client_get_collection(client, DATABASE_NAME, COLLECTION_NAME);
	cursor = mongoc_collection_find_with_opts(collection, bson_new(), NULL, NULL);
	while (mongoc_cursor_next(cursor, &doc)) {
		char *str = bson_as_canonical_extended_json(doc, NULL);
		bson_append_utf8(bson_result, "entry", -1, str, -1);
		bson_free(str);
	}
	if (mongoc_cursor_error(cursor, &error)) { 
		bson_destroy(bson_result);
		mongoc_cursor_destroy(cursor);
		mongoc_collection_destroy(collection);
		fprintf(stderr, "Cursor error: %s\n", error.message);
		return NULL;
	}
	char *json_string = bson_as_json(bson_result, NULL);
	bson_destroy(bson_result);
	mongoc_cursor_destroy(cursor);
	mongoc_collection_destroy(collection);
	return json_string;
}

void addDatabaseEntry(mongoc_client_t *client, const char *random_string, const char *adlist_url, const char *adlist_name) {
	mongoc_collection_t *collection;
	bson_error_t error;
	bson_t *doc;
	time_t now;
	char timestamp_str[21];
	collection = mongoc_client_get_collection(client, DATABASE_NAME, COLLECTION_NAME);
	now = time(NULL);
	snprintf(timestamp_str, sizeof(timestamp_str), "%ld", (long)now);
	doc = bson_new();
	bson_append_utf8(doc, "random_string", -1, random_string, -1);
	bson_append_utf8(doc, "adlist_url", -1, adlist_url, -1);
	bson_append_utf8(doc, "adlist_name", -1, adlist_name, -1);
	bson_append_utf8(doc, "creation_time", -1, timestamp_str, -1);
	if (!mongoc_collection_insert_one(collection, doc, NULL, NULL, &error)) { die("Failed to insert document: %s\n", error.message); }
	bson_destroy(doc);
	mongoc_collection_destroy(collection);
}

void deleteDatabaseEntry(mongoc_client_t *client, const char *random_string) {
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

void cleanDocuments(mongoc_client_t *client) {
	mongoc_collection_t *collection;
	bson_error_t error;
	bson_t *empty_filter;
	collection = mongoc_client_get_collection(client, DATABASE_NAME, COLLECTION_NAME);
	empty_filter = bson_new();
	if (mongoc_collection_delete_many(collection, empty_filter, NULL, NULL, &error)) { printf("All documents deleted successfully.\n");
	} else { die("Failed to delete documents: %s\n", error.message); }
	bson_destroy(empty_filter);
	mongoc_collection_destroy(collection);
}

void cleanCollection(mongoc_client_t *client) {
	mongoc_collection_t *collection;
	bson_error_t error;
	collection = mongoc_client_get_collection(client, DATABASE_NAME, COLLECTION_NAME);
	if (mongoc_collection_drop(collection, &error)) { printf("Collection dropped successfully.\n");
	} else { die("Failed to drop collection: %s\n", error.message); }
	mongoc_collection_destroy(collection);
}

void viewDaemon(int argc, char *argv[]) {
	mongoc_client_t *client;
	mongoc_uri_t *uri;
	
	mongoc_init();

	uri = mongoc_uri_new_with_error("mongodb://localhost:27017", NULL);
	client = mongoc_client_new_from_uri(uri);

	if (argc >= 2 && (strcmp(argv[1], "clean") == 0)) {
		cleanDocuments(client);
		cleanCollection(client);

		mongoc_client_destroy(client);
		mongoc_uri_destroy(uri);
		mongoc_cleanup();
		die("Database has been cleaned.");
	}

	createDatabase(client);
	listDatabaseEntries(client);

	mongoc_client_destroy(client);
	mongoc_uri_destroy(uri);
	mongoc_cleanup();
}
