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

void downloadListsAndUpdate() {
	mongoc_client_t *client;
	const char *adlistEntries;
	json_object *json_obj, *json_entry_string, *json_entry;
	size_t i = 0;
	const char *random_string, *adlist_url, *adlist_name;
	char filename[100];
	char entry_key[10]; // To store keys like "entry_1", "entry_2", etc.
	mongoc_init();
	client = mongoc_client_new("mongodb://localhost:27017");
	if (!client) {
		fprintf(stderr, "Failed to create MongoDB client.\n");
		return;
	}
	adlistEntries = listDatabaseEntries(client);

	// Parse the JSON string (first layer)
	json_obj = json_tokener_parse(adlistEntries);

	// Check if it's a valid JSON object
	if (json_obj == NULL) {
		fprintf(stderr, "Failed to parse JSON: %s\n", adlistEntries);
		return;
	}

	// Loop over each "entry" in the object
	while (1) {
		snprintf(entry_key, sizeof(entry_key), "entry_%zu", i);  // Generate keys like "entry_0", "entry_1", etc.

		// Try to get the entry string by key
		if (!json_object_object_get_ex(json_obj, "entry", &json_entry_string)) {
			// If no more entries, break the loop
			break;
		}

		// The "entry" is a string, so we need to parse it as a JSON object
		json_entry = json_tokener_parse(json_object_get_string(json_entry_string));
		if (json_entry == NULL) {
			fprintf(stderr, "Failed to parse the 'entry' string as JSON.\n");
			json_object_put(json_obj);  // Free the original JSON object
			return;
		}

		// Now process the parsed JSON object
		random_string = json_object_get_string(json_object_object_get(json_entry, "random_string"));
		adlist_url = json_object_get_string(json_object_object_get(json_entry, "adlist_url"));
		adlist_name = json_object_get_string(json_object_object_get(json_entry, "adlist_name"));

		// 2. Delete the database entry
		deleteDatabaseEntry(client, random_string);

		// 3. Create a filename in the format "random_string.conf"
		snprintf(filename, sizeof(filename), "%s.conf", random_string);

		// 4. Download the file from adlist_url and save it as random_string.conf
		printf("Downloading %s\n", filename);
		downloadFile(adlist_url, filename);
		printf("Success!");

		// 5. Re-add the entry to the database
		addDatabaseEntry(client, random_string, adlist_url, adlist_name);

		// Increment the index for the next iteration
		i++;
	}

	// Clean up
	json_object_put(json_obj);	// Free the outer JSON object
	mongoc_client_destroy(client);  // Destroy MongoDB client
	mongoc_cleanup();  // Cleanup MongoDB resources
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
		downloadListsAndUpdate();
	else if (argc >= 2 && strcmp(argv[1], "daemon") == 0)
		viewDaemon(argc - 1, argv + 1);
	else if (argc >= 2 && strcmp(argv[1], "adlist") == 0)
		manageAdlist(argc - 1, argv + 1);
}
