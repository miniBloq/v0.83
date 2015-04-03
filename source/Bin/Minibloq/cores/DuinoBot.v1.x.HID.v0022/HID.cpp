/*
             LUFA Library
     Copyright (C) Dean Camera, 2011.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2011  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  Main source file for the GenericHID demo. This file contains the main tasks of the demo and
 *  is responsible for the initial application hardware configuration.
 */

#include "HID.h"
#include "wd.h"

#include "VirtualSerial.h"

#define TX_BUFFER_SIZE 128

struct ring_buffer {
  unsigned char buffer[TX_BUFFER_SIZE];
  int head;
  int tail;
};
ring_buffer tx_buffer_cdc = { { 0 }, 0, 0 };

inline void store_char(unsigned char c, ring_buffer *rx_buffer)
{
  int i = (rx_buffer->head + 1) % TX_BUFFER_SIZE;

  // if we should be storing the received character into the location
  // just before the tail (meaning that the head would advance to the
  // current location of the tail), we're about to overflow the buffer
  // and so we don't write the character or advance the head.
  if (i != rx_buffer->tail) {
    rx_buffer->buffer[rx_buffer->head] = c;
    rx_buffer->head = i;
  }
  
}

/** Type define for a non-returning pointer to the start of the loaded application in flash memory. */
typedef void (*AppPtr_t)(void) ATTR_NO_RETURN;

//===============================================================================
//	Interface
//===============================================================================

void HID_Send(uint8_t * Buffer){
	uint8_t i=0;

	while (Buffer[i]!='\0'){
		store_char(Buffer[i], &tx_buffer_cdc);
		i++;
	}
}


ISR(WDT_vect){
	int16_t ReceivedByte;

	// TODO: Hay que chequear si esta o no esta conectado
	if (1){
		HID_Task();
		USB_USBTask();
		/// Must throw away unused bytes from the host, or it will lock up while waiting for the device
		//ReceivedByte = CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);

		//if (!(ReceivedByte < 0)){
		//	Serial.store((uint8_t)ReceivedByte);
		//}

		//CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
		//USB_USBTask();
	}

	WD_SET(WD_IRQ,WDTO_15MS);

}

//===============================================================================
//	USB event handlers
//===============================================================================

/** Event handler for the USB_Connect event. This indicates that the device is enumerating via the status LEDs and
 *  starts the library USB task to begin the enumeration and USB management process.
 */
void EVENT_USB_Device_Connect(void)
{
	/* Indicate USB enumerating */
	//LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
}

/** Event handler for the USB_Disconnect event. This indicates that the device is no longer connected to a host via
 *  the status LEDs and stops the USB management task.
 */
void EVENT_USB_Device_Disconnect(void)
{
	/* Indicate USB not ready */
	//LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

/** Event handler for the USB_ConfigurationChanged event. This is fired when the host sets the current configuration
 *  of the USB device after enumeration, and configures the generic HID device endpoints.
 */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	/* Setup HID Report Endpoints */
	ConfigSuccess &= Endpoint_ConfigureEndpoint(GENERIC_IN_EPNUM, EP_TYPE_INTERRUPT, ENDPOINT_DIR_IN,
	                                            GENERIC_EPSIZE, ENDPOINT_BANK_SINGLE);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(GENERIC_OUT_EPNUM, EP_TYPE_INTERRUPT, ENDPOINT_DIR_OUT,
	                                            GENERIC_EPSIZE, ENDPOINT_BANK_SINGLE);

	/* Indicate endpoint configuration success or failure */
	//LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
	/*
	if (ConfigSuccess){
		LED_PORT |= LED_PIN;
	}else{
		while(1){
		LED_PORT |= LED_PIN;
		_delay_ms(100);
		LED_PORT &= ~LED_PIN;
		_delay_ms(100);
		}
	}
	*/
}

/** Event handler for the USB_ControlRequest event. This is used to catch and process control requests sent to
 *  the device from the USB host before passing along unhandled control requests to the library for processing
 *  internally.
 */
