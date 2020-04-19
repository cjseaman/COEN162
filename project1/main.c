#include "main.h"

int main (int argc, char* argv[]) {
	
	if (argc < 2) {
		printf("Need at least one argument!\n./get_web_object <domain or address of server> <file path (defaults to index.html)>\n");
		return 1;
	}

	char *filename;
	char name[] = "/index.html";
	char *domain = argv[1];
	if(argc > 2) 
		filename = argv[2];
	else
		filename = name;
	unsigned long ip;
	int sock;

	ip = domainToIp(domain);
	
	if(ip == (unsigned long) -1) {
		printf("Failed to evaluate server address\n");
		return 1;
	}	
	
	sock = establishConnection(&ip);
	
	if(sock == -1) {
		printf("Connection to server failed\n");
		return 1;
	}
	else {
		printf("Connected\n");
		getFile(sock, filename, domain);
		close(sock);
		return 0;
	}
	
}
