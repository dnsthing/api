#define BUFFER_SIZE 1024

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
#include <curl/curl.h>

int main(int argc, char *argv[]);
size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp);
void dbCleanUp(mongoc_database_t *database, bson_t *command, mongoc_server_api_t *api, mongoc_client_t *client);
void die(const char *fmt, ...);
void downloadFile(const char *url, const char *filename);
void generateDnsmasqViaBlocklist(const char *blocklist_file, const char *dnsmasq_file);
void generateDnsmasqViaHosts(const char *hosts_file, const char *dnsmasq_file);
void showHelp();
void viewDaemon();
