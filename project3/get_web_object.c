#include "get_web_object.h"

unsigned long domainToIp(char *domain) {
	
	int i;
	struct hostent *ip;
	void *address;
	
	printf("Evaluating Host...");
	ip = gethostbyname(domain);
	if(ip == NULL) {
		printf("Host name could not be found\n");
		return -1;
	}
	address = malloc(ip->h_length);
	memcpy(address, ip->h_addr_list[0], ip->h_length);	
	printf("Host name found\n");
	return *((unsigned long *) address);
}


int establishConnection(unsigned long *ip, int port) {
	struct sockaddr_in server;
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	printf("Creating socket...");
	if(sock == -1) {
		printf("Failed to create socket\n");
		return -1;
	}
	server.sin_addr.s_addr = *ip;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	printf("Binding to socket...");
	if(bind(sock, (struct sockaddr *) &server, sizeof(server)) == -1) {
		printf("Failed to bind to socket\n");
		return -1;
	}
	else {
		printf("Connection successful\n");
		return sock;
	}
}

int connectToServer(unsigned long *ip, int port) {
	struct sockaddr_in server;
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	printf("Creating socket...");
	if(sock == -1) {
		printf("Failed to create socket\n");
		return -1;
	}
	server.sin_addr.s_addr = *ip;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	printf("Connecting to socket...");
	if(connect(sock, (struct sockaddr *) &server, sizeof(server)) == -1) {
		printf("Failed to connect to socket\n");
		return -1;
	}
	else {
		printf("Connection successful\n");
		return sock;
	}
}
char *removeLine(char *request, char *section) {
	char *start, *end;
	start = strstr(request, section);
	if(start != NULL) {
		end = strchr(start, '\n');
		strcpy(start, end + 1);
	}
	return request;
	
}

char *sendRequest(int sock, char *message) {
	char c, *response;
	int n, i = 0, extra_mem = 1;

	printf("Sending request...");
	n = send(sock, message, strlen(message), 0);
	if(n < 0) {
		printf("Error writing to socket\n");
		return NULL;
	}
	printf("Request sent\n");
	
	response = (char *) malloc(INIT_RESP);
	n = recv(sock, response, INIT_RESP, 0);
	response[n] = '\0';
	
	response = removeLine(response, "If-Modified-Since");
	response = removeLine(response, "Proxy-Connection");
	response = removeLine(response, "Content-Length");
	response = removeLine(response, "ETag");
	response = removeLine(response, "Connection");
	response = removeLine(response, "Cache-Control");

	if(n < 0) {
		printf("Error reading from socket\n");
		return NULL;
	}

	printf("\n\nRead Completed.\n");
	
	return response;
}


char *parseRequest(char *request, char *section) {
	printf("Parsing request:\n");
	printf("%s\n", request);
			
	int section_length = strlen(section);
	char *start = strstr(request, section) + section_length + 2;
	if(start == NULL) 
		return "none";
	char *end = strchr(start, '\n') - 1;

	if(strcmp(section, "GET") == 0) {
		start = strstr(request, section) + section_length + 1;
		end = strchr(start, ' ');
	}
	int n_bytes = end - start;
	printf("Section is %d bytes long\n", n_bytes);
	char *value = malloc(n_bytes + 1);
	printf("Copying into new string...");
	strncpy(value, start, n_bytes);
	value[n_bytes] = '\0';
	printf("Copied value: \"%s\"\n", value);
	return value;
}

char *getRequest(int sock, int *client_sock, struct sockaddr_in *client) {
	char *response, *start, *end;
	int n, extra_mem = 0;
	int clientLen = sizeof(struct sockaddr_in);

	response = (char *) malloc(INIT_RESP);
	printf("Accepting incoming connections...");
	*client_sock = accept(sock, (struct sockaddr *) client, (socklen_t*)&clientLen);
	if(sock < 0) {
		printf("Accept failed\n");
		return NULL;
	}
	printf("Accepted\n");	
	n = recv(*client_sock, response, INIT_RESP, 0);
	response[n] = '\0';

	if(strlen(response) <= 0) {
		printf("Error reading from socket or no data read from socket\n");
		return NULL;
	}

	response = removeLine(response, "If-Modified-Since");
	response = removeLine(response, "Proxy-Connection");
	response = removeLine(response, "Content-Length");
	response = removeLine(response, "ETag");
	response = removeLine(response, "Connection");
	response = removeLine(response, "Cache-Control");
	printf("%d Bytes read\nResponse:\n%s\n", n, response);

	printf("\n\nRead Completed.\n");
	
	return response;
}
