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

	// Initialize libcurl
	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	if (!curl) {
		fprintf(stderr, "Failed to initialize curl\n");
		curl_global_cleanup();
		return;
	}

	// Open file to write
	fp = fopen(filename, "wb");
	if (!fp) {
		perror("fopen");
		curl_easy_cleanup(curl);
		curl_global_cleanup();
		return;
	}

	// Set URL
	curl_easy_setopt(curl, CURLOPT_URL, url);

	// Set write callback function
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);

	// Set file pointer to the callback function
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

	// Perform the request
	res = curl_easy_perform(curl);

	// Check for errors
	if(res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		fclose(fp);
		curl_easy_cleanup(curl);
		curl_global_cleanup();
		return;
	}

	// Close file
	fclose(fp);

	// Cleanup
	curl_easy_cleanup(curl);
	curl_global_cleanup();

	return;
}
