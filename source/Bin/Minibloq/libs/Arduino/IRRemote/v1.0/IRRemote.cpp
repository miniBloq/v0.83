/*
 * IRRemote
 * Version 0.11 August, 2009
 * Copyright 2009 Ken Shirriff
 * For details, see http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.html
 *
 * Interrupt code based on NECIRrcv by Joe Knapp
 * http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1210243556
 * Also influenced by http://zovirl.com/2008/11/12/building-a-universal-remote-with-an-arduino/
 */

#include "IRRemote.h"
#include "IRRemoteInt.h"

// Provides ISR
#include <avr/interrupt.h>

volatile irparams_t irparams;
volatile int IrOut = 0;
volatile int blink = 1;

//##2011.07.31: Send not used by now (in the future will be probably added support for this):
#if 0
void IRsend::sendNEC(unsigned long data, int nbits)
{
  enableIROut(38);
  mark(NEC_HDR_MARK);
  space(NEC_HDR_SPACE);
  for (int i = 0; i < nbits; i++) {
    if (data & TOPBIT) {
      mark(NEC_BIT_MARK);
      space(NEC_ONE_SPACE);
    }
    else {
      mark(NEC_BIT_MARK);
      space(NEC_ZERO_SPACE);
    }
    data <<= 1;
  }
  mark(NEC_BIT_MARK);
  space(0);
  digitalWrite(15, LOW); // When not sending PWM, we want it low
}

void IRsend::sendSony(unsigned long data, int nbits) {
  enableIROut(40);
  mark(SONY_HDR_MARK);
  space(SONY_HDR_SPACE);
  data = data << (32 - nbits);
  for (int i = 0; i < nbits; i++) {
    if (data & TOPBIT) {
      mark(SONY_ONE_MARK);
      space(SONY_HDR_SPACE);
    }
    else {
      mark(SONY_ZERO_MARK);
      space(SONY_HDR_SPACE);
    }
    data <<= 1;
  }
}

void IRsend::sendRaw(unsigned int buf[], int len, int hz)
{
  enableIROut(hz);
  for (int i = 0; i < len; i++) {
    if (i & 1) {
      space(buf[i]);
    }
    else {
      mark(buf[i]);
    }
  }
  space(0); // Just to be sure
}

// Note: first bit must be a one (start bit)
void IRsend::sendRC5(unsigned long data, int nbits)
{
  enableIROut(36);
  data = data << (32 - nbits);

  mark(RC5_T1); // First start bit
  space(RC5_T1); // Second start bit
  mark(RC5_T1); // Second start bit

  for (int i = 0; i < nbits; i++) {
    if (data & TOPBIT) {
      space(RC5_T1); // 1 is space, then mark
      mark(RC5_T1);
    }
    else {
      mark(RC5_T1);
      space(RC5_T1);
    }
    data <<= 1;
  }
  space(0); // Turn off at end
}

// Caller needs to take care of flipping the toggle bit
void IRsend::sendRC6(unsigned long data, int nbits)
{
  enableIROut(36);
  data = data << (32 - nbits);
  mark(RC6_HDR_MARK);
  space(RC6_HDR_SPACE);
  mark(RC6_T1); // start bit
  space(RC6_T1);
  int t;
  for (int i = 0; i < nbits; i++) {
    if (i == 3) {
      // double-wide trailer bit
      t = 2 * RC6_T1;
    }
    else {
      t = RC6_T1;
    }
    if (data & TOPBIT) {
      mark(t);
      space(t);
    }
    else {
      space(t);
      mark(t);
    }

    data <<= 1;
  }
  space(0); // Turn off at end
}

void IRsend::mark(int time) {
  // Sends an IR mark for the specified number of microseconds.
  // The mark output is modulated at the PWM frequency.
  //TCCR1A |= _BV(COM2B1); // Enable pin 3 PWM output
  //IrOut = 1;
  PORTF |= _BV(7);
  TIMSK3 |= _BV(OCIE3A);

  //delayMicroseconds(time-80);
  //delayMicroseconds(time-40);
  delayMicroseconds(time);
}


