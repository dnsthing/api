#define BUFFER_SIZE 1024
#define DATABASE_NAME "dnsthing"
#define COLLECTION_NAME "adlistcollection"

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
#include <json-c/json.h>
#include <curl/curl.h>

size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp);

/* void functions */
void addAdlist(int argc, char *argv[], mongoc_client_t *client, mongoc_uri_t *uri);
void addDatabaseEntry(mongoc_client_t *client, const char *random_string, const char *adlist_url, const char *adlist_name);
void cleanCollection(mongoc_client_t *client);
void cleanDocuments(mongoc_client_t *client);
void createDatabase(mongoc_client_t *client);
void dbCleanUp(mongoc_database_t *database, bson_t *command, mongoc_server_api_t *api, mongoc_client_t *client);
void delAdlist(int argc, char *argv[], mongoc_client_t *client, mongoc_uri_t *uri);
void deleteDatabaseEntry(mongoc_client_t *client, const char *random_string);
void die(const char *fmt, ...);
void downloadFile(const char *url, const char *filename);
void generateDnsmasqViaBlocklist(const char *blocklist_file, const char *dnsmasq_file);
void generateDnsmasqViaHosts(const char *hosts_file, const char *dnsmasq_file);
void generateDnsmasqViaWildcard(const char *blocklist_file, const char *dnsmasq_file);
void generateRandomString(char *str, size_t length);
char *listDatabaseEntries(mongoc_client_t *client);
void main(int argc, char *argv[]);
void manageAdlist(int argc, char *argv[]);
void showHelp();
void viewAdlist(mongoc_client_t *client, mongoc_uri_t *uri);
void bruh();
void viewDaemon(int argc, char *argv[]);
