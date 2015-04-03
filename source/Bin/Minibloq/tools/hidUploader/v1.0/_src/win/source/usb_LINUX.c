/****************************************************************/
/*                                                              */
/*             USB Access - libusb (Linux & FreeBSD)            */
/*                                                              */
/****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

#include "usb_XXX.h"


// http://libusb.sourceforge.net/doc/index.html
#include <usb.h>

usb_dev_handle * open_usb_device(int vid, int pid)
{
	struct usb_bus *bus;
	struct usb_device *dev;
	usb_dev_handle *h;
	char buf[128];
	int r;

	usb_init();
	usb_find_busses();
	usb_find_devices();
	//printf_verbose("\nSearching for USB device:\n");
	for (bus = usb_get_busses(); bus; bus = bus->next) {
		for (dev = bus->devices; dev; dev = dev->next) {
			//printf_verbose("bus \"%s\", device \"%s\" vid=%04X, pid=%04X\n",
			//	bus->dirname, dev->filename,
			//	dev->descriptor.idVendor,
			//	dev->descriptor.idProduct
			//);
			if (dev->descriptor.idVendor != vid) continue;
			if (dev->descriptor.idProduct != pid) continue;
			h = usb_open(dev);
			if (!h) {
				printf_verbose("Found device but unable to open");
				continue;
			}
			#ifdef LIBUSB_HAS_GET_DRIVER_NP
			r = usb_get_driver_np(h, 0, buf, sizeof(buf));
			if (r >= 0) {
				r = usb_detach_kernel_driver_np(h, 0);
				if (r < 0) {
					usb_close(h);
					printf_verbose("Device is in use by \"%s\" driver", buf);
					continue;
				}
			}
			#endif
			// Mac OS-X - removing this call to usb_claim_interface() might allow
			// this to work, even though it is a clear misuse of the libusb API.
			// normally Apple's IOKit should be used on Mac OS-X
			r = usb_claim_interface(h, 0);
			if (r < 0) {
				usb_close(h);
				printf_verbose("Unable to claim interface, check USB permissions");
				continue;
			}
			return h;
		}
	}
	return NULL;
}

static usb_dev_handle *libusb_teensy_handle = NULL;

int teensy_open(void)
{
	teensy_close();
	libusb_teensy_handle = open_usb_device(0x16C0, 0x0478);

	if (!libusb_teensy_handle)
		libusb_teensy_handle = open_usb_device(0x03eb, 0x2067);

	if (!libusb_teensy_handle) return 0;
	return 1;
}

int teensy_write(void *buf, int len, double timeout)
{
	int r;

	if (!libusb_teensy_handle) return 0;
	r = usb_control_msg(libusb_teensy_handle, 0x21, 9, 0x0200, 0, (char *)buf,
		len, (int)(timeout * 1000.0));
	if (r < 0) return 0;
	return 1;
}

void teensy_close(void)
{
	if (!libusb_teensy_handle) return;
	usb_release_interface(libusb_teensy_handle, 0);
	usb_close(libusb_teensy_handle);
	libusb_teensy_handle = NULL;
}

int hard_reboot(void)
{
	usb_dev_handle *rebootor;
	int r;

	rebootor = open_usb_device(0x16C0, 0x0477);

	if (!rebootor)
		rebootor = open_usb_device(0x03eb, 0x2067);

	if (!rebootor) return 0;
	r = usb_control_msg(rebootor, 0x21, 9, 0x0200, 0, "reboot", 6, 100);
	usb_release_interface(rebootor, 0);
	usb_close(rebootor);
	if (r < 0) return 0;
	return 1;
}