/* Leave pin off for time (given in microseconds) */

void IRsend::space(int time) {
  // Sends an IR space for the specified number of microseconds.
  // A space is no output, so the PWM output is disabled.
  //TCCR1A &= ~(_BV(COM2B1)); // Disable pin 3 PWM output
  //IrOut = 0;
  TIMSK3 &=~_BV(OCIE3A);
  PORTF &= ~_BV(7);
  delayMicroseconds(time);
  delayMicroseconds(time);
  //delayMicroseconds(time-40);
  //delayMicroseconds(time);
}

void IRsend::enableIROut(int khz) {

	uint16_t temp;
	uint16_t tmpkhz;
	tmpkhz = khz *1000;
	// Configure Timer 3 in CTC Mode

	// WGM33 = 0
	// WGM32 = 1
	// WGM31 = 0
	// WGM30 = 0

	// Configure Timer 3 Clock Source 001 = Clk / 1
	// CS32 = 0
	// CS31 = 0
	// CS30 = 1
	pinMode(14, OUTPUT); // ## esto??

	TCCR3A = 0b00000000;
	TCCR3B = 0b00001001;

	//temp = (F_CPU / (khz*1000))/2;
	temp = (F_CPU / tmpkhz)/2;

    OCR3A = (uint16_t)temp;

	TIMSK3 |= _BV(OCIE3A);
/*
  // Enables IR output.  The khz value controls the modulation frequency in kilohertz.
  // The IR output will be on pin 3 (OC2B).
  // This routine is designed for 36-40KHz; if you use it for other values, it's up to you
  // to make sure it gives reasonable results.  (Watch out for overflow / underflow / rounding.)
  // TIMER2 is used in phase-correct PWM mode, with OCR2A controlling the frequency and OCR2B
  // controlling the duty cycle.
  // There is no prescaling, so the output frequency is 16MHz / (2 * OCR2A)
  // To turn the output on and off, we leave the PWM running, but connect and disconnect the output pin.
  // A few hours staring at the ATmega documentation and this will all make sense.
  // See my Secrets of Arduino PWM at http://arcfn.com/2009/07/secrets-of-arduino-pwm.html for details.


  // Disable the Timer2 Interrupt (which is used for receiving IR)
  //TIMSK2 &= ~_BV(TOIE2); //Timer2 Overflow Interrupt
  cbi(TIMSK1,TOIE1);

  pinMode(3, OUTPUT);
  digitalWrite(3, LOW); // When not sending PWM, we want it low

  // COM2A = 00: disconnect OC2A
  // COM2B = 00: disconnect OC2B; to send signal set to 10: OC2B non-inverted
  // WGM2 = 101: phase-correct PWM with OCRA as top
  // CS2 = 000: no prescaling
  TCCR1A = _BV(WGM20);
  TCCR1B = _BV(WGM22) | _BV(CS20);

  // The top value for the timer.  The modulation frequency will be SYSCLOCK / 2 / OCR2A.
  OCR2A = SYSCLOCK / 2 / khz / 1000;
  OCR2B = OCR2A / 3; // 33% duty cycle
 */
 //==============================================================
 /*
	float freq = khz*1000; //[Hz]

//	float ocr1a = (9216000 / freq) - 1; // 9216000 = 18432000 / (2*1)
	//float ocr1a = (16000000 / freq) - 1; // 9216000 = 18432000 / (2*1)

	uint16_t temp;
	temp = (F_CPU / freq)/2;

	//TCCR1A = 0b11111100;
	//TCCR1B = 0b00001001;

	TCCR1A = 0b00000000;
	TCCR1B = 0b00001001;
	OCR1A = (uint16_t)temp;

	TIMSK1 |= _BV(OCIE1A);

	pinMode(14, OUTPUT);
	digitalWrite(14, LOW); // When not sending PWM, we want it low
*/
}

