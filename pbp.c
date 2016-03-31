#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

int main (int argc, char* argv[]) {
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
		strcpy(url, "https://api.pbcp.io/board/");
	} else {
		fscanf(f, "%s", url);
		fclose(f);
	}

	if (argc == 2)
		sprintf(url, "%s%s/%s", url, id, argv[1]);
	else if (argc == 1)
		sprintf(url, "%s%s/%d", url, id, 0);
	else {
		fprintf(stderr, "Usage: pbp [index]\n");
		return -1;
	}

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
			fprintf(stderr, "Cannot perform request: %s\n", curl_easy_strerror(res));

	} else {
		fprintf(stderr, "Cannot obtain cURL handle!\n");
	}

	curl_easy_cleanup(curl);
	curl_global_cleanup();
	return 0;
}
