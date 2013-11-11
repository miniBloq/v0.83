/* Modified for the LUFA HID Bootloader by Dean Camera
 *           http://www.lufa-lib.org
 *
 *   THIS MODIFIED VERSION IS UNSUPPORTED BY PJRC.
 */

/* Teensy Loader, Command Line Interface
 * Program and Reboot Teensy Board with HalfKay Bootloader
 * http://www.pjrc.com/teensy/loader_cli.html
 * Copyright 2008-2010, PJRC.COM, LLC
 *
 *
 * You may redistribute this program and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/
 */

/* Want to incorporate this code into a proprietary application??
 * Just email paul@pjrc.com to ask.  Usually it's not a problem,
 * but you do need to ask to use this code in any way other than
 * those permitted by the GNU General Public License, version 3  */

/* For non-root permissions on ubuntu or similar udev-based linux
 * http://www.pjrc.com/teensy/49-teensy.rules
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

#include "ihex.h"
#include "usb_XXX.h"

void usage(void)
{
	fprintf(stderr, "Usage: hiduploader -mmcu=<MCU> [-v] -usb=<vid:pid> [-d=<mmmm>] <file.hex>\n");
	fprintf(stderr, "\t-v : Verbose output\n");
	fprintf(stderr, "\n<MCU> = atmegaXXuY or at90usbXXXY");
	fprintf(stderr, "\n<vid:pid> = vid:pid of HID device in hex (eg. -usb=0x2842:0x0001)");
	fprintf(stderr, "\n<mmmm> = Optional delay time in miliseconds to wait between program datapages");

	fprintf(stderr, "HID Uploader v1.0 (Based on code from Teensy Loader 2.0 - command line)\n");
	fprintf(stderr, "Federico Lanza (ingfedericolanza[@]gmail.com) for Robotgroup (multiplo.org)\n");

	fprintf(stderr, "\nFor support and more information, please visit:\n");
	fprintf(stderr, "http://www.multiplo.org\n");

	fprintf(stderr, "\nBased on the TeensyHID command line programmer software:\n");
	fprintf(stderr, "http://www.pjrc.com/teensy/loader_cli.html\n");
	exit(1);
}

#if defined(OS_WINDOWS)
#include <conio.h>
#include <windows.h>
#endif

// Misc stuff
int printf_verbose(const char *format, ...);
void delay(double seconds);
void die(const char *str, ...);
void parse_options(int argc, char **argv );

// options (from user via command line args)
int verbose = 0;
int DelayDataPage = 0;
int vid, pid;
int code_size = 0, block_size = 0;
const char *filename=NULL;


/****************************************************************/
/*                                                              */
/*                       Misc Functions                         */
/*                                                              */
/****************************************************************/

int printf_verbose(const char *format, ...)
{
	va_list ap;
	int r;

	va_start(ap, format);
	if (verbose) {
		r = vprintf(format, ap);
		fflush(stdout);
		return r;
	}
	return 0;
}

void delay(double seconds)
{
	#ifdef WIN32
	Sleep((DWORD)(seconds * 1000.0));
	#else
	usleep(seconds * 1000000.0);
	#endif
}

void die(const char *str, ...)
{
	va_list  ap;

	va_start(ap, str);
	vfprintf(stderr, str, ap);
	fprintf(stderr, "\n");
	exit(1);
}

#if defined(WIN32)
#define strcasecmp stricmp
#endif