ISR(TIMER3_COMPA_vect)
{
	PORTF ^= _BV(7);
}
#endif


IRrecv::IRrecv(int recvpin) : enabled(false)
{
  irparams.recvpin = recvpin;
  irparams.blinkflag = 0;
}


////##2011.03.07: Method added for Minibloq:
//IRrecv::setPin(int recvpin)
//{
//  irparams.recvpin = recvpin;
//  irparams.blinkflag = 0;
//  enableIRIn();
//}


#if defined(__AVR_ATmega32U4__)
void IRrecv::disableIRIn() //##Minibloq.20111014.
{
	//Restores the timer 3:
	TCCR3A = _tccr3a;
	TCCR3B = _tccr3b;
	TIMSK3 = _timsk3;
	enabled = false;
}

// initialization
void IRrecv::enableIRIn() 
{
  _tccr3a = TCCR3A; //##Minibloq.20111014.
  _tccr3b = TCCR3B;
  _timsk3 = TIMSK3;  

  // setup pulse clock timer interrupt
  TCCR3A = 0;  // normal mode
  TCCR3B = 0;  // normal mode

  //Prescale /8 (16M/8 = 0.5 microseconds per tick)
  // Therefore, the timer interval can range from 0.5 to 128 microseconds
  // depending on the reset value (255 to 0)
  cbi(TCCR3B,CS32);
  sbi(TCCR3B,CS31);
  cbi(TCCR3B,CS30);
  /*
  cbi(TCCR1B,CS12);
  sbi(TCCR1B,CS11);
  cbi(TCCR1B,CS10);
*/
  //Timer2 Overflow Interrupt Enable
  sbi(TIMSK3,TOIE3);

  RESET_TIMER3;

  sei();  // enable interrupts

  // initialize state machine variables
  irparams.rcvstate = STATE_IDLE;
  irparams.rawlen = 0;

  // set pin modes
  pinMode(irparams.recvpin, INPUT);
  
  enabled = true; //##Minibloq
}


// TIMER2 interrupt code to collect raw data.
// Widths of alternating SPACE, MARK are recorded in rawbuf.
// Recorded in ticks of 50 microseconds.
// rawlen counts the number of entries recorded so far.
// First entry is the SPACE between transmissions.
// As soon as a SPACE gets long, ready is set, state switches to IDLE, timing of SPACE continues.
// As soon as first MARK arrives, gap width is recorded, ready is cleared, and new logging starts

SIGNAL(TIMER3_OVF_vect)
{
/*	static int bl = 1;
	 RESET_TIMER1;

	if (bl==1){
		bl = 0;
		digitalWrite(13, HIGH);
	}else{
		bl = 1;
		digitalWrite(13, LOW);
	}*/

  RESET_TIMER3;

  uint8_t irdata = (uint8_t)digitalRead(irparams.recvpin);

  irparams.timer++; // One more 50us tick
  if (irparams.rawlen >= RAWBUF) {
    // Buffer overflow
    irparams.rcvstate = STATE_STOP;
  }

  switch(irparams.rcvstate) {
	  case STATE_IDLE: // In the middle of a gap
		if (irdata == MARK) {
		  if (irparams.timer < GAP_TICKS) {
			// Not big enough to be a gap.
			irparams.timer = 0;
		  }
		  else {
			// gap just ended, record duration and start recording transmission
			irparams.rawlen = 0;
			irparams.rawbuf[irparams.rawlen++] = irparams.timer;
			irparams.timer = 0;
			irparams.rcvstate = STATE_MARK;
		  }
		}
		break;
	  case STATE_MARK: // timing MARK
		if (irdata == SPACE) {   // MARK ended, record time
		  irparams.rawbuf[irparams.rawlen++] = irparams.timer;
		  irparams.timer = 0;
		  irparams.rcvstate = STATE_SPACE;
		}
		break;
	  case STATE_SPACE: // timing SPACE
		if (irdata == MARK) { // SPACE just ended, record it
		  irparams.rawbuf[irparams.rawlen++] = irparams.timer;
		  irparams.timer = 0;
		  irparams.rcvstate = STATE_MARK;
		}
		else { // SPACE
		  if (irparams.timer > GAP_TICKS) {
			// big SPACE, indicates gap between codes
			// Mark current code as ready for processing
			// Switch to STOP
			// Don't reset timer; keep counting space width
			irparams.rcvstate = STATE_STOP;
		  }
		}
		break;
	  case STATE_STOP: // waiting, measuring gap
		if (irdata == MARK) { // reset gap timer
		  irparams.timer = 0;
		}
		break;
  }

  if (irparams.blinkflag) {
    if (irdata == MARK) {
      PORTB |= B00100000;  // turn pin 13 LED on
    }
    else {
      PORTB &= B11011111;  // turn pin 13 LED off
    }
  }
}
#else
void IRrecv::disableIRIn() //##Minibloq.20111014.
{
	//Restores the timer 3:
	TCCR1A = _tccr3a;
	TCCR1B = _tccr3b;
	TIMSK1 = _timsk3;
	enabled = false;
}

