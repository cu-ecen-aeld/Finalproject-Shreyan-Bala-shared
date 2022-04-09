

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
#define PORT 9000

int main (int argc, char *argv[])
{
	int socketfd = 0;
	int socketconnectfd = 0;
	//int bytes_read = 0;
	char datafromserver[1000];
	openlog("Socket Application client",LOG_PID,LOG_USER);
	printf("Welcome to Socket Application - CLIENT\n");
	// TODO: Check the actual socket size
	char server_ipaddress[IP_ADDRESS_SIZE];
	memcpy(server_ipaddress, argv[1], strlen(argv[1]));
	

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);
	
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketfd < 0)
	{
		syslog(LOG_DEBUG, "ERROR: In creating Socket file descriptor");
	}

	if((inet_pton(AF_INET, server_ipaddress, &server_address.sin_addr)) < 0)
	{
		syslog(LOG_DEBUG, "ERROR: In converting IPV4 from text to binary form");
	}
	
	socketconnectfd =  connect(socketfd, (struct sockaddr *)&server_address,sizeof(server_address));
	if(socketconnectfd < 0)
	{
		syslog(LOG_DEBUG, "ERROR: In connecting to Server");
	}
	
	syslog(LOG_DEBUG, "connected to Server");
	while (1)
	{
	       read(socketfd,datafromserver,sizeof(datafromserver));	
		printf("Data Read from server is %s\n",datafromserver);
	
	
	}
	
	
	

	


}
