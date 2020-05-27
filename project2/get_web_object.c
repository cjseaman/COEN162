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
	response[n + INIT_RESP * extra_mem] = '\0';

	if(n < 0) {
		printf("Error reading from socket\n");
		return NULL;
	}

	printf("\n\nRead Completed.\n");
	
	return response;
}

char *parseRequest(char *request) {
	
	int i, path_length, host_length;
	char *path_start, *path_end, *host_start, *host_end, *request_type, *host;

	//Skip path
	request = strchr(request, ' ') + 1;
	path_start = request;
	path_end = strchr(request, ' ');
	path_length = path_end - path_start;

	//Just get hostname
	host_start = strchr(request, '\n') + 1;
	host_start = strchr(host_start, ' ') + 1;
	host_end = strchr(host_start, '\n') - 2;
	host_length = host_end - host_start + 1;
	printf("%d\n", host_length);
	host = malloc(host_length + 1);
	strncpy(host, host_start, host_length);
	host[host_length] = '\0';
	return host;
}

char *getRequest(int sock, int *client_sock, struct sockaddr_in *client) {
	char *response;
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
	printf("%d Bytes read\nResponse:\n%s\n", n, response);

	if(strlen(response) <= 0) {
		printf("Error reading from socket or no data read from socket\n");
		return NULL;
	}

	printf("\n\nRead Completed.\n");
	
	return response;
}