// initialization
void IRrecv::enableIRIn() 
{
  _tccr3a = TCCR1A; //##Minibloq.20111014.
  _tccr3b = TCCR1B;
  _timsk3 = TIMSK1;  

  // setup pulse clock timer interrupt
  TCCR1A = 0;  // normal mode
  TCCR1B = 0;  // normal mode

  //Prescale /8 (16M/8 = 0.5 microseconds per tick)
  // Therefore, the timer interval can range from 0.5 to 128 microseconds
  // depending on the reset value (255 to 0)
  cbi(TCCR1B,CS12);
  sbi(TCCR1B,CS11);
  cbi(TCCR1B,CS10);
  /*
  cbi(TCCR1B,CS12);
  sbi(TCCR1B,CS11);
  cbi(TCCR1B,CS10);
*/
  //Timer2 Overflow Interrupt Enable
  sbi(TIMSK1,TOIE1);

  RESET_TIMER1;

  sei();  // enable interrupts

  // initialize state machine variables
  irparams.rcvstate = STATE_IDLE;
  irparams.rawlen = 0;

  // set pin modes
  pinMode(irparams.recvpin, INPUT);
  
  enabled = true; //##Minibloq
}

// TIMER2 interrupt code to collect raw data.
// Widths of alternating SPACE, MARK are recorded in rawbuf.
// Recorded in ticks of 50 microseconds.
// rawlen counts the number of entries recorded so far.
// First entry is the SPACE between transmissions.
// As soon as a SPACE gets long, ready is set, state switches to IDLE, timing of SPACE continues.
// As soon as first MARK arrives, gap width is recorded, ready is cleared, and new logging starts

SIGNAL(TIMER1_OVF_vect)
{
/*	static int bl = 1;
	 RESET_TIMER1;

	if (bl==1){
		bl = 0;
		digitalWrite(13, HIGH);
	}else{
		bl = 1;
		digitalWrite(13, LOW);
	}*/

  RESET_TIMER1;

  uint8_t irdata = (uint8_t)digitalRead(irparams.recvpin);

  irparams.timer++; // One more 50us tick
  if (irparams.rawlen >= RAWBUF) {
    // Buffer overflow
    irparams.rcvstate = STATE_STOP;
  }

  switch(irparams.rcvstate) {
	  case STATE_IDLE: // In the middle of a gap
		if (irdata == MARK) {
		  if (irparams.timer < GAP_TICKS) {
			// Not big enough to be a gap.
			irparams.timer = 0;
		  }
		  else {
			// gap just ended, record duration and start recording transmission
			irparams.rawlen = 0;
			irparams.rawbuf[irparams.rawlen++] = irparams.timer;
			irparams.timer = 0;
			irparams.rcvstate = STATE_MARK;
		  }
		}
		break;
	  case STATE_MARK: // timing MARK
		if (irdata == SPACE) {   // MARK ended, record time
		  irparams.rawbuf[irparams.rawlen++] = irparams.timer;
		  irparams.timer = 0;
		  irparams.rcvstate = STATE_SPACE;
		}
		break;
	  case STATE_SPACE: // timing SPACE
		if (irdata == MARK) { // SPACE just ended, record it
		  irparams.rawbuf[irparams.rawlen++] = irparams.timer;
		  irparams.timer = 0;
		  irparams.rcvstate = STATE_MARK;
		}
		else { // SPACE
		  if (irparams.timer > GAP_TICKS) {
			// big SPACE, indicates gap between codes
			// Mark current code as ready for processing
			// Switch to STOP
			// Don't reset timer; keep counting space width
			irparams.rcvstate = STATE_STOP;
		  }
		}
		break;
	  case STATE_STOP: // waiting, measuring gap
		if (irdata == MARK) { // reset gap timer
		  irparams.timer = 0;
		}
		break;
  }

  if (irparams.blinkflag) {
    if (irdata == MARK) {
      PORTB |= B00100000;  // turn pin 13 LED on
    }
    else {
      PORTB &= B11011111;  // turn pin 13 LED off
    }
  }
}
#endif