void parse_options(int argc, char **argv)
{
	int i;
	const char *arg;


	for (i=1; i<argc; i++) {
		arg = argv[i];

		if (*arg == '-') {
            if (strcmp(arg, "-v") == 0) {
                arg += 2;
				verbose = 1;
            } else if (strncmp(arg, "-d=", 3) == 0) {
                arg += 3;
                if (sscanf(arg, "%d", &DelayDataPage) != 1) die("Malformed Delay parameter\n");
            } else if (strncmp(arg, "-usb=", 5) == 0) {
                arg += 5;
                if (sscanf(arg, "%x", &vid) != 1) die("Malformed VID\n");
                arg += 7;
                if (sscanf(arg, "%x", &pid) != 1) die("Malformed PID\n");
			} else if (strncmp(arg, "-mmcu=", 6) == 0) {
				arg += 6;

				uint8_t valid_prefix = 0;

				if (strncmp(arg, "at90usb", 7) == 0) {
					valid_prefix = 1;
					arg += 7;
				} else if (strncmp(arg, "atmega", 6) == 0) {
					valid_prefix = 1;
					arg += 6;
				} else {
					die("Unknown MCU type\n");
				}

				if (strncmp(arg, "128", 3) == 0) {
					code_size  = 128 * 1024;
					block_size = 256;
				} else if (strncmp(arg, "64", 2) == 0) {
					code_size  = 64 * 1024;
					block_size = 256;
				} else if (strncmp(arg, "32", 2) == 0) {
					code_size  = 32 * 1024;
					block_size = 128;
				} else if (strncmp(arg, "16", 2) == 0) {
					code_size  = 16 * 1024;
					block_size = 128;
				} else if (strncmp(arg, "8", 1) == 0) {
					code_size  = 8 * 1024;
					block_size = 128;
				} else {
					die("Unknown MCU type\n");
				}
			}
		} else {
			filename = argv[i];
		}
	}
}

#define SUBHEADER_RESET			0x01
#define SUBHEADER_LOADADDR		0x02
#define SUBHEADER_PROGRAMPAGE	0x03
#define SUBHEADER_DATAPAGE		0x04
#define SUBHEADER_CLOSEDATAPAGE	0x05
#define SUBHEADER_STARTAPP		0x06
#define SUBHEADER_ENDOFUPLOAD	0x07
#define SUBHEADER_SETBAUDRATE	0x08

#define ST_RESET            1
#define ST_LOADADDR         2
#define ST_PROGRAMPAGE      3
#define ST_DATAPAGE         4
#define ST_CLOSEDATAPAGE    5
#define ST_STARTAPP         6
#define ST_ENDOFUPLOAD      7

