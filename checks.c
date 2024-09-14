#include "util.h"

// Callback function to write data to a file
size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
	FILE *fp = (FILE *)userp;
	size_t written = fwrite(contents, size, nmemb, fp);
	return written;
}

// Function to download a file from a URL and save it locally
void downloadFile(const char *url, const char *filename) {
	CURL *curl;
	CURLcode res;
	FILE *fp;
	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	if (!curl) {
		curl_global_cleanup();
		die("Failed to initialize curl\n");
	}
	fp = fopen(filename, "wb");
	if (!fp) {
		perror("fopen");
		curl_easy_cleanup(curl);
		curl_global_cleanup();
		die("Failed to open file.");
	}
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
	res = curl_easy_perform(curl);
	if(res != CURLE_OK) {
		fclose(fp);
		curl_easy_cleanup(curl);
		curl_global_cleanup();
		die("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
	}
	fclose(fp);
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	return;
}

void generateDnsmasqViaHosts(const char *hosts_file, const char *dnsmasq_file) {
	FILE *hosts_fp = fopen(hosts_file, "r");
	FILE *dnsmasq_fp = fopen(dnsmasq_file, "w");
	if (!hosts_fp) { die("Error opening hosts file"); }
	if (!dnsmasq_fp) { fclose(hosts_fp); die("Error opening dnsmasq file"); }
	char line[256];
	while (fgets(line, sizeof(line), hosts_fp)) {
		if (line[0] == '#' || line[0] == '\n') { continue; }
		char *ip_address = strtok(line, " \t");
		char *hostname = strtok(NULL, " \t\n");
		if (ip_address && hostname) { fprintf(dnsmasq_fp, "address=/%s/%s\n", hostname, ip_address); }
	}
	fclose(hosts_fp);
	fclose(dnsmasq_fp);	
}

void generateDnsmasqViaBlocklist(const char *blocklist_file, const char *dnsmasq_file) {
	FILE *blocklist_fp = fopen(blocklist_file, "r");
	FILE *dnsmasq_fp = fopen(dnsmasq_file, "w");
	if (!blocklist_fp) { die("Error opening blocklist file"); }
	if (!dnsmasq_fp) { fclose(blocklist_fp); die("Error opening dnsmasq file"); }
	char line[256];
	while (fgets(line, sizeof(line), blocklist_fp)) {
		if (line[0] == '\n') { continue; }
		line[strcspn(line, "\n")] = '\0';
		char *domain = line;
		if (line[0] == '|') { domain++; }
		if (domain[strlen(domain) - 1] == '^') { domain[strlen(domain) - 1] = '\0'; }
		fprintf(dnsmasq_fp, "address=/%s/0.0.0.0\n", domain);
	}
	fclose(blocklist_fp);
	fclose(dnsmasq_fp);
}