// enable/disable blinking of pin 13 on IR processing
void IRrecv::blink13(int blinkflag)
{
  irparams.blinkflag = blinkflag;
  if (blinkflag)
    pinMode(BLINKLED, OUTPUT);
}


void IRrecv::resume() {
  irparams.rcvstate = STATE_IDLE;
  irparams.rawlen = 0;
}

// Decodes the received IR message
// Returns 0 if no data ready, 1 if data ready.
// Results of decoding are stored in results
int IRrecv::decode(decode_results *results) {
  results->rawbuf = irparams.rawbuf;
  results->rawlen = irparams.rawlen;
  if (irparams.rcvstate != STATE_STOP) {
    return ERR;
  }
#ifdef DEBUG
  Serial.println("Attempting NEC decode");
#endif
  if (decodeNEC(results)) {
    return DECODED;
  }
#ifdef DEBUG
  Serial.println("Attempting Sony decode");
#endif
  if (decodeSony(results)) {
    return DECODED;
  }
#ifdef DEBUG
  Serial.println("Attempting RC5 decode");
#endif
  if (decodeRC5(results)) {
    return DECODED;
  }
#ifdef DEBUG
  Serial.println("Attempting RC6 decode");
#endif
  if (decodeRC6(results)) {
    return DECODED;
  }
  if (results->rawlen >= 6) {
    // Only return raw buffer if at least 6 bits
    results->decode_type = UNKNOWN;
    results->bits = 0;
    results->value = 0;
    return DECODED;
  }
  // Throw away and start over
  resume();
  return ERR;
}

long IRrecv::decodeNEC(decode_results *results) {
  long data = 0;
  int offset = 1; // Skip first space
  // Initial mark
  if (!MATCH_MARK(results->rawbuf[offset], NEC_HDR_MARK)) {
    return ERR;
  }
  offset++;
  // Check for repeat
  if (irparams.rawlen == 4 &&
    MATCH_SPACE(results->rawbuf[offset], NEC_RPT_SPACE) &&
    MATCH_MARK(results->rawbuf[offset+1], NEC_BIT_MARK)) {
    results->bits = 0;
    results->value = REPEAT;
    results->decode_type = NEC;
    return DECODED;
  }
  if (irparams.rawlen < 2 * NEC_BITS + 4) {
    return ERR;
  }
  // Initial space
  if (!MATCH_SPACE(results->rawbuf[offset], NEC_HDR_SPACE)) {
    return ERR;
  }
  offset++;
  for (int i = 0; i < NEC_BITS; i++) {
    if (!MATCH_MARK(results->rawbuf[offset], NEC_BIT_MARK)) {
      return ERR;
    }
    offset++;
    if (MATCH_SPACE(results->rawbuf[offset], NEC_ONE_SPACE)) {
      data = (data << 1) | 1;
    }
    else if (MATCH_SPACE(results->rawbuf[offset], NEC_ZERO_SPACE)) {
      data <<= 1;
    }
    else {
      return ERR;
    }
    offset++;
  }
  // Success
  results->bits = NEC_BITS;
  results->value = data;
  results->decode_type = NEC;
  return DECODED;
}

