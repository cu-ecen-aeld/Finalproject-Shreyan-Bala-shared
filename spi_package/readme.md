The spidevtest is useful to verify that the SPI bus is working correctly on Rasperrbg Pi 3b+ post the SPI drivers are installed in the yocto build
The code is referenced from the https://github.com/torvalds/linux/blob/master/tools/spi/spidev_test.c 
The follwing changes are made from the above link

1) The default device i schanged from /dev/spidev1.1 to /dev/spidev0.0
2) The SPI mode are changed from SPI_IOC_WR_MODE32 and SPI_IOC_RD_MODE32 to SPI_IOC_WR_MODE and SPI_IOC_RD_MODE respectively
