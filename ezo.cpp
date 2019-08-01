#include "ezo.h"

uint16_t clk_div = BCM2835_I2C_CLOCK_DIVIDER_626;

bool read_ezo_device(__u8 address, __u8 *buffer, size_t buffersize)
{
  if (!bcm2835_i2c_begin()) {
    printf("bcm2835_i2c_begin failed. Are you running as root??\n");
    return 1;
  }
  
  bcm2835_i2c_setSlaveAddress(address);
  bcm2835_i2c_setClockDivider(clk_div);
  bcm2835_i2c_read((char*)buffer,buffersize);
  bcm2835_i2c_end();
  
  return true;
}

bool write_ezo_device(__u8 address, __u8 *buffer, size_t buffersize)
{
  if (!bcm2835_i2c_begin()) {
    printf("bcm2835_i2c_begin failed. Are you running as root??\n");
    return false;
  }
  
  bcm2835_i2c_setSlaveAddress(address);
  bcm2835_i2c_setClockDivider(clk_div);
  bcm2835_i2c_write((char *)buffer,buffersize);
  bcm2835_i2c_end();
  
  return true;
}

