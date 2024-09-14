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

void generateDnsmasq();
void modify_line(char *line, const char *search, const char *replacement);
void viewDaemon();
void dbCleanUp(mongoc_database_t *database, bson_t *command, mongoc_server_api_t *api, mongoc_client_t *client);
int main(int argc, char *argv[]);
void showHelp();
void die(const char *fmt, ...);
