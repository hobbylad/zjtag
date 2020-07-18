## Description

This project is a fork of [zjtag](https://github.com/zoobab/zjtag) with support for using the Raspberry Pi as a JTAG programmer. In addition to the various parallel port and USB JTAG adaptors supported by the parent project, it is now possible to use the Pi's GPIO to drive the JTAG interface.

This tool is typically used to program a custom bootloader to the flash device of supported wireless routers to allow open source firmware such as [Openwrt](https://openwrt.org/) to be booted. This project was used on a Raspberry Pi 3 to program OpenWrt CFE bootloader to a [BT HomeHub 2.0A](https://openwrt.org/toh/bt/homehub_v2a). A similar project called [tjtag-pi](https://github.com/oxplot/tjtag-pi) exists and although I could use it to program and verify FLASH memory, the BT HomeHub 2.0A sadly would not boot.

This project uses the [BCM2835 library](https://www.airspayce.com/mikem/bcm2835/) to write to GPIO. Since only the GPIO is accessed, zjtag should not need to run as "root" on recent versions of the Raspbian OS. The library is just used to configure the GPIO direction, sense and drive GPIO lines without anything too fancy.

## Building

To compile the project, the build tools and library dependencies need to be installed as in the parent project. The only addition is the BCM2835 library and the installation is detailed [here](https://www.airspayce.com/mikem/bcm2835).

```sh
cd zjtag
make zjtag
```
## Usage

Connect the Raspberry Pi GPIO pins on [connector J8 header](https://www.raspberrypi.org/documentation/usage/gpio/README.md) to the target device as follows:

| RPi J8 pin | RPi signal type | JTAG signal|
| ------ | ------ | ------ |
| 11 | output | TDI|
| 16 | input | TDO |
| 18 | output | TMS |
| 26 | output | TCK |

For the convenience, the same GPIO allocation is used as in tjtag-pi.

If all is connected correctly and with the target powered it should be possible to query the CPU chip ID. This needs to work reliably before anything else is attempted.
```sh
pi@pi:~/zjtag $ sudo ./zjtag -probeonly /cable:9
```
It should also be possible to program flash memory.
```sh
pi@pi:~/zjtag $ sudo ./zjtag -flash:custom /window:1e000000 /start:1e000000 /length:20000 /BE /cable:9 /fc:093
```
Below is the log for programming the OpenWrt CFE bootloader on a BT HomeHub 2.0A. 
```sh
        ==============================================
               zJTAG EJTAG Debrick Utility v1.9
        ==============================================

cableid=9, cabletype=8

Raspberry Pi open done
Detected IR chain length = 32

There are 1 device(s) in the JTAG chain
 IDCODE for device 1 is 0x0635817F (IR length:1)

Probing bus ... Done

Defined IR Length is 5 bits

CPU assumed running under BIG endian

CPU Chip ID: 00000110001101011000000101111111 (0x0635817F)
*** Found a Broadcom manufactured BCM6358 REV 01 CPU ***

    - EJTAG IMPCODE ....... : 00000000100000011000100100000100 (0x00818904)
    - EJTAG Version ....... : 1 or 2.0
    - EJTAG DMA Support ... : Yes
    - EJTAG Implementation flags: R4k MIPS16 MIPS32

Issuing Processor / Peripheral Reset ... Done
Enabling Memory Writes ... Done
Halting Processor ... <Processor Entered Debug Mode!> ... Done
Clearing Watchdog ... Done
Loading CPU Configuration Code ... Skipped
Detecting Flash Base Address...
Read MPI register value : 1E00000C
MPI register show Flash Access Base Addr : 1E000000
*** Manually Selected a ST M29W128GL Uni          (16MB) from ST/Numonyx

    - Flash Chip Window Start .... : 1E000000
    - Flash Chip Window Length ... : 01000000
    - Selected Area Start ........ : 1E000000
    - Selected Area Length ....... : 00020000

*** You Selected to Flash the CUSTOM.BIN ***

=========================
Flashing Routine Started
=========================
Total Blocks to Erase: 1

Erasing block: 1 (addr = 1E000000)...Done

Loading CUSTOM.BIN to Flash Memory...
Done  (CUSTOM.BIN loaded into Flash Memory OK)

=========================
Flashing Routine Complete
=========================
elapsed time: 41 seconds


 *** REQUESTED OPERATION IS COMPLETE ***
Raspberry Pi close done
```


