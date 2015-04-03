
#include "VirtualSerial.h"
#include "CDCSerial.h"
#include "wd.h"

#include "wiring_private.h"

/** LUFA CDC Class driver interface configuration and state information. This structure is
 *  passed to all CDC Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */


#define _RESET_ON_FALLING_DTR	1

/*
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
	{
		.Config =
			{
				.ControlInterfaceNumber         = 0,

				.DataINEndpointNumber           = CDC_TX_EPNUM,
				.DataINEndpointSize             = CDC_TXRX_EPSIZE,
				.DataINEndpointDoubleBank       = false,

				.DataOUTEndpointNumber          = CDC_RX_EPNUM,
				.DataOUTEndpointSize            = CDC_TXRX_EPSIZE,
				.DataOUTEndpointDoubleBank      = false,

				.NotificationEndpointNumber     = CDC_NOTIFICATION_EPNUM,
				.NotificationEndpointSize       = CDC_NOTIFICATION_EPSIZE,
				.NotificationEndpointDoubleBank = false,
			},
	};
*/

// The cpp compiler wont let me initialize the struct as above so this is the simplified version
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
	{
			{0,CDC_TX_EPNUM,CDC_TXRX_EPSIZE,false,CDC_RX_EPNUM, CDC_TXRX_EPSIZE, false, CDC_NOTIFICATION_EPNUM, CDC_NOTIFICATION_EPSIZE, false},

	};

volatile int DTR_old = -1;
volatile int DTR = -1;


/** Standard file stream for the CDC interface when set up, so that the virtual CDC COM port can be
 *  used like any regular character stream in the C APIs
 */
//static FILE USBSerialStream;

/** Type define for a non-returning pointer to the start of the loaded application in flash memory. */
typedef void (*AppPtr_t)(void) ATTR_NO_RETURN;

//===============================================================================
//	Interface
//===============================================================================

void CDC_Send(uint8_t * Buffer){
	uint8_t i=0;

	while (Buffer[i]!=0){
		CDC_Device_SendByte(&VirtualSerial_CDC_Interface, Buffer[i]);
		i++;
	}
}


ISR(WDT_vect){
	int16_t ReceivedByte;

	// TODO: Hay que chequear si esta o no está conectado
	if (true){
		/// Must throw away unused bytes from the host, or it will lock up while waiting for the device
		ReceivedByte = CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);

		if (!(ReceivedByte < 0)){
			Serial.store((uint8_t)ReceivedByte);
		}

		CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
		USB_USBTask();
	}

	WD_SET(WD_IRQ,WDTO_15MS);

}

//====================================================================================
//																USB Event handlers
//====================================================================================

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
	//LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
	//LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	ConfigSuccess &= CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);

//	LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

/** Event handler for the library USB Unhandled Control Request event. */
void EVENT_USB_Device_UnhandledControlRequest(void)
{
	CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}


void EVENT_CDC_Device_ControLineStateChanged(USB_ClassInfo_CDC_Device_t * const DCInterfaceInfo){

	DTR_old = DTR;

	if (DCInterfaceInfo->State.ControlLineStates.HostToDevice & CDC_CONTROL_LINE_OUT_DTR){
		DTR = 1;
	}else{
		DTR = 0;
	}

	if ((DTR == 0) && (DTR_old == 1)){
		// Falling Edge of DTR signal

		// We reset the board
		// The original idea was to perform a softreset with a jump, but there are some problems in the bootloader if so.
		// Now a hard reset via WDT is being used.

#ifdef _RESET_ON_FALLING_DTR
		cli();
		WD_SET(WD_RST,WDTO_15MS);

		while(1){
			;
		}
#endif

	} else if ((DTR==1) && (DTR_old == 0)){
		// Rising edge of DTR signal

		// This event occurs when opening the serial port in the pc.
		// This event will be used to reset the user application only

		GPIOR0 = 0xDC;
		GPIOR1 = 0xA7;

		AppPtr_t AppStartPtr = (AppPtr_t)0x0000;
		AppStartPtr();

	}

}
