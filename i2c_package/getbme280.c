/*
   Modified by Mehul Patel  
Author : Frank M.
reference : https://github.com/fm4dd/pi-bme280
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include "getbme280.h"

int verbose = 0;
int outflag = 0;
int argflag = 0; // 1=dump, 2=info, 3=reset, 4=data, 5=continuous
char osrs_mode[7] = {0};  // oversampling mode
char pwr_mode[7]  = {0};  // power mode
char iir_mode[4]  = {0};  // IIR filter mode
char stby_time[5] = {0};  // standby time
char senaddr[256] = BME280_ADDR;
char i2c_bus[256] = I2CBUS;
char htmfile[256] = {0};

int main() 
{    		  
	get_i2cbus(i2c_bus, senaddr);
	struct bmecal bmec;
	struct bmedata bmed;
	get_calib(&bmec);

	/* -------------------------------------------------------- *
	 * If power mode != NORMAL, set NORMAL for continuous reads *
	 * -------------------------------------------------------- */
	if(get_power() != 0x3) 
	{
		set_power(normal);
	}
	while(1)
	{
		//time_t tsnow = time(NULL);
		get_data(&bmec, &bmed);

		/* ----------------------------------------------------------- *
		 * print the formatted output string to stdout (Example below) *
		 * ----------------------------------------------------------- */
		printf(" Temp=%3.2f*C Humidity=%3.2f%% Pressure=%3.2fhPa\n",bmed.temp_c, bmed.humi_p, bmed.pres_p/100);

		sleep(1);
	}	 
}
