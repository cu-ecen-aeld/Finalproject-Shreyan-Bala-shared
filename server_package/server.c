#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
 #include <unistd.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

// Function designed for chat between client and server.
void func(int connfd)
{
	char buff[MAX] = {"hello from server"};
	int n;
	// infinite loop for chat
	for (;;) {
		bzero(buff, MAX);


		// and send that buffer to client
		write(connfd, buff, sizeof(buff));

		sleep(2);
	}
}

// Driver function
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
		printf("Server listening..\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("server accept failed...\n");
		exit(0);
	}
	else
		printf("server accept the client...\n");

	// Function for chatting between client and server
	func(connfd);

	// After chatting close the socket
	close(sockfd);
}


#undef de
#ifdef de
/*
*File: server.c
*
*Author: Balapranesh Elango
*
*References: 
*AESD Assignment Content
*/

/*Header files*/
#include<string.h>
#include<stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <syslog.h>
#include <dirent.h>
#include <libgen.h>
#include <pthread.h>
#include <sys/queue.h>
#include <sys/time.h>
#include <time.h>



/*Macros*/

#define MYPORT   "8080"

#define BACKLOG  (10)
#define FALSE    (0)
#define TRUE     (1)
#define log_message syslog

int sockfd, newsockfd;
int fd;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 
bool complete_flag = FALSE;
/*
 *Thread parameters
 */
typedef struct {
	bool thread_complete;
	int client_sock_fd;
	pthread_mutex_t *mutex;
	
	}thread_params_t;



/*
 * static void graceful_exit(int status)
 *
 * Used to cleanup and exit from the program
 *
 * Parameters:
 *   status - 0=> Successful exit
 *           -1=> Failure
 *
 * Returns:
 *   None
 */
static void graceful_exit(int status){


	complete_flag = TRUE;
	
	//Destroy mutex
    	pthread_mutex_destroy(&lock);
    	
	if(sockfd > -1){ //socket fd closed
		shutdown(sockfd, SHUT_RDWR);
		close(sockfd);
	}
	
	if(newsockfd > -1){//client socket fd closed
		shutdown(newsockfd, SHUT_RDWR);
		close(newsockfd);
	}

 	
	closelog(); 
	
	exit (status);
}




/*
 * Signal handler to catch and handle SIGINT and SIGTERM
 */
static void signal_handler(int signum){
	syslog(LOG_INFO, "Caught singal, exiting; Signal==>%d", signum);
	graceful_exit(0); 
}


/*
 * void *workerthread_socket(void *param)
 *
 * Thread to Receive content from client, write to file, and send back to client
 *
 * Parameters:
 *		Thread parameters
 *
 * Returns:
 *   		Thread_param to check if completed successfully
 */
static void *workerthread_socket(void *param){

	int rc;
	
	thread_params_t *thread_param = (thread_params_t *)param;
	 	
	while (complete_flag != TRUE) {
		rc = send(newsockfd, "hello", strlen("hello"), 0); //send to client
			
		if(rc == -1) {
			log_message(LOG_ERR, "ERROR: send() fail");
			graceful_exit(-1);
		}
	
	
	}
	
	close(newsockfd);
	
	thread_param->thread_complete = TRUE;
	return(thread_param);
}


/* Application entry point */
int main(int argc, char *argv[]) {

	int rc; 
	struct addrinfo hints;
	struct sockaddr_in cli_addr;
	socklen_t clilen;
	struct addrinfo *res, *ptr;
	
	/*Initializing the signal handlers for SIGINT and SIGTERM*/
	sig_t sig_rc;
	sig_rc = signal(SIGTERM, signal_handler);
	if(sig_rc == SIG_ERR){
		log_message(LOG_ERR, "ERROR: Sigterm signal() fail");
		graceful_exit(-1);
	}
	
	sig_rc = signal(SIGINT, signal_handler);
	if(sig_rc == SIG_ERR){
		log_message(LOG_ERR, "ERROR: sigint signal() fail");
		graceful_exit(-1);
	}
	
	
	openlog("aesdscoket.c - LOG", LOG_PID, LOG_USER); //Opening syslog for logging using LOG_USER facility
	
	//Populate the hints structure
	memset(&hints, 0, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	rc = getaddrinfo(NULL, MYPORT, &hints, &res);
	
	//getaddrinfo gets address that we need to bind to.
	if(rc){
		log_message(LOG_ERR, "ERROR: getaddrinfo() fail");
		graceful_exit(-1);
	}
	
	
	//Try recursively binding to the socket
	for (ptr = res; ptr != NULL; ptr = ptr->ai_next) {
	
		sockfd = socket(res->ai_family, res->ai_socktype, 0);
		if(sockfd == -1) {
			log_message(LOG_ERR, "ERROR: socket() fail"); //socket() failed
			graceful_exit(-1);
		}	
	
		rc = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) ;
	
		if (rc < 0) {
			log_message(LOG_ERR, "ERROR: sockopt() fail");
			graceful_exit(-1);
    		}
	
		rc = bind(sockfd, res->ai_addr, res->ai_addrlen);
		if (!rc) {
			log_message(LOG_INFO, "Successfully Bound");
			break;
		}
	}
	
	freeaddrinfo(res); //No longer required
	
	if(ptr == NULL) {
		log_message(LOG_ERR, "ERROR: bind() fail");
		graceful_exit(-1);		
	}
	
	log_message(LOG_INFO, "listen for connection");
	//listen for connection
	rc = listen(sockfd, BACKLOG);
	if(rc == -1){
		log_message(LOG_ERR, "ERROR: listen() fail");
		graceful_exit(-1);
	}
	
	log_message(LOG_INFO, "listen successfull");
	clilen = sizeof(cli_addr);

	pthread_mutex_init( &lock, NULL); //Initialize the mutex
	
	while (complete_flag != TRUE) {		
				
		pthread_t thread;
		thread_params_t thread_param;
		
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen); //accept the connection
    	        if (newsockfd < 0) {
      			log_message(LOG_ERR, "ERROR: accept() fail");
			graceful_exit(-1);
     		}
		
		thread_param.thread_complete = FALSE;
		thread_param.mutex = &lock;
		log_message(LOG_INFO, "Accepted connection from %s", inet_ntoa(cli_addr.sin_addr) );
		
		
		//Create thread for current node
		pthread_create(&thread, NULL, workerthread_socket, &(thread_param) );
		
		pthread_join(thread, NULL);
		
		
			
	}
	
	graceful_exit(0);
	
	
}
#endif