long IRrecv::decodeSony(decode_results *results) {
  long data = 0;
  if (irparams.rawlen < 2 * SONY_BITS + 2) {
    return ERR;
  }
  int offset = 1; // Skip first space
  // Initial mark
  if (!MATCH_MARK(results->rawbuf[offset], SONY_HDR_MARK)) {
    return ERR;
  }
  offset++;

  while (offset + 1 < irparams.rawlen) {
    if (!MATCH_SPACE(results->rawbuf[offset], SONY_HDR_SPACE)) {
      break;
    }
    offset++;
    if (MATCH_MARK(results->rawbuf[offset], SONY_ONE_MARK)) {
      data = (data << 1) | 1;
    }
    else if (MATCH_MARK(results->rawbuf[offset], SONY_ZERO_MARK)) {
      data <<= 1;
    }
    else {
      return ERR;
    }
    offset++;
  }

  // Success
  results->bits = (offset - 1) / 2;
  if (results->bits < 12) {
    results->bits = 0;
    return ERR;
  }
  results->value = data;
  results->decode_type = SONY;
  return DECODED;
}

// Gets one undecoded level at a time from the raw buffer.
// The RC5/6 decoding is easier if the data is broken into time intervals.
// E.g. if the buffer has MARK for 2 time intervals and SPACE for 1,
// successive calls to getRClevel will return MARK, MARK, SPACE.
// offset and used are updated to keep track of the current position.
// t1 is the time interval for a single bit in microseconds.
// Returns -1 for error (measured time interval is not a multiple of t1).
int IRrecv::getRClevel(decode_results *results, int *offset, int *used, int t1) {
  if (*offset >= results->rawlen) {
    // After end of recorded buffer, assume SPACE.
    return SPACE;
  }
  int width = results->rawbuf[*offset];
  int val = ((*offset) % 2) ? MARK : SPACE;
  int correction = (val == MARK) ? MARK_EXCESS : - MARK_EXCESS;

  int avail;
  if (MATCH(width, t1 + correction)) {
    avail = 1;
  }
  else if (MATCH(width, 2*t1 + correction)) {
    avail = 2;
  }
  else if (MATCH(width, 3*t1 + correction)) {
    avail = 3;
  }
  else {
    return -1;
  }

  (*used)++;
  if (*used >= avail) {
    *used = 0;
    (*offset)++;
  }
#ifdef DEBUG
  if (val == MARK) {
    Serial.println("MARK");
  }
  else {
    Serial.println("SPACE");
  }
#endif
  return val;
}

long IRrecv::decodeRC5(decode_results *results) {
  if (irparams.rawlen < MIN_RC5_SAMPLES + 2) {
    return ERR;
  }
  int offset = 1; // Skip gap space
  long data = 0;
  int used = 0;
  // Get start bits
  if (getRClevel(results, &offset, &used, RC5_T1) != MARK) return ERR;
  if (getRClevel(results, &offset, &used, RC5_T1) != SPACE) return ERR;
  if (getRClevel(results, &offset, &used, RC5_T1) != MARK) return ERR;
  int nbits;
  for (nbits = 0; offset < irparams.rawlen; nbits++) {
    int levelA = getRClevel(results, &offset, &used, RC5_T1);
    int levelB = getRClevel(results, &offset, &used, RC5_T1);
    if (levelA == SPACE && levelB == MARK) {
      // 1 bit
      data = (data << 1) | 1;
    }
    else if (levelA == MARK && levelB == SPACE) {
      // zero bit
      data <<= 1;
    }
    else {
      return ERR;
    }
  }

  // Success
  results->bits = nbits;
  results->value = data;
  results->decode_type = RC5;
  return DECODED;
}

