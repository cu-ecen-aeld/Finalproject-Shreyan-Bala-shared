ifeq ($(CC),)
	CC= $(CROSS_COMPILE)gcc
endif

ifeq ($(CFLAGS),)
	CFLAGS= -g -Wall
endif
 
ifeq ($(LDFLAGS),)
	LDFLAGS= -pthread 
endif

TARGET= mpu6050
SRCS:= mpu6050.c

default: $(TARGET)
all: $(TARGET)


$(TARGET): $(SRCS) 
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDES) $(LDFLAGS)


clean: 	
	rm -f *.o $(TARGET) *.elf *.map
