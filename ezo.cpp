#include "ezo.h"

uint16_t clk_div = BCM2835_I2C_CLOCK_DIVIDER_2500;

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

bool send_command(__u8 address, __u8 *command, ssize_t sleep_time_us, bool wait_response=true)
{
  char response[32];
  memset(response,0x00,sizeof(response));

  if(!write_ezo_device(address,command,sizeof(command))) {
    printf("error sending ec calibration command/r/n");
    return false;
  }

  usleep(sleep_time_us);

  if(!read_ezo_device(address,(__u8*)response,sizeof(response))) {
    printf("error getting ec calibration response/r/n");
    return false;
  }

  if(wait_response) {
    if(response[1] != '1') {
      return false;
    }
  }

  return true;
}

bool calibrate_ec(float cal_value, calibration_type c_type)
{
  char command[32];
  char response[32];

  memset(command,0x00,32);
  memset(response,0x00,32);

  switch(c_type) {
  case DRY:
    sprintf(command,"CAL,DRY");
    send_command(ECADDR,(__u8*)command,600000,true);
    break;
  case SINGLE_POINT:
    sprintf(command,"Cal,%f",cal_value);
    send_command(ECADDR,(__u8*)command,600000,true);
    break;
  case LOW_END:
    sprintf(command,"Cal,low,%f",cal_value);
    send_command(ECADDR,(__u8*)command,600000,true);
    break;
  case HIGH_END:
    sprintf(command,"Cal,high,%f",cal_value);
    send_command(ECADDR,(__u8*)command,600000,true);
    break;
  case CLEAR:
    sprintf(command,"CAL,clear");
    send_command(ECADDR,(__u8*)command,600000,true);
    break;
  default:
    break;
  }
  
  return true;
}
