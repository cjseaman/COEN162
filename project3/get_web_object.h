#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>

#define INIT_RESP 10000

//convert domain name to unsigned long ip value
//Could be ip address or domain name
unsigned long domainToIp(char *domain);


//initializes server values and tries to connect
//takes in the ip of the server and the port number to connect to
//Returns socket identifier on sucessful connection
//Returns -1 on failure
int establishConnection(unsigned long *ip, int port);


//Just like establishConnection() but uses connect() rather than bind()
int connectToServer(unsigned long *ip, int port);


//Sends an HTTP GET request pointed to by message to the passed sock 
//Prints the response on stdout
char *sendRequest(int sock, char *message);


//Parses the filename and host information from an HTTP GET request
//takes the request, and two pointers where the filename and host will be stored after parsing.
//points to the char pointer that is created by malloc
char *parseRequest(char *request);


//Waits and listens for a request from the passed sock and then returns a pointer to the request
//client_sock and client are assigned by getRequest
//Returns null on failure
char *getRequest(int sock, int *client_sock, struct sockaddr_in *client);
