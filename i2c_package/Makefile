CROSS_COMPILE=

ifeq ($(CC),)
	CC=$(CROSS_COMPILE)gcc
endif

ifeq ($(CFLAGS),)
	CFLAGS= -g -Wall -Werror
endif

all: bme_sensor
default: bme_sensor

bme_sensor: main.o bme280.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o bme_sensor main.o bme280.o $(LDFLAGS)

clean :
	rm -f *.o bme_sensor
