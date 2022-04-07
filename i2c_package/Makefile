ifeq ($(CC),)
	CC= $(CROSS_COMPILE)gcc
endif

ifeq ($(CFLAGS),)
	CFLAGS= -g -Wall
endif
 
ifeq ($(LDFLAGS),)
	LDFLAGS= -pthread 
endif

TARGET= bme280
SRCS:= bme280.c \
       compensation.c \
       bme280.h \
       compensation.h \
       i2c_utils.h 



default: $(TARGET)
all: $(TARGET)


$(TARGET): $(SRCS) 
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDES) $(LDFLAGS)


clean: 	
	rm -f *.o $(TARGET) *.elf *.map
