/*
*File: server.c
*
*Author: Balapranesh Elango
*
*References: 
*https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
*AESD Assignment Content
*/

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

int msgid;

// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[200];
} message;


/*
 * void serversend(int connfd)
 *
 * Function to send data to client
 *
 * Parameters:
 *		connfd - Clients fd
 *
 * Returns:
 *   		None
 */
void serversend(int cli_fd) {
	
	int n;
	// infinite loop to send data every 2 seconds to the client from the server
	while(1) {
	
	    	// msgrcv to receive message
   		 msgrcv(msgid, &message, sizeof(message), 1, 0);
   		 
   		 write(cli_fd, "\n", sizeof("\n") );
		 write(cli_fd, message.mesg_text, sizeof(message.mesg_text)); //Send data to client
		 sleep(6);
	}
}

/* Application entry point */
int main()
{
	int sockfd, connfd, len;
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

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening...\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("server accept failed...\n");
		exit(0);
	}
	
	printf("Accepted connection from %s", inet_ntoa(cli.sin_addr) );

	key_t key;
  
    	// ftok to generate unique key
    	key = ftok("progfile", 65);
  
 	   // msgget creates a message queue
    	// and returns identifier
    	msgid = msgget(key, 0666 | IPC_CREAT);
    
	sleep(1);
	serversend(connfd); //Send data from server to client


	close(sockfd);
}