long IRrecv::decodeRC6(decode_results *results) {
  if (results->rawlen < MIN_RC6_SAMPLES) {
    return ERR;
  }
  int offset = 1; // Skip first space
  // Initial mark
  if (!MATCH_MARK(results->rawbuf[offset], RC6_HDR_MARK)) {
    return ERR;
  }
  offset++;
  if (!MATCH_SPACE(results->rawbuf[offset], RC6_HDR_SPACE)) {
    return ERR;
  }
  offset++;
  long data = 0;
  int used = 0;
  // Get start bit (1)
  if (getRClevel(results, &offset, &used, RC6_T1) != MARK) return ERR;
  if (getRClevel(results, &offset, &used, RC6_T1) != SPACE) return ERR;
  int nbits;
  for (nbits = 0; offset < results->rawlen; nbits++) {
    int levelA, levelB; // Next two levels
    levelA = getRClevel(results, &offset, &used, RC6_T1);
    if (nbits == 3) {
      // T bit is double wide; make sure second half matches
      if (levelA != getRClevel(results, &offset, &used, RC6_T1)) return ERR;
    }
    levelB = getRClevel(results, &offset, &used, RC6_T1);
    if (nbits == 3) {
      // T bit is double wide; make sure second half matches
      if (levelB != getRClevel(results, &offset, &used, RC6_T1)) return ERR;
    }
    if (levelA == MARK && levelB == SPACE) { // reversed compared to RC5
      // 1 bit
      data = (data << 1) | 1;
    }
    else if (levelA == SPACE && levelB == MARK) {
      // zero bit
      data <<= 1;
    }
    else {
      return ERR; // Error
    }
  }
  // Success
  results->bits = nbits;
  results->value = data;
  results->decode_type = RC6;
  return DECODED;
}


//##Minibloq:
int IRrecv::getIRRemoteCode()
{
	if (!isEnabledIRIn())
		enableIRIn();

	decode_results results;
    int result = 0; //##Se determinó que devolver 0 en vez de -1 cuando no recibe nada coherente, es mejor
                    //para algunas actividades, como la música con el control remoto (sobre todo cuando se
                    //trabaja con chicos). Es por esto que abajo se devuelve 10 cuando se detecta un 0.
    //##Implementar...
    if (decode(&results))
    {
        int count = results.rawlen;
        if (results.decode_type == RC5)  //By now, only supports RC5 Format.
        {
            char command[10];// = results.value;
            ultoa(results.value,command,16);
            if ( (strcmp(command, "0") == 0) || (strcmp(command, "800") == 0)) //##Ver si el cero es correcto.
                result = 10; //See note above.
            else if ( (strcmp(command, "1") == 0) || (strcmp(command, "801") == 0))
                result = 1;
            else if ( (strcmp(command, "2") == 0) || (strcmp(command, "802") == 0))
                result = 2;
            else if ( (strcmp(command, "3") == 0) || (strcmp(command, "803") == 0))
                result = 3;
            else if ((strcmp(command, "4") == 0) || (strcmp(command, "804") == 0))
                result = 4;
            else if ( (strcmp(command, "5") == 0) || (strcmp(command, "805") == 0))
                result = 5;
            else if ((strcmp(command, "6") == 0) || (strcmp(command, "806") == 0))
                result = 6;
            else if ( (strcmp(command, "7") == 0) || (strcmp(command, "807") == 0))
                result = 7;
            else if ((strcmp(command, "8") == 0) || (strcmp(command, "808") == 0))
                result = 8;
            else if ( (strcmp(command, "9") == 0) || (strcmp(command, "809") == 0))
                result = 9;
//            if ((strcmp(command, "80c")  == 0) || (strcmp(command, "c") == 0))
//              //Remote control's POWER button: By now do nothing.
//                return 10;
        }
        resume(); // Receive the next value
    }
		
	//disableIRIn();	
    return result; //##Invalid code... See if in the future there will be a constant defined in Minibloq.
}
