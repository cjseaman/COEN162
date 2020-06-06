#include "get_web_object.h"
	
struct Cache {
	char *document_path;
	char *document_name;
	char *host_name;
	struct Cache *next;
};

int main(int argc, char *argv[]) {

	char **filename_pointer, *filename, *filepath, **host_pointer, *host, *message, *request, *response, *content;
	unsigned long ip, local_host;
	int server_sock = -1, browser_sock = -1, client_sock, port = 6666, doc_num = 0, in_cache = 0;
	FILE *file;
	struct sockaddr_in client;
	struct Cache *home, *cache_list = NULL;

	filename_pointer = &filename;
	host_pointer = &host;

	response = (char *) malloc(INIT_RESP);
	content = (char *) malloc(INIT_RESP * 2);

	if(argc > 1)
		port = atoi(argv[1]);
	
	local_host = inet_addr("127.0.0.1");
	browser_sock = establishConnection(&local_host, port);
	if(browser_sock == -1) {
		printf("Bind to browser (port %d) failed\n", port);
		return 1;
	}

	listen(browser_sock, 1);

	while(1) {
		
			request = getRequest(browser_sock, &client_sock, &client);
			if(request == NULL) {
				printf("Failed to get request from browser on socket %d\n", browser_sock);
				return 1;
			}

			printf("Parsing request:\n\n%s\n", request);
			host = parseRequest(request, "Host");
			filename = parseRequest(request, "GET");
			
			printf("Converting ");
			printf("%s", host);
			printf(" to ip (strlen is %lu)...", strlen(host));

			ip = domainToIp(host);
			
			if(ip == (unsigned long) -1) {
				printf("Failed to evaluate server address\n");
				return 1;
			}	
			printf("Connecting to server...");
			server_sock = connectToServer(&ip, 80);	
			if(server_sock == -1) {
				printf("Connection to server failed\n");
				return 1;
			}
			printf("Connected\n");

			if(cache_list == NULL) {
				printf("Creating cache...");
				cache_list = (struct Cache *) malloc(sizeof(struct Cache));
				cache_list->document_path = "cache/document000";
				cache_list->document_name = filename;
				cache_list->host_name = host;
				home = cache_list;
				doc_num = 0;
				in_cache = 0;
				printf("Cache created.\n");	
			} else {
				in_cache = 0;
				cache_list = home;
				do {
					if(strcmp(cache_list->document_name, filename) == 0 && strcmp(cache_list->host_name, host) == 0) {
						//Serve document_name
						printf("File already in cache.\n");
						in_cache = 1;
						file = fopen(cache_list->document_path, "r");
						while(fgets(response, INIT_RESP, file)) {
							strcat(content, response);
						}
						fclose(file);
						send(client_sock, content, sizeof(content), 0);

						printf("Done\n");
						fclose(file);
						close(client_sock);
					}
					else {
						cache_list = cache_list->next;
						doc_num ++;
					}
				} while (cache_list->next != NULL);

			}
			if(in_cache == 0) {
				printf("Value not in cache\n");
				//Store in cache and send
				response = sendRequest(server_sock, request);
				printf("Response(main):\n\n%s\n", response);
				printf("Sending response to browser...\n");
				send(client_sock, response, sizeof(response), 0);
				printf("sent.\nAllocating cache list...");
				cache_list->next = (struct Cache *) malloc(sizeof(cache_list));
				printf("Allocated.\n");
				cache_list->document_name = filename;
				cache_list->host_name = host;
				filepath = (char *) malloc(sizeof("cache/document000"));
				sprintf(filepath, "cache/document%03d", doc_num);
				cache_list->document_path = filepath;
				file = fopen(filepath, "w+");
				printf("Opened %s\n", cache_list->document_path);
				fwrite(response, 1, strlen(response), file);
				fclose(file);
				printf("Saved.\n");
			}
		printf("Closing server connection...");			
		close(server_sock);
		printf("Closed.\n");
	}	
	close(browser_sock);
	browser_sock = -1;
	return 0;
	
}
