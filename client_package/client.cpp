/*
*@Name: aesdsocket.c
*@Author: Shreyan Prabhu D and Balapranesh Elango
*@Brief: Source file to start aesd application which can send and receive pockets
*@Reference:https://beej.us/guide/bgnet/html/#getaddrinfoprepare-to-launch
*		  : Linux System Programming Robert Love
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
#include <gpiod.h>
#include "ssd1306.h"

#define IP_ADDRESS_SIZE 20

#define HALF_SECOND 500000
#define ERROR -1
#define SUCCESS 1
#define PORT 8080
#define GREENLED_GPIO_PIN    5
#define GREENLED_ON	      1

int green_led_init();
void blink_green_led();
void extractSensorValues(char datafromserver[]);
struct gpiod_chip *gpio_fd;
struct gpiod_line *gpio_green_line;
int green_led_status = 0;
char datafromserver[1000];
SSD1306 myDisplay;

/*
*@Function: main
*@brief: consists of Socket initialization, blinking led and oled display functions
*@argument: argc- number of command line arguments,  argv -command line arguments stored in an array
*@Return: 0 for success and 1 for failure
*/
int main (int argc, char *argv[])
{

/*Initializing OLED display*/

	myDisplay.initDisplay();
	myDisplay.clearDisplay();
	myDisplay.setWordWrap(TRUE);
	myDisplay.setDisplayMode(SSD1306::Mode::SCROLL);

	
	green_led_status = green_led_init();
	if (green_led_status == -1)
	{
	    printf("ERROR: Initializing Green Led");
	    syslog(LOG_DEBUG, "ERROR: Initializing Green Led");
	}

/* Client code */
	int socketfd = 0;
	int socketconnectfd = 0;
	openlog("Socket Application client",LOG_PID,LOG_USER);
	printf("Welcome to Socket Application - CLIENT\n");
	char server_ipaddress[IP_ADDRESS_SIZE] = {0};
	memcpy(server_ipaddress, argv[1], strlen(argv[1]));
	/*Socket address structure values*/
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
	       extractSensorValues(datafromserver);
               blink_green_led();


	}
	return 0;
}

/*
*@Function: ExtractSensorValues
*@brief: Convert raw string to useful data
*@argument: data from the Server string
*@Return: None
*/
void extractSensorValues(char datafromserver[])
{
	char buff[50];
	myDisplay.clearDisplay();
	int roll_value = 0;
	int temperature_value = 0;
	int pressure_value=0;
	char dest_str[10] = {0};
	char temp_str[10] = {0};
	char detect_str[50] = {0};
	strncpy(detect_str,datafromserver,strlen(datafromserver));

		strncpy(temp_str,detect_str,7);
		strncpy(dest_str,temp_str+4,3);
		roll_value = atoi(dest_str);
		strncpy(temp_str,"",strlen(temp_str));
		printf("Roll value: %d\n",(roll_value - 100)); 
			
		strncpy(temp_str,detect_str+8,7);
		strncpy(dest_str,temp_str+4,3);
		temperature_value = atoi(dest_str);
		strncpy(temp_str,"",strlen(temp_str));
		printf("Temperature value: %d\n",(temperature_value-100)); 
	
		strncpy(temp_str,detect_str+16,7);
		strncpy(dest_str,temp_str+4,3);
		pressure_value = atoi(dest_str);
		strncpy(temp_str,"",strlen(temp_str));
		printf("Pressure value: %d\n",(pressure_value-100)); 
		
		myDisplay.textDisplay("*****Vehicle Status*****");
		myDisplay.textDisplay("************************");
    		snprintf(buff, sizeof(buff), "Roll= %d", (roll_value-100));
		myDisplay.textDisplay(buff);
    		snprintf(buff, sizeof(buff), "Temperature= %d", (temperature_value-100));
		myDisplay.textDisplay(buff);
    		snprintf(buff, sizeof(buff), "Tyre Pressure= %d", (pressure_value-100));
		myDisplay.textDisplay(buff);
}

/*
*@Function: Initializing green led
*@argument: None
*@Return: sucess if initialization is complete and 0 if failure
*/
int green_led_init()
{
	int status = SUCCESS;
	gpio_fd = gpiod_chip_open("/dev/gpiochip0");
	if(!gpio_fd)
	{
		return ERROR; 
	}

	gpio_green_line= gpiod_chip_get_line(gpio_fd,GREENLED_GPIO_PIN );
	if(!gpio_green_line)
	{
		gpiod_chip_close(gpio_fd);
		return ERROR; 
	}
	green_led_status = 0;
	status = gpiod_line_request_output(gpio_green_line, "foobar", green_led_status); 
	if(status)
	{
		 return ERROR;
		 gpiod_chip_close(gpio_fd);  
	}

	printf("Green Led Initialized\n");
	return status;

}

/*
*@Function:  Blinking gree led to indicate socket communication is established
*@argument: None
*@Return: None
*/

void blink_green_led()
{
	
	green_led_status ^= 1;
	gpiod_line_set_value(gpio_green_line, green_led_status);
	printf("Green_led_status = %d\n",green_led_status);
	usleep(HALF_SECOND);
	green_led_status ^= 1;
	gpiod_line_set_value(gpio_green_line, green_led_status);
	printf("Green_led_status = %d\n",green_led_status);
	

}
