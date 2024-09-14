#include "util.h"

void modify_line(char *line, const char *search, const char *replacement) {
	char *pos;
	if ((pos = strstr(line, search)) != NULL) {
		// Replace the search string with the replacement string
		int len_search = strlen(search);
		int len_replacement = strlen(replacement);
		memmove(pos + len_replacement, pos + len_search, strlen(pos + len_search) + 1);
		memcpy(pos, replacement, len_replacement);
	}
}

void generateDnsmasq() {
	FILE *input_file = fopen("dnsmasq.conf", "r");
	FILE *output_file = fopen("dnsmasq.conf.tmp", "w");
	
	if (input_file == NULL || output_file == NULL) {
		die("Error opening file");
	}
	
	char buffer[BUFFER_SIZE];
	while (fgets(buffer, BUFFER_SIZE, input_file)) {
		// Modify lines containing "server="
		modify_line(buffer, "server=8.8.8.8", "server=1.1.1.1"); // Example replacement
		fputs(buffer, output_file);
	}

	fclose(input_file);
	fclose(output_file);

	// Replace the original file with the modified file
	if (remove("dnsmasq.conf") != 0) {
		die("Error deleting the original file");
	}
	if (rename("dnsmasq.conf.tmp", "dnsmasq.conf") != 0) {
		die("Error renaming the temporary file");
	}

	die("Configuration file updated successfully.\n");
}