void EVENT_USB_Device_ControlRequest(void)
{
	/* Handle HID Class specific requests */
	switch (USB_ControlRequest.bRequest)
	{
		case HID_REQ_GetReport:
			if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				uint8_t GenericData[GENERIC_REPORT_SIZE];
				CreateGenericHIDReport(GenericData);

				Endpoint_ClearSETUP();

				/* Write the report data to the control endpoint */
				Endpoint_Write_Control_Stream_LE(&GenericData, sizeof(GenericData));
				Endpoint_ClearOUT();
			}

			break;
		case HID_REQ_SetReport:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				uint8_t GenericData[GENERIC_REPORT_SIZE];

				Endpoint_ClearSETUP();

				/* Read the report data from the control endpoint */
				Endpoint_Read_Control_Stream_LE(&GenericData, sizeof(GenericData));
				Endpoint_ClearIN();

				ProcessGenericHIDReport(GenericData);
			}

			break;
	}
}

/** Function to process the lest received report from the host.
 *
 *  \param[in] DataArray  Pointer to a buffer where the last report data is stored
 */
void ProcessGenericHIDReport(uint8_t* DataArray)
{
	/*
		This is where you need to process the reports being sent from the host to the device.
		DataArray is an array holding the last report from the host. This function is called
		each time the host has sent a report to the device.
	*/
	switch (DataArray[0]){
		case 0xA1:
			// Control block received
			switch (DataArray[1]){
				case 0x01:
					// Reset commmand
							cli();
							WD_SET(WD_RST,WDTO_15MS);
							while(1);
					break;
				case 0x02:
					// Battery level
					break;
			}
			break;
		case 0xB1:
			// Serial data received
			Serial.store(DataArray[1]);
			break;
		default:
			// Unhandled block received
			break;
	}
}

/** Function to create the next report to send back to the host at the next reporting interval.
 *
 *  \param[out] DataArray  Pointer to a buffer where the next report data should be stored
 */
void CreateGenericHIDReport(uint8_t* DataArray)
{
  // if the head isn't ahead of the tail, we don't have any characters
	
	DataArray[0] = 0xB1;
	int i;
	ring_buffer * _tx_buffer =  &tx_buffer_cdc;
	
	for (i=1;i<8;i++){
	  if (_tx_buffer->head == _tx_buffer->tail) {
			DataArray[i] = '\0';
			break;
		} else {
			DataArray[i] = _tx_buffer->buffer[_tx_buffer->tail];
			_tx_buffer->tail = (_tx_buffer->tail + 1) % TX_BUFFER_SIZE;
		}
	}
	
}


volatile uint8_t dataReceived=0;

void HID_Task(void)
{

	ring_buffer * _tx_buffer =  &tx_buffer_cdc;

	/* Device must be connected and configured for the task to run */
	//if (USB_DeviceState != DEVICE_STATE_Configured)
	//  return;

	Endpoint_SelectEndpoint(GENERIC_OUT_EPNUM);

	/* Check to see if a packet has been sent from the host */
	if (Endpoint_IsOUTReceived())
	{
		/* Check to see if the packet contains data */
		if (Endpoint_IsReadWriteAllowed())
		{
			/* Create a temporary buffer to hold the read in report from the host */
			uint8_t GenericData[GENERIC_REPORT_SIZE];

			/* Read Generic Report Data */
			Endpoint_Read_Stream_LE(&GenericData, sizeof(GenericData), NULL);

			/* Process Generic Report Data */
			ProcessGenericHIDReport(GenericData);
			
			dataReceived = 1;
		}

		/* Finalize the stream transfer to send the last packet */
		Endpoint_ClearOUT();
	}

	Endpoint_SelectEndpoint(GENERIC_IN_EPNUM);

	/* Check to see if the host is ready to accept another packet */
	//if (Endpoint_IsINReady() && dataReceived!=0)
	
	// && (_tx_buffer->head != _tx_buffer->tail)
	if (Endpoint_IsINReady())
	{
		/* Create a temporary buffer to hold the report to send to the host */
		uint8_t GenericData[GENERIC_REPORT_SIZE];

		/* Create Generic Report Data */
		CreateGenericHIDReport(GenericData);

		/* Write Generic Report Data */
		Endpoint_Write_Stream_LE(&GenericData, sizeof(GenericData), NULL);

		/* Finalize the stream transfer to send the last packet */
		Endpoint_ClearIN();
		dataReceived = 0;
	}
	
}

