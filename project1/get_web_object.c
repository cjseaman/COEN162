#include "get_web_object.h"

unsigned long domainToIp(char *domain) {
	
	bool is_ip = true;
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


int establishConnection(unsigned long *ip) {
	struct sockaddr_in server;
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	printf("Creating socket...");
	if(sock == -1) {
		printf("Failed to create socket\n");
		return -1;
	}
	server.sin_addr.s_addr = *ip;
	server.sin_family = AF_INET;
	server.sin_port = htons(80);
	printf("Connecting to socket...");
	if(connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
		printf("Failed to connect to socket\n");
		return -1;
	}
	else {
		printf("Connection successful\n");
		return sock;
	}
}

void getFile(int sock, char *filename, char *hostname) {
	char message[100] = {0};
	char response[512] = {'\0'};
	char c;
	int n, i = 0;

	n = sprintf(message, "GET %s HTTP/1.1\r\nHost: %s\r\nContent-Type: text/plain\r\n\r\n", filename, hostname);	
	printf("Sending request...");
	n = send(sock, message, strlen(message), 0);
	if(n < 0) {
		printf("Error writing to socket\n");
		return;
	}
	printf("Request sent\n");
	printf("Reading response:\n\n");
	while(n = recv(sock, &c, 1, 0) > 0) {	
		putchar(c);
		if(c == EOF)
			break;
		//printf("%s", response);
	}

	if(n < 0) {
		printf("Error reading from socket\n");
		return;
	}

	printf("\n\nRead Completed.\n");

}
