#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

size_t wnull(char* ptr, size_t size, size_t nmemb, void* userdata) { return size * nmemb; }

int main () {
	CURL* curl;
	CURLcode res;

	char url[128];
	char id[128];
	char id_path[128];
	char server_path[128];

	FILE* f;

	strcpy(id_path, getenv("HOME"));
	strcat(id_path, "/.config/pbcp/id");
	f = fopen(id_path, "r");
	if (f == NULL) {
		fprintf(stderr, "Cannot open ~/.config/pbcp/id\n");
		return 1;
	}
	fscanf(f, "%s", id);
	fclose(f);

	strcpy(server_path, getenv("HOME"));
	strcat(server_path, "/.config/pbcp/server");
	f = fopen(server_path, "r");
	if (f == NULL) {
		strcpy(url, "http://23.227.187.40:9000/board/");
	} else {
		fscanf(f, "%s", url);
		fclose(f);
	}
	strcat(url, id);

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	struct curl_slist* headers = NULL;

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_READDATA, stdin);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, wnull);

		headers = curl_slist_append(headers, "Transfer-Encoding: chunked");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		res = curl_easy_perform(curl);
		curl_slist_free_all(headers);
		if (res != CURLE_OK)
			fprintf(stderr, "Cannot perform request: %s\n", curl_easy_strerror(res));

	} else {
		fprintf(stderr, "Cannot obtain cURL handle!\n");
	}

	curl_easy_cleanup(curl);
	curl_global_cleanup();
	return 0;
}
