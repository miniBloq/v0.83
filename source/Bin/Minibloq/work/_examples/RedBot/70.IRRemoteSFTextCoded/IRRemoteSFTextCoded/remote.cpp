/* RGB Remote Control
   by: Jim Lindblom
   SparkFun Electronics
   date: October 1, 2013

   This sketch uses Ken Shirriff's *awesome* IRremote library:
       https://github.com/shirriff/Arduino-IRremote

   RGB Remote Control uses a combination of SparkFun's 
   IR Remote (https://www.sparkfun.com/products/11759) and an
   IR receiver diode (https://www.sparkfun.com/products/10266) to
   control an RGB LED.

   The IR Remote's power button turns the LED on or off. The A, B, 
   and C buttons select a channel (red, green, or blue). The up
   and down arrows increment or decrement the LED brightness on that channel.
   The left and right arrows turn a channel to min or max, and
   circle set it to the middle.

   Hardware setup:
     * The output of an IR Receiver Diode (38 kHz demodulating
       version) should be connected to the Arduino's pin 11.
       * The IR Receiver diode should also be powered off the
         Arduino's 5V and GND rails.
     * A common cathode RGB LED is connected to Arduino's pins 
       5, 9, and 6 (red, green, and blue pins).
 */

#include <IRremoteSF.h> // Include the IRremote library
#include <mbq.h> // Include the IRremote library

/* Setup constants for SparkFun's IR Remote: */
#define NUM_BUTTONS 9 // The remote has 9 buttons

/* Define the IR remote button codes. We're only using the
   least signinficant two bytes of these codes. Each one 
   should actually have 0x10EF in front of it. Find these codes
   by running the IRrecvDump example sketch included with
   the IRremote library.*/
const uint16_t BUTTON_POWER = 0xD827; // i.e. 0x10EFD827
const uint16_t BUTTON_A = 0xF807;
const uint16_t BUTTON_B = 0x7887;
const uint16_t BUTTON_C = 0x58A7;
const uint16_t BUTTON_UP = 0xA05F;
const uint16_t BUTTON_DOWN = 0x00FF;
const uint16_t BUTTON_LEFT = 0x10EF;
const uint16_t BUTTON_RIGHT = 0x807F;
const uint16_t BUTTON_CIRCLE = 0x20DF;

IRrecvSF irReceiverSF(11);

decode_resultsSF results; // This will store our IR received codes
uint16_t lastCode = 0; // This keeps track of the last code RX'd

void mainLoop();

void go()
{
	serial0.begin(115200);
	irReceiverSF.enableIRIn();
	while(true)
	{
		mainLoop();
	}
}

void mainLoop()
{
  if (irReceiverSF.decode(&results)) 
  {
    /* read the RX'd IR into a 16-bit variable: */
    uint16_t resultCode = (results.value & 0xFFFF);

    /* The remote will continue to spit out 0xFFFFFFFF if a 
     button is held down. If we get 0xFFFFFFF, let's just
     assume the previously pressed button is being held down */
    if (resultCode == 0xFFFF)
      resultCode = lastCode;
    else
      lastCode = resultCode;

    // This switch statement checks the received IR code against
    // all of the known codes. Each button press produces a 
    // serial output, and has an effect on the LED output.
    switch (resultCode)
    {
      case BUTTON_POWER:
        Serial.println("Power");
        break;
      case BUTTON_A:
        Serial.println("A");
        break;
      case BUTTON_B:
        Serial.println("B");
        break;
      case BUTTON_C:
        Serial.println("C");
        break;
      case BUTTON_UP:
        Serial.println("Up");
        break;
      case BUTTON_DOWN:
        Serial.println("Down");
        break;
      case BUTTON_LEFT:
        Serial.println("Left");
        break;
      case BUTTON_RIGHT:
        Serial.println("Right");
        break;
      case BUTTON_CIRCLE:
        Serial.println("Circle");
        break;
      default:
        Serial.print("Unrecognized code received: 0x");
        Serial.println(results.value, HEX);
        break;        
    }    
    irReceiverSF.resume(); // Receive the next value
  }
}