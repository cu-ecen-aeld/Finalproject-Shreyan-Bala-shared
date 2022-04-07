CROSS_COMPILE=

ifeq ($(CC),)
	CC=$(CROSS_COMPILE)gcc
endif

ifeq ($(CFLAGS),)
	CFLAGS= -g -Wall -Werror
endif

#ifeq ($(LDFLAGS),)
#	LDFLAGS= -lm
#endif
all: bme_sensor
default: bme_sensor

bme_sensor: i2c_bme280.o getbme280.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o bme_sensor i2c_bme280.o getbme280.o $(LDFLAGS)

clean :
	rm -f *.o bme_sensor
