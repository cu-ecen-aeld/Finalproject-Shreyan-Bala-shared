/*
*@Name:gpio_write.c
*@Author: Shreyan Prabhu D and Balapranesh Elango
*@Brief: GPIO testing code
*  @Reference - https://ostconf.com/system/attachments/
*  files/000/001/532/original/
*  Linux_Piter_2018_-_New_GPIO_interface_for_linux_userspace.pdf?1541021776
*/


#include <stdio.h>
#include <gpiod.h>
#include <unistd.h>

#define GPIO_PIN    5

struct gpiod_chip *chip;
struct gpiod_line *line;
int rv, value;

int main()
{
    chip= gpiod_chip_open("/dev/gpiochip0");

    if (!chip)
     return -1;

    line = gpiod_chip_get_line(chip, GPIO_PIN);

    if (!line) 
    {
     gpiod_chip_close(chip);
     return -1; 
    }

    rv = gpiod_line_request_output(line, "foobar", 1);

    if (rv) 
    {
     gpiod_chip_close(chip);
     return -1;
    }

    value = gpiod_line_set_value(line, 1);
    printf("GPIO%d value is cleared to 0\n", GPIO_PIN);
    sleep(1);
    gpiod_chip_close(chip);

    return 0;
}
