ifeq ($(CC),)
	CC= $(CROSS_COMPILE)gcc
endif

ifeq ($(CFLAGS),)
	CFLAGS= -g -Wall
endif
 
ifeq ($(LDFLAGS),)
	LDFLAGS= -pthread -std=c++11
endif


TARGET= oled

SRCS:= oled.cpp ssd1306.cpp 


default: $(TARGET)
all: $(TARGET)


$(TARGET): $(SRCS) 
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDES) $(LDFLAGS)

clean: 	
	rm -f *.o $(TARGET) *.elf *.map
