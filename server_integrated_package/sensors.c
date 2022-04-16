//Reference: https://github.com/mindbeast/rpi-mpu6050/blob/master/rpi-mpu6050.c
//Modified by Balapranesh Elango

#include <stdio.h>
#include <linux/i2c-dev.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include "i2c_utils.h"
#include "bme280.h"
#include "compensation.h"
#include <math.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define MAX 10

#define MPU_ACCEL_XOUT1 0x3b
#define MPU_ACCEL_XOUT2 0x3c
#define MPU_ACCEL_YOUT1 0x3d
#define MPU_ACCEL_YOUT2 0x3e
#define MPU_ACCEL_ZOUT1 0x3f
#define MPU_ACCEL_ZOUT2 0x40

#define MPU_GYRO_XOUT1 0x43
#define MPU_GYRO_XOUT2 0x44
#define MPU_GYRO_YOUT1 0x45
#define MPU_GYRO_YOUT2 0x46
#define MPU_GYRO_ZOUT1 0x47
#define MPU_GYRO_ZOUT2 0x48

#define MPU_TEMP1 0x41
#define MPU_TEMP2 0x42

#define MPU_POWER1 0x6b
#define MPU_POWER2 0x6c


int16_t xaccel = 0;
int16_t yaccel = 0;
int16_t zaccel = 0;
int16_t xgyro  = 0;
int16_t ygyro = 0;
int16_t zgyro = 0;

double temp = 0.0;
double station_press = 0.0;
double sea_press = 0.0;
double humidity = 0.0;

struct mesg_buffer {
    long mesg_type;
    char mesg_text[200];
} message;


void mpu6050() {
    int fd;
    char *fileName = "/dev/i2c-1";
    int  address = 0x68;
	
    if ((fd = open(fileName, O_RDWR)) < 0) {
        printf("Failed to open i2c port\n");
      //  exit(1);
    }
	
    if (ioctl(fd, I2C_SLAVE, address) < 0) {
        printf("Unable to get bus access to talk to slave\n");
      //  exit(1);
    }
    
    int8_t power = i2c_smbus_read_byte_data(fd, MPU_POWER1);
    i2c_smbus_write_byte_data(fd, MPU_POWER1, ~(1 << 6) & power);

    
    int16_t temp1 = i2c_smbus_read_byte_data(fd, MPU_TEMP1) << 8 |
                        i2c_smbus_read_byte_data(fd, MPU_TEMP2);

    xaccel = i2c_smbus_read_byte_data(fd, MPU_ACCEL_XOUT1) << 8 |
                         i2c_smbus_read_byte_data(fd, MPU_ACCEL_XOUT2);
    yaccel = i2c_smbus_read_byte_data(fd, MPU_ACCEL_YOUT1) << 8 |
                         i2c_smbus_read_byte_data(fd, MPU_ACCEL_YOUT2);
    zaccel = i2c_smbus_read_byte_data(fd, MPU_ACCEL_ZOUT1) << 8 |
                         i2c_smbus_read_byte_data(fd, MPU_ACCEL_ZOUT2);

    xgyro = i2c_smbus_read_byte_data(fd, MPU_GYRO_XOUT1) << 8 |
                        i2c_smbus_read_byte_data(fd, MPU_GYRO_XOUT2);
    ygyro = i2c_smbus_read_byte_data(fd, MPU_GYRO_YOUT1) << 8 |
                        i2c_smbus_read_byte_data(fd, MPU_GYRO_YOUT2);
    zgyro = i2c_smbus_read_byte_data(fd, MPU_GYRO_ZOUT1) << 8 |
                        i2c_smbus_read_byte_data(fd, MPU_GYRO_ZOUT2);


    //printf("accel x,y,z: %d, %d, %d\n", (int)xaccel, (int)yaccel, (int)zaccel);
    //printf("gyro x,y,z: %d, %d, %d\n\n", (int)xgyro, (int)ygyro, (int)zgyro);
    
}


double sta2sea(double station_press) {
    return station_press * exp((-M * G * -LOCAL_HASL) / (R * T));
}


