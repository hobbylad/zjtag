#ifndef RPI_H
#define RPI_H

#include <stdint.h>

int rpi_open(void);
void rpi_close(void);
void rpi_outp(uint8_t data);
uint8_t rpi_inp(void);

#endif
