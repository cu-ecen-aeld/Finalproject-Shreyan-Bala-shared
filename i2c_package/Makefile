ifeq ($(CC),)
	CC= $(CROSS_COMPILE)gcc
endif

ifeq ($(CFLAGS),)
	CFLAGS= -g -Wall
endif
 
ifeq ($(LDFLAGS),)
	LDFLAGS= -pthread 
endif

TARGET1= mpu6050
TARGET2= bme280
SRCS1:= mpu6050.c \
	i2c_utils.h
SRCS2:= getbme280.c i2c_bme280.c getbme280.h


default: $(TARGET1) $(TARGET2)
all: $(TARGET1) $(TARGET2)


$(TARGET1): $(SRCS1) 
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDES) $(LDFLAGS)
$(TARGET2): $(SRCS2) 
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDES) $(LDFLAGS)


clean: 	
	rm -f *.o $(TARGET1) *.elf *.map
	rm -f *.o $(TARGET2) *.elf *.map
