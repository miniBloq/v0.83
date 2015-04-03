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
	fprintf(stderr, "Usage: hid_bootloader_cli -mmcu=<MCU> [-w] [-h] [-n] [-v] <file.hex>\n");
	fprintf(stderr, "\t-w : Wait for device to appear\n");
	fprintf(stderr, "\t-r : Use hard reboot if device not online\n");
	fprintf(stderr, "\t-n : No reboot after programming\n");
	fprintf(stderr, "\t-v : Verbose output\n");
	fprintf(stderr, "\n<MCU> = atmegaXXuY or at90usbXXXY");

	fprintf(stderr, "\nFor support and more information, please visit:\n");
	fprintf(stderr, "http://www.lufa-lub.org\n");

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
void parse_options(int argc, char **argv);

// options (from user via command line args)
int wait_for_device_to_appear = 0;
int hard_reboot_device = 0;
int reboot_after_programming = 1;
int verbose = 0;
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
			if (strcmp(arg, "-w") == 0) {
				wait_for_device_to_appear = 1;
			} else if (strcmp(arg, "-r") == 0) {
				hard_reboot_device = 1;
			} else if (strcmp(arg, "-n") == 0) {
				reboot_after_programming = 0;
			} else if (strcmp(arg, "-v") == 0) {
				verbose = 1;
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


/****************************************************************/
/*                                                              */
/*                       Main Program                           */
/*                                                              */
/****************************************************************/
int main(int argc, char **argv)
{
	unsigned char buf[260];
	int num, addr, r, first_block=1, waited=0;

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
	printf_verbose("Teensy Loader, Command Line, Version 2.0\n");

	// read the intel hex file
	// this is done first so any error is reported before using USB
	num = read_intel_hex(filename);
	if (num < 0) die("error reading intel hex file \"%s\"", filename);
	printf_verbose("Read \"%s\": %d bytes, %.1f%% usage\n",
		filename, num, (double)num / (double)code_size * 100.0);

    hard_reboot_device = 1;

	// open the USB device
	while (1) {
		if (teensy_open()) break;

		if (hard_reboot_device) {
			if (!hard_reboot()) die("Unable to find rebootor\n");
			printf_verbose("Hard Reboot performed\n");
			hard_reboot_device = 0; // only hard reboot once
			wait_for_device_to_appear = 1;
		}

		if (!wait_for_device_to_appear) die("Unable to open device\n");
		if (!waited) {
			printf_verbose("Waiting for Teensy device...\n");
			printf_verbose(" (hint: press the reset button)\n");
			waited = 1;
			fflush(stdout);
		}
		delay(0.25);
	}
	printf_verbose("Found HalfKay Bootloader\n");

	// if we waited for the device, read the hex file again
	// perhaps it changed while we were waiting?
	if (waited) {
		num = read_intel_hex(filename);
		if (num < 0) die("error reading intel hex file \"%s\"", filename);
		printf_verbose("Read \"%s\": %d bytes, %.1f%% usage\n",
		 	filename, num, (double)num / (double)code_size * 100.0);
	}

	// program the data
	printf_verbose("Programming");
	fflush(stdout);
	for (addr = 0; addr < code_size; addr += block_size) {
		if (addr > 0 && !ihex_bytes_within_range(addr, addr + block_size - 1)) {
			// don't waste time on blocks that are unused,
			// but always do the first one to erase the chip
			continue;
		}
		printf_verbose(".");
		if (code_size < 0x10000) {
			buf[0] = addr & 255;
			buf[1] = (addr >> 8) & 255;
		} else {
			buf[0] = (addr >> 8) & 255;
			buf[1] = (addr >> 16) & 255;
		}
		ihex_get_data(addr, block_size, buf + 2);
		r = teensy_write(buf, block_size + 2, first_block ? 3.0 : 0.25);
		if (!r) die("Error writing to device\n");
		first_block = 0;
	}
	printf_verbose("\n");

	// reboot to the user's new code
	if (reboot_after_programming) {
		printf_verbose("Booting\n");
		buf[0] = 0xFF;
		buf[1] = 0xFF;
		memset(buf + 2, 0, sizeof(buf) - 2);
		teensy_write(buf, block_size + 2, 0.25);
	}
	teensy_close();

	die("\nDone.\n");
	return 0;
}
