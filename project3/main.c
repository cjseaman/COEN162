#include "get_web_object.h"

int main(int argc, char *argv[]) {

	char **filename_pointer, *filename, **host_pointer, *host, *message, *request, *response, *content;
	unsigned long ip, local_host;
	int server_sock, browser_sock, client_sock, port = 6666;
	struct sockaddr_in client;

	filename_pointer = &filename;
	host_pointer = &host;

	if(argc > 1)
		port = atoi(argv[1]);
	
	local_host = inet_addr("127.0.0.1");	
	browser_sock = establishConnection(&local_host, port);
	if(browser_sock == -1) {
		printf("Bind to browser (port %d) failed\n", port);
		return 1;
	}

	listen(browser_sock, 1);
	request = getRequest(browser_sock, &client_sock, &client);
	if(request == NULL) {
		printf("Failed to get request from browser on socket %d\n", browser_sock);
		return 1;
	}

	printf("Parsing request:\n\n%s\n", request);
	host = parseRequest(request);

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

	response = sendRequest(server_sock, request);
	printf("Response:\n\n%s\n", response);
	printf("Sending response to browser...");
	//content = strstr(response, "\r\n\r\n");
	
	if(content != NULL) {
		content += 4;
	} else {
		content = response;
	}

	send(client_sock, content, INIT_RESP, 0);

	printf("Done\n");
	close(client_sock);
	close(server_sock);
	close(browser_sock);
	return 0;
	
}
