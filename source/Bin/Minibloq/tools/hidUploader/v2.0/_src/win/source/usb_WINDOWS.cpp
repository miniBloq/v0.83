
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include "usb_XXX.h"
/****************************************************************/
/*                                                              */
/*               USB Access - Microsoft WIN32                   */
/*                                                              */
/****************************************************************/

#if defined(OS_WINDOWS)

// http://msdn.microsoft.com/en-us/library/ms790932.aspx
#include <windows.h>
#include <setupapi.h>
#include <ddk/hidsdi.h>
#include <ddk/hidclass.h>

HANDLE open_usb_device(int vid, int pid)
{
	GUID guid;
	HDEVINFO info;
	DWORD index, required_size;
	SP_DEVICE_INTERFACE_DATA iface;
	SP_DEVICE_INTERFACE_DETAIL_DATA *details;
	HIDD_ATTRIBUTES attrib;
	HANDLE h;
	BOOL ret;

	HidD_GetHidGuid(&guid);
	info = SetupDiGetClassDevs(&guid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	if (info == INVALID_HANDLE_VALUE) return NULL;
	for (index=0; 1 ;index++) {
		iface.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		ret = SetupDiEnumDeviceInterfaces(info, NULL, &guid, index, &iface);
		if (!ret) {
			SetupDiDestroyDeviceInfoList(info);
			break;
		}
		SetupDiGetInterfaceDeviceDetail(info, &iface, NULL, 0, &required_size, NULL);
		details = (SP_DEVICE_INTERFACE_DETAIL_DATA *)malloc(required_size);
		if (details == NULL) continue;
		memset(details, 0, required_size);
		details->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		ret = SetupDiGetDeviceInterfaceDetail(info, &iface, details,
			required_size, NULL, NULL);
		if (!ret) {
			free(details);
			continue;
		}
		h = CreateFile(details->DevicePath, GENERIC_READ|GENERIC_WRITE,
			FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
			FILE_FLAG_OVERLAPPED, NULL);
		free(details);
		if (h == INVALID_HANDLE_VALUE) continue;
		attrib.Size = sizeof(HIDD_ATTRIBUTES);
		ret = HidD_GetAttributes(h, &attrib);
		if (!ret) {
			CloseHandle(h);
			continue;
		}
		if (attrib.VendorID != vid || attrib.ProductID != pid) {
			CloseHandle(h);
			continue;
		}
		SetupDiDestroyDeviceInfoList(info);
		return h;
	}
	return NULL;
}

int write_usb_device(HANDLE h, void *buf, unsigned int len, int timeout)
{
	static HANDLE event = NULL;
	unsigned char tmpbuf[1040];
	OVERLAPPED ov;
	DWORD n, r;

	if (len > sizeof(tmpbuf) - 1) return 0;
	if (event == NULL) {
		event = CreateEvent(NULL, TRUE, TRUE, NULL);
		if (!event) return 0;
	}
	ResetEvent(&event);
	memset(&ov, 0, sizeof(ov));
	ov.hEvent = event;
	tmpbuf[0] = 0;
	memcpy(tmpbuf + 1, buf, len);
	if (!WriteFile(h, tmpbuf, len + 1, NULL, &ov)) {
		if (GetLastError() != ERROR_IO_PENDING) return 0;
		r = WaitForSingleObject(event, timeout);
		if (r == WAIT_TIMEOUT) {
			CancelIo(h);
			return 0;
		}
		if (r != WAIT_OBJECT_0) return 0;
	}
	if (!GetOverlappedResult(h, &ov, &n, FALSE)) return 0;
	if (n <= 0) return 0;
	return 1;
}


//##:
//void print_win32_err(void)
//{
//        char buf[256];
//        DWORD err;
//
//        err = GetLastError();
//        FormatMessage(  FORMAT_MESSAGE_FROM_SYSTEM, NULL, err,
//                        0, buf, sizeof(buf), NULL);
//        printf("err %ld: %s\n", err, buf);
//}


static HANDLE win32_teensy_handle = NULL;

int teensy_open(int vid, int pid)
{
	teensy_close();
	win32_teensy_handle = open_usb_device(vid, pid);

	if (!win32_teensy_handle) return 0;
	return 1;
}

int teensy_write(void *buf, int len, double timeout)
{
	int r;
	if (!win32_teensy_handle) return 0;
	r = write_usb_device(win32_teensy_handle, buf, len, (int)(timeout * 1000.0));
	return r;
}

void teensy_close(void)
{
	if (!win32_teensy_handle) return;
	CloseHandle(win32_teensy_handle);
	win32_teensy_handle = NULL;
}

#endif