void bme280() {
    int fd = 0;
    uint8_t dataBlock[8];
    int32_t temp_int = 0;
    int32_t press_int = 0;
    int32_t hum_int = 0;


    /* open i2c comms */
    if ((fd = open(DEV_PATH, O_RDWR)) < 0) {
        perror("Unable to open i2c device");
        return;
    }

    /* configure i2c slave */
    if (ioctl(fd, I2C_SLAVE, DEV_ID) < 0) {
        perror("Unable to configure i2c slave device");
        close(fd);
        return;
    }

    /* check our identification */
    if (i2c_smbus_read_byte_data(fd, IDENT) != 0x60) {
        perror("device ident error");
        close(fd);
        return;
    }

    /* device soft reset */
    i2c_smbus_write_byte_data(fd, SOFT_RESET, 0xB6);
    usleep(50000);

    /* read and set compensation parameters */
    setCompensationParams(fd);

    /* humidity o/s x 1 */
    i2c_smbus_write_byte_data(fd, CTRL_HUM, 0x1);

    /* filter off */
    i2c_smbus_write_byte_data(fd, CONFIG, 0);

    /* set forced mode, pres o/s x 1, temp o/s x 1 and take 1st reading */
    i2c_smbus_write_byte_data(fd, CTRL_MEAS, 0x25);

     /* check data is ready to read */
     if ((i2c_smbus_read_byte_data(fd, STATUS) & 0x9) != 0) {
          printf("%s\n", "Error, data not ready");
          return;
     }

     /* read data registers */
     i2c_smbus_read_i2c_block_data(fd, DATA_START_ADDR, DATA_LENGTH, dataBlock);

     /* awake and take next reading */
     i2c_smbus_write_byte_data(fd, CTRL_MEAS, 0x25);

     /* get raw temp */
     temp_int = (dataBlock[3] << 16 | dataBlock[4] << 8 | dataBlock[5]) >> 4;

     /* get raw pressure */
     press_int = (dataBlock[0] << 16 | dataBlock[1] << 8 | dataBlock[2]) >> 4;

     /* get raw humidity */
     hum_int = dataBlock[6] << 8 | dataBlock[7];

     /* calculate and print compensated temp. This function is called first, as it also sets the
      * t_fine global variable required by the next two function calls
      */
    temp = BME280_compensate_T_double(temp_int);

     station_press = BME280_compensate_P_double(press_int) / 100.0;


     /* calculate and print compensated press */
     sea_press =  sta2sea(station_press);

     /* calculate and print compensated humidity */
     humidity = BME280_compensate_H_double(hum_int);

}

int main(int argc, char **argv) {

    printf("\n**************************************************************");
    printf("\n***********************Vehicle Status*************************");
    printf("\n**************************************************************\n\n");
    
    key_t key;
    int msgid;
  
    // ftok to generate unique key
    key = ftok("progfile", 65);
  
    // msgget creates a message queue
    // and returns identifier
    msgid = msgget(key, 0666 | IPC_CREAT);
    message.mesg_type = 1;
    int roll = 0;
    while(1) {
    	mpu6050();
    	bme280();
    	roll = atan2(yaccel, zaccel)* 180 / 3.14159265;
    	
    	snprintf(message.mesg_text, sizeof(message.mesg_text), "%d", roll);
    	msgsnd(msgid, &message, sizeof(message), 0);
    	snprintf(message.mesg_text, sizeof(message.mesg_text), "%d", roll);
    	msgsnd(msgid, &message, sizeof(message), 0);  
    	snprintf(message.mesg_text, sizeof(message.mesg_text), "%d", roll);
    	msgsnd(msgid, &message, sizeof(message), 0);
    	
    	sleep(3);    	
    //	snprintf(message.mesg_text, sizeof(message.mesg_text), "X acceleration = %d, Y acceleration = %d, Z acceleration = %d, X Gyro = %d, Y Gyro = %d, Z Gyro = %d, Temperature = %f, Tyre pressure = %f, Sea pressure = %f, Humidity = %f",(int) xaccel, (int)yaccel, (int)zaccel, (int)xgyro, (int)ygyro, (int)zgyro, temp, station_press, sea_press, humidity);
    	
    	    // msgsnd to send message

    

    }



}

