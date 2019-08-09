#include "ezo.h"

char filename[20];

typedef struct st_sensors{
  float ph;
  float ec;
  float temperature;
  float oxigen;
} st_sensors;

std::ostream& operator<<(std::ostream &os, const st_sensors &measures)
{
  os << "    ph: " << std::to_string(measures.ph) << std::endl
     << "    ec: " << std::to_string(measures.ec) << std::endl
     << "  temp: " << std::to_string(measures.temperature) << std::endl
     << "oxigen: " << std::to_string(measures.oxigen) << std::endl;
  return os;
}

st_sensors *shm_sensors;

void print_buffer(__u8 *buffer, size_t buffersize)
{
  for(int k=0; k < buffersize; k++){
    printf("%02x",buffer[k]);
  }
  printf("\r\n");
}

bool read_ph(float &value)
{
  __u8 resp[32];
  __u8 command = 'R';
  memset(resp,0x00,sizeof(resp));

  if(!write_ezo_device(PHADDR,(__u8*)&command,1)) {
    printf("Error reading pH, write_ezo_device \r\n");
    return false;
  }
  
  usleep(1000000);

  if(!read_ezo_device(PHADDR,resp,sizeof(resp))){
    printf("Error reading ph, read_ezo_device \r\n");
    return false;
  }
  print_buffer((__u8*)resp,32);
  value = atof((char*)(resp+1));
  return true;
}

bool read_ec(float &value)
{
  __u8 resp[32];
  __u8 command = 'R';;
  memset(resp,0x00,32); // clear buffer

  if(!write_ezo_device(ECADDR,(__u8*)&command,1)){  // send {'R',0x00 }
    printf("Error reading EC, write_ezo_device \r\n");
    return false;
  }

  usleep(1000000); // sleep 1.5s
  
  if(!read_ezo_device(ECADDR,resp,sizeof(resp))) {
    printf("Error reading EC, read_ezo_device \r\n");
    return false;
  }
  print_buffer((__u8*)resp,sizeof(resp)); 
  value = atof((char*)(resp+1));
  return true;
}

bool read_t(float &value)
{
  __u8 resp[32];
  __u8 command = 'R';;
  memset(resp,0x00,32); // clear buffer

  if(!write_ezo_device(TADDR,(__u8*)&command,1)){  // send {'R',0x00 }
    printf("Error reading RTD, write_ezo_device \r\n");
    return false;
  }

  usleep(1000000); // sleep 1.5s
  
  if(!read_ezo_device(TADDR,resp,sizeof(resp))) {
    printf("Error reading RTD, read_ezo_device \r\n");
    return false;
  }

  value = atof((char*)(resp+1));  
  return true;
}

void ec_factory_reset()
{
  __u8 command[8];
  memset(command,0x00,8); // clear buffer
  memcpy(command,"Factory",7);
  if(!write_ezo_device(ECADDR,(__u8*)&command,7)){  // send {'R',0x00 }
    printf("Error factory reset for ec, write_ezo_device \r\n");
  }
  usleep(100000); 
}

int main(int argc, char *argv[])
{
  if (!bcm2835_init()) {
    printf("bcm2835_init failed. Are you running as root??\n");
    exit(EXIT_FAILURE);;
  }
  
  shm_sensors = (st_sensors*) new(st_sensors);

  if(argc <= (unsigned int)1) {
    printf("******************************************\r\n");
    printf("TENTACLE 3 -  for Raspberry PI 3\r\n");
    printf("ph, ec, temp reader\r\n");
    printf("******************************************\r\n");
    printf("jpcordovae@gmail.com | Jul-2019\r\n");
  }

  float ph = 0.0f;
  float ec = 0.0f;
  float t  = 0.0f;
  
  //ec_factory_reset();
  
  while(1) {
    
    if(!read_ph(ph)) {
      printf("pH reading error.\r\n");
      continue;
    }

    if(!read_ec(ec)) {
      printf("ec reading error.\r\n");
      continue;
    }

    if(!read_t(t)) {
      printf("temperature reading error.\r\n");
      continue;
    }

    shm_sensors->ph = ph;
    shm_sensors->ec = ec;
    shm_sensors->temperature = t;
    
    std::cout << *shm_sensors << std::endl;
    
  }
  
}

