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


SRCS1:= mpu6050.c \
	i2c_utils.h



default: $(TARGET1) 
all: $(TARGET1) 


$(TARGET1): $(SRCS1) 
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDES) $(LDFLAGS)



clean: 	
	rm -f *.o $(TARGET2) *.elf *.map
