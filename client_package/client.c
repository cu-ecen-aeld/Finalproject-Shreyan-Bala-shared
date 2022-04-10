/*#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <syslog.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr
void func(int sockfd)
{
	char buff[MAX];
	int n;
	for (;;) {
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
		printf("\nFrom Server : %s\n", buff);
	}
}

int main (int argc, char *argv[])
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));
	char server_ipaddress[20] = {0};
	memcpy(server_ipaddress, argv[1], strlen(argv[1]));
	
	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(server_ipaddress);
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	// function for chat
	func(sockfd);

	// close the socket
	close(sockfd);
}

*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <syslog.h>

#define IP_ADDRESS_SIZE 20

#define PORT 8080


int main (int argc, char *argv[])
{
	int socketfd = 0;
	int socketconnectfd = 0;
	//int bytes_read = 0;
	char datafromserver[1000];
	openlog("Socket Application client",LOG_PID,LOG_USER);
	printf("Welcome to Socket Application - CLIENT\n");
	// TODO: Check the actual socket size
	char server_ipaddress[IP_ADDRESS_SIZE] = {0};
	memcpy(server_ipaddress, argv[1], strlen(argv[1]));
	

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);
	syslog(LOG_DEBUG, "Client started");
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketfd < 0)
	{
		syslog(LOG_DEBUG, "ERROR: In creating Socket file descriptor");
		exit(0);
	}

	if((inet_pton(AF_INET, server_ipaddress, &server_address.sin_addr)) < 0)
	{
		syslog(LOG_DEBUG, "ERROR: In converting IPV4 from text to binary form");
		exit(0);
	}
	
	socketconnectfd =  connect(socketfd, (struct sockaddr *)&server_address,sizeof(server_address));
	if(socketconnectfd < 0)
	{
		syslog(LOG_DEBUG, "ERROR: In connecting to Server");
		exit(0);
	}
	
	syslog(LOG_DEBUG, "connected to Server");
	printf("connected to Server\n");
	while (1)
	{
	       read(socketfd,datafromserver,sizeof(datafromserver));	
	       printf("Data Read from server is %s\n",datafromserver);
	
	
	}
	
	
	

	


}

