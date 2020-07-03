

#include "rpi.h"
#include "zjtag.h"

#include <stdio.h>
#include <bcm2835.h>

#if 1
#define debug_printf(...)
#else
#define debug_printf(s, ...) printf((s), __VA_ARGS__)
#endif

/*
 * Raspberry PI GPIO pins on connector J8:
 *
 *     pin 11 (output): TDI
 *     pin 16 (input ): TDO
 *     pin 18 (output): TMS
 *     pin 26 (output): TCK
 *
 * J8 pinout:
 *
 */
static const int tdi_pin = RPI_BPLUS_GPIO_J8_11,
                 tdo_pin = RPI_BPLUS_GPIO_J8_16,
                 tms_pin = RPI_BPLUS_GPIO_J8_18,
                 tck_pin = RPI_BPLUS_GPIO_J8_26;

int rpi_open(void)
{
	if (!bcm2835_init())
	{
		printf("Raspberry Pi open failed. Are you running as root?\n");
		return -1;
	}

	bcm2835_gpio_fsel(tdi_pin, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_clr(tdi_pin);

	bcm2835_gpio_fsel(tck_pin, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_clr(tck_pin);

	bcm2835_gpio_fsel(tms_pin, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_clr(tms_pin);

	bcm2835_gpio_fsel(tdo_pin, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_set_pud(tdo_pin, BCM2835_GPIO_PUD_OFF);
	bcm2835_gpio_fsel(tdo_pin, BCM2835_GPIO_FSEL_INPT);

	printf("Raspberry Pi open done\n");
	return 0;
}

void rpi_close(void)
{
	if (!bcm2835_close())
	{
		printf("rpi close failed.\n");
	}
	printf("Raspberry Pi close done\n");
}

void rpi_outp(uint8_t data)
{
	debug_printf("OUTP: 0x%2.2x\n", data);
	bcm2835_gpio_write(tck_pin, (data & (1 << WTCK)) ? HIGH : LOW);
	bcm2835_gpio_write(tdi_pin, (data & (1 << WTDI)) ? HIGH : LOW);
	bcm2835_gpio_write(tms_pin, (data & (1 << WTMS)) ? HIGH : LOW);
}

uint8_t rpi_inp(void)
{
  uint8_t data = (bcm2835_gpio_lev(tdo_pin) != 0);
  debug_printf("INP:  0x%2.2x\n", data);
  return ((!data) << WTDO);
}





