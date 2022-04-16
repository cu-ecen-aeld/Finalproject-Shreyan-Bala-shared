ifeq ($(CC),)
	CC= $(CROSS_COMPILE)gcc
endif

ifeq ($(CFLAGS),)
	CFLAGS= -g -Wall
endif
 
ifeq ($(LDFLAGS),)
	LDFLAGS= -pthread -lm
endif


TARGET1= sensor 
TARGET2= server

SRCS1:= sensors.c \
       compensation.c \
       bme280.h \
       compensation.h \
       i2c_utils.h 

SRCS2:= server.c

default: $(TARGET1) $(TARGET2)
all: $(TARGET1) $(TARGET2)


$(TARGET1): $(SRCS1) 
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDES) $(LDFLAGS)

$(TARGET2): $(SRCS2) 
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDES) $(LDFLAGS)

clean: 	
	rm -f *.o $(TARGET1) *.elf *.map
	rm -f *.o $(TARGET2) *.elf *.map