/****************************************************************/
/*                                                              */
/*                       Main Program                           */
/*                                                              */
/****************************************************************/
int main(int argc, char **argv)
{
	unsigned char ControlBuf[64];
	unsigned char ProgramBuf[256];
	int BuffOffset = 0;
	int addr = 0;
	int waddr = 0;
	int first_block = 1;
    unsigned char ProgramDevice = 1;
    unsigned char ProgramState = ST_RESET;
	int num, r;

	// parse command line arguments
	parse_options(argc, argv);
	if (!filename) {
		fprintf(stderr, "Filename must be specified\n\n");
		usage();
	}
	if (!code_size) {
		fprintf(stderr, "MCU type must be specified\n\n");
		usage();
	}
	printf("\nHID Uploader v1.2 (Based on code from Teensy Loader 2.0 - command line)\n");
	printf("Federico Lanza (ingfedericolanza[@]gmail.com) for multiplo.org\n");

	// read the intel hex file
	// this is done first so any error is reported before using USB
	num = read_intel_hex(filename);
	if (num < 0) die("error reading intel hex file \"%s\"", filename);
	printf_verbose("Read \"%s\": %d bytes, %.1f%% usage\n",
		filename, num, (double)num / (double)code_size * 100.0);

    // Show delay specified (if any)
    if (DelayDataPage > 0) {
        printf_verbose("Added delay between datapages: %d ms\n", DelayDataPage);
    }

	// Open the USB device
	if (teensy_open(vid, pid)) {
        printf_verbose("Found HID Bootloader\n");
	} else {
        die("Unable to open device\n");
	}

	// Program the data
	printf_verbose("Programming: ");
	fflush(stdout);

	while (ProgramDevice) {
        switch (ProgramState) {
            case ST_RESET:
                // Send Reset to Target
                ControlBuf[0] = 0xA1;
                ControlBuf[1] = SUBHEADER_RESET;
                memset(ControlBuf + 2, 0, sizeof(ControlBuf) - 2);
                r = teensy_write(ControlBuf, sizeof(ControlBuf), 1.0);
                if (!r) die("Error writing to device\n");
                delay(1.0);
                addr = 0;
                ProgramState = ST_LOADADDR;
                break;
            case ST_LOADADDR:
                // Send Address of current block
                if (addr >= code_size) {
                    // Reach end of program, start application
                    ProgramState = ST_STARTAPP;
                } else {
                    if (addr > 0 && !ihex_bytes_within_range(addr, addr + block_size - 1)) {
                        // Avoid unused blocks
                        addr += block_size;
                    } else {
                        // Send current block address
                        ControlBuf[0] = 0xA1;
                        ControlBuf[1] = SUBHEADER_LOADADDR;
                        // Address must be sent in word format
                        waddr = addr >> 1;
                        ControlBuf[2] = waddr & 0xFF;
                        ControlBuf[3] = (waddr >> 8) & 0xFF;
                        memset(ControlBuf + 4, 0, sizeof(ControlBuf) - 4);
                        r = teensy_write(ControlBuf, sizeof(ControlBuf), 0.25);
                        if (!r) die("Error writing to device\n");
                        //
                        // Wait delay for some windows systems to work
                        delay(0.01 + ((float)DelayDataPage / 1000.0));
                        //
                        // Start Program Page command
                        ProgramState = ST_PROGRAMPAGE;
                    }
                }
                break;
            case ST_PROGRAMPAGE:
                // Send program page
                ControlBuf[0] = 0xA1;
                ControlBuf[1] = SUBHEADER_PROGRAMPAGE;
                ControlBuf[2] = (block_size >> 8) & 0xFF;
                ControlBuf[3] = block_size & 0xFF;
                memset(ControlBuf + 4, 0, sizeof(ControlBuf) - 4);
                r = teensy_write(ControlBuf, sizeof(ControlBuf), 1.0);
                if (!r) die("Error writing to device\n");
                delay(0.01);
                printf_verbose(".");
                // Load Data of page from hex file
                ihex_get_data(addr, block_size, ProgramBuf);
                // Next time send data of page to be programmed, starting in byte 0
                BuffOffset = 0;
                ProgramState = ST_DATAPAGE;
                break;
            case ST_DATAPAGE:
                // Send current data page
                memset(ControlBuf, 0, sizeof(ControlBuf));
                ControlBuf[0] = 0xA1;
                ControlBuf[1] = SUBHEADER_DATAPAGE;
                memcpy(&ControlBuf[2], &ProgramBuf[BuffOffset], 32);
                r = teensy_write(ControlBuf, sizeof(ControlBuf), 1.0);
                if (!r) die("Error writing to device\n");
                // Wait delay for some windows systems to work
                delay(0.01 + ((float)DelayDataPage / 1000.0));
                // Next time with next byte
                BuffOffset += 32;
                if (BuffOffset > (block_size - 1)) {
                    ProgramState = ST_CLOSEDATAPAGE;
                }
                break;
            case ST_CLOSEDATAPAGE:
                // Send end of page command
                ControlBuf[0] = 0xA1;
                ControlBuf[1] = SUBHEADER_CLOSEDATAPAGE;
                memset(ControlBuf + 2, 0, sizeof(ControlBuf) - 2);
                r = teensy_write(ControlBuf, sizeof(ControlBuf), first_block ? 3.0 : 0.25);
                if (!r) die("Error writing to device\n");
                delay(0.01);
                // Next Block
                addr += block_size;
                first_block = 0;
                ProgramState = ST_LOADADDR;
                break;
            case ST_STARTAPP:
                // Send Start of app command
                ControlBuf[0] = 0xA1;
                ControlBuf[1] = SUBHEADER_STARTAPP;
                memset(ControlBuf + 2, 0, sizeof(ControlBuf) - 2);
                r = teensy_write(ControlBuf, sizeof(ControlBuf), 0.25);
                if (!r) die("Error writing to device\n");
                delay(0.01);
                ProgramState = ST_ENDOFUPLOAD;
                break;
            case ST_ENDOFUPLOAD:
                // Send End of upload
                ControlBuf[0] = 0xA1;
                ControlBuf[1] = SUBHEADER_ENDOFUPLOAD;
                memset(ControlBuf + 2, 0, sizeof(ControlBuf) - 2);
                r = teensy_write(ControlBuf, sizeof(ControlBuf), 0.25);
                if (!r) die("Error writing to device\n");
                delay(0.3);
                // Quit from programming mode
                printf_verbose("\nStarting Application\n");
                ProgramDevice = 0;
                break;
        }
	}
	teensy_close();

	die("\nDone.\n");
	return 0;
}
