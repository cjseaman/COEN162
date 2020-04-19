#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>
#include <netdb.h>

//convert domain name to unsigned long ip value
//Could be ip address or domain name

unsigned long domainToIp(char *domain);

//initializes server values and tries to connect
//Returns socket identifier on sucessful connection
//Returns -1 on failure

int establishConnection(unsigned long *ip);

//Sends an HTTP GET request to the specified socket at the specified domain for the specified file
//Prints the response on stdout

void getFile(int sock, char *filename, char *domain);


