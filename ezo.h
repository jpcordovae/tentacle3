#ifndef EZO_H__
#define EZO_H__

#include <fcntl.h>
#include <sys/types.h>
#include <asm/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

// i2c bus
#include <bcm2835.h>
#include <string>
#include <iostream>

#define PHADDR 0x63
#define ECADDR 0x64
#define TADDR  0x66

extern uint16_t clk_div;

bool read_ezo_device(__u8 address, __u8 *buffer, size_t buffersize);
bool write_ezo_device(__u8 address, __u8 *buffer, size_t buffersize);



#endif
