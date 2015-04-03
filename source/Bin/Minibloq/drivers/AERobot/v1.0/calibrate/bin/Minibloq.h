//Copyright (C) 1997-2011 Julián U. da Silva Gillig - 
//This file is  is distributed under the RobotGroup-Multiplo Pacifist License (RMPL), either version 1.0 of the licence, 
//or (at your option) any later version. 

//##Aux functions for the code generated with Minibloq:

#define F_CPU 12000000UL

#define ee_POWER_STATE 0x00
#define ee_move_f_a 0x02
#define ee_move_f_b 0x04
#define ee_move_b_a 0x06
#define ee_move_b_b 0x08
#define ee_rotate_ccw_a 0x0a
#define ee_rotate_ccw_b 0x0c
#define ee_rotate_cw_a 0x0e
#define ee_rotate_cw_b 0x10
#define ee_line_dark 0x12
#define ee_line_light 0x14
 
#include "DCMotor.h"
#include "Ping.h"
#include "IRRanger.h"
//#include "pitches.h"
#include "Servo.h"
#include <avr/eeprom.h>
#include <avr/io.h>
#include <avr/wdt.h>
//include "interrupt.h"
#include <stdlib.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <math.h>

#define BuzzerPin 12

#define serial0 Serial
#define serial1 Serial1

//Always cast (at least by now):
#define MOD(term1, term2) ( ((int)term1) % ((int)term2) )

#define timeStamp() millis()

#define M_E		2.7182818284590452354
#define M_PI	3.14159265358979323846


IRrecv irReceiver(2);

DCMotor motor0(3, 4, 5);
DCMotor motor1(11, 10, 9);
PingSensor ping(0);
IRRanger irRanger20to150(1, IRRanger::range20to150cm);
IRRanger irRanger10to80(1, IRRanger::range10to80cm);


#define sensor0  A0
#define sensor1  A1
#define sensor2  A2
#define sensor3  A3
#define sensor4  A4
#define sensor5  A5

//Servos:
Servo servo0;
Servo servo1;
Servo servo2;

//##PWM pins:
#define PWM3   3
#define PWM5   5
#define PWM6   6
#define PWM9   9
#define PWM10   10
#define PWM11  11

//##Digital pins:
#define D0  0
#define D1  1
#define D2  2
#define D3  3
#define D4  4
#define D5  5
#define D6  6
#define D7  7
#define D8  8
#define D9  9
#define D10 10
#define D11 11
#define D12 12
#define D13_LED 13
#define D14 14
#define D15 15
#define D16 16
#define D17 17
#define D18 18



//Bo's stuff starts here
const int DELAY_LIM = 5000;
enum MoveDirection {STOP, FORWARD, BACKWARD, TURN_LEFT, TURN_RIGHT};
enum SensorNum {LEFT = 800, CENTER = 600, RIGHT = 150};
enum LineReturn{N, L, C, R};
enum Color{BLACK, RED, ORANGE, YELLOW, GREEN, BLUE, INDIGO, VIOLET, WHITE};
enum Bump{NOBUMP, ISBUMP};
int previous_direction=0;

//load default motor calibration values
int move_f_a=0xca;
int move_f_b=0xca;
int move_b_a=0x33;
int move_b_b=0x33;
int rotate_ccw_a=0xca;
int rotate_ccw_b=0x2a;
int rotate_cw_b=0xca;
int rotate_cw_a=0x2a;

int8_t line_before;
int8_t line_after;
int current;

void setLED(unsigned char red, unsigned char green, unsigned char blue)
{
	//Bit banging 20-600kHz
	//Send LSB first
	
	unsigned char array[4] = {0x3A, red, blue,green};

	for(char byte = 0; byte <= 3; byte++)
	{
		for(unsigned char bit=0; bit<=7; bit++)
		{
			//bit initiation
			PORTB |= 0x04;
			_delay_us(2);
			PORTB &=~ 0x04;
			_delay_us(3);

			if(array[byte] & (0x80>>bit))
				PORTB |= 0x04;
			else
				PORTB &=~0x04;
			_delay_us(10); 
			PORTB &=~0x04;
			_delay_us(5);  //~50kHz	
		}
	}
	_delay_us(80);//End of Sequence
}

void ledControl(Color color)
{
	current=color;
	if(color == BLACK)
	{
		setLED(0,0,0);
	}
	if(color == RED)
	{
		setLED(50,0,0);
	}
	if(color == ORANGE)
	{
		setLED(50,32,0);
	}
	if(color == YELLOW)
	{
		setLED(50,50,0);
	}
	if(color == GREEN)
	{
		setLED(0,50,0);
	}
	if(color == BLUE)
	{
		setLED(0,0,50);
	}
	if(color == INDIGO)
	{
		setLED(10,0,30);
	}
	if(color == VIOLET)
	{
		setLED(50,30,50);
	}
	if(color == WHITE)
	{
		setLED(50,50,50);
	}
}

void move_forward()
{
	PORTB |= (1<<1);
	OCR0A = 0x0;
	PORTC |= (1<<0);
	OCR0B = 0x0;
	_delay_ms(10);
	
	
	PORTB |= (1<<1);
	OCR0A = move_f_a;
	PORTC |= (1<<0);
	OCR0B = move_f_b;

}

void move_ccw()
{
	PORTB |= (1<<1);
	OCR0A = 0x0;
	PORTC |= (1<<0);
	OCR0B = 0xff;
	_delay_ms(10);

	PORTB |= (1<<1);
	OCR0A = 0xb9;
	PORTC |= (1<<0);
	OCR0B = 0xff;
	

}
void rotate_ccw()
{

	PORTB |= (1<<1);
	OCR0A = 0x0;
//	OCR0A = 0xff;
	PORTC &= ~(1<<0);
	OCR0B = 0xff;
	_delay_ms(10);
	
	PORTB |= (1<<1);
	OCR0A = rotate_ccw_a;
	PORTC &= ~(1<<0);
	OCR0B = rotate_ccw_b;

}

void move_cw()
{
	PORTB |= (1<<1);
	OCR0A = 0xff;
	PORTC |= (1<<0);
	OCR0B = 0x0;
	_delay_ms(10);

	PORTB |= (1<<1);
	OCR0A = 0xff;
	PORTC |= (1<<0);
	OCR0B = 0xb9;

}
void rotate_cw()
{
	PORTB &= ~(1<<1);
	OCR0A = 0xff;
	PORTC |= (1<<0);
	OCR0B = 0x00;
	_delay_ms(10);

	PORTB &= ~(1<<1);
	OCR0A = rotate_cw_a;
	PORTC |= (1<<0);
	OCR0B = rotate_cw_b;

}


void move_backward()
{
	PORTB &= ~(1<<1);
	OCR0A = 0xff;
	PORTC &= ~(1<<0);
	OCR0B = 0xff;
	_delay_ms(10);

	PORTB &= ~(1<<1);
	OCR0A = move_b_a;
	PORTC &= ~(1<<0);
	OCR0B = move_b_b;


}

void move_stop()
{
	PORTB &= ~(1<<1);
	OCR0A = 0x00;
	PORTC &= ~(1<<0);
	OCR0B = 0x00;
}

void move(int direction)
{
	if(direction!=previous_direction)
	{
		if(direction == FORWARD)
		{
			move_forward();
			previous_direction=direction;
		}	
		else if(direction == BACKWARD)
		{
			move_backward();
			previous_direction=direction;
		}	
		else if(direction == TURN_LEFT)
		{
			rotate_ccw();
			previous_direction=direction;
		}	
		else if(direction == TURN_RIGHT)
		{
			rotate_cw();
			previous_direction=direction;
		}	
		else
		{
			move_stop();
			previous_direction=direction;
		}	
		_delay_ms(40);
	
	}
	
}

void load_calibration()
{
	
	line_before=(int8_t)eeprom_read_byte((uint8_t *) (ee_line_dark));//load previous state value;
	line_after=(int8_t)eeprom_read_byte((uint8_t *) (ee_line_light));//load previous state value;
	//if motors are calibrated, load those values
	int temp;
	temp=eeprom_read_byte((uint8_t *) (ee_move_f_a));//load previous state value
	if(temp!=0xff)
	{
		move_f_a=temp;
	}

	temp=eeprom_read_byte((uint8_t *) (ee_move_f_b));//load previous state value
	if(temp!=0xff)
	{
		move_f_b=temp;
	}

	temp=eeprom_read_byte((uint8_t *) (ee_move_b_a));//load previous state value
	if(temp!=0xff)
	{
		move_b_a=temp;
	}

	temp=eeprom_read_byte((uint8_t *) (ee_move_b_b));//load previous state value
	if(temp!=0xff)
	{
		move_b_b=temp;
	}

	temp=eeprom_read_byte((uint8_t *) (ee_rotate_ccw_a));//load previous state value
	if(temp!=0xff)
	{
		rotate_ccw_a=temp;
	}

	temp=eeprom_read_byte((uint8_t *) (ee_rotate_ccw_b));//load previous state value
	if(temp!=0xff)
	{
		rotate_ccw_b=temp;
	}

	temp=eeprom_read_byte((uint8_t *) (ee_rotate_cw_b));//load previous state value
	if(temp!=0xff)
	{
		rotate_cw_b=temp;
	}

	temp=eeprom_read_byte((uint8_t *) (ee_rotate_cw_a));//load previous state value
	if(temp!=0xff)
	{
		rotate_cw_a=temp;
	}
		
}

ISR(__vector_default)
{
	PORTB &= ~(1<<1);
	OCR0A = 0x00;
	PORTC &= ~(1<<0);
	OCR0B = 0x00;
	cli();
	int i;
	for(i=20;i>0;i--)
	{
		setLED(i*5,0,0);
		_delay_ms(20);
	}
	DDRD &= ~(1<<7);
	PORTD &= ~(1<<7);

		
		
	wdt_enable(WDTO_30MS);
	while(1){};


}

void initBoard()
{
	MCUCR=1;
	MCUCR=0; 
	EIMSK=0;
	PCICR = 1;

	PCMSK0 = (1 << 5);  
	sei();
	
	
	
	uint8_t power_state=eeprom_read_byte((uint8_t *) (ee_POWER_STATE));//load previous state value

	if(power_state==0)//if 0, go to sleep
	{
		eeprom_write_byte((uint8_t *)(ee_POWER_STATE),1);//store current state in eeprom
		OCR0B = 0x00;// motor off
		OCR0A = 0x00;// motor off
		DDRB=0;
		PORTB=0;
		DDRC=0;
		PORTC=0;
		DDRD=0;
		PORTD=0;
		PORTD  &= ~(1<<0);//turn ir led off	
		sei();
		SMCR = 0x05;
		asm volatile("sleep\n\t");

	}
	else
	{
		eeprom_write_byte((uint8_t *)(ee_POWER_STATE),0);//store current state in eeprom
		load_calibration();
		DDRB=0;
		PORTB=0;
		DDRC=0;
		PORTC=0;
		DDRD=0;
		PORTD=0;	
		
		
		
		//turn on power for rest of robot
		DDRD |= (1<<7);
		PORTD |= (1<<7);
		DDRB |= 0x04;
		setLED(0,0,0);
		

		//enable pins for motor control
		DDRB |=(1<<1);
		DDRC |=(1<<0);

		//start init

		//turn on power for rest of robot
		DDRD |= (1<<7);
		PORTD |= (1<<7);
		
		

		DDRB |= 0x04;
		

		//initalize adc
		ADMUX = (1<<REFS0)|(1<<MUX1);//choose analog pin 
		ADCSRA = (1<<ADEN) |    (1<<ADPS0); //set up a/d



		//enable pins to control motor 
		
		OCR0B = 0x00;//start with motor off
		OCR0A = 0x00;//start with motor off	
		PORTB &= ~(1<<1);
		PORTC &= ~(1<<0);


		//initalize motors/pwm
		
		DDRD |= (1<<5);
		DDRD |= (1<<6);
		TCCR0A |= (1<<COM0A1) | (1<<COM0B1) | (1<<WGM00);
		TCCR0B =0x03; //prescaler set to 0
		OCR0B = 0x00;//start with motor off
		OCR0A = 0x00;//start with motor off
		PORTB &= ~(1<<1);
		PORTC &= ~(1<<0);
		setLED(0,0,0);
	
	//end init
	}
	
	
}







int lightSens(SensorNum num)
{
	if(num == CENTER)
	{
		ADMUX &= 0xf0;//clear adc port selection 
		ADMUX |= 2;//set adc port to left rx
		_delay_ms(1);
		//when the following code was in main, while loop started here
		ADCSRA |= (1<<ADSC);//start adc conversion to sample sensor with led off
		while((ADCSRA&(1<<ADSC))!=0);//busy wait for converstion to end
	}
	else if(num == LEFT)
	{
		//when the following code was in main, while loop started here
		ADMUX &= 0xf0;//clear adc port selection 
		ADMUX |= 5;//set adc port to left rx
		_delay_ms(1);
		ADCSRA |= (1<<ADSC);//start adc conversion to sample sensor with led off
		while((ADCSRA&(1<<ADSC))!=0);//busy wait for converstion to end
	}
	else
	{
		ADMUX &= 0xf0;//clear adc port selection 
		ADMUX |= 6;//set adc port to left rx
		_delay_ms(1);
		ADCSRA |= (1<<ADSC);//start adc conversion to sample sensor with led off
		while((ADCSRA&(1<<ADSC))!=0);//busy wait for converstion to end
	}

	return ADCW;	//store sensor value
}

int	distSens(SensorNum num)
{
	int adcsample_before=0;
	int adcsample_after=0;
	
	if(num == CENTER)
	{
		ADMUX &= 0xf0;//clear adc port selection 
		ADMUX |= 2;//set adc port to left rx
		//delay for 0.02 seconds
		_delay_ms(10);

		
		//when the following code was in main, while loop started here
		ADCSRA |= (1<<ADSC);//start adc conversion to sample sensor with led off
		while((ADCSRA&(1<<ADSC))!=0);//busy wait for converstion to end
		adcsample_before=ADCW;	//store sensor value

		PORTD &= ~(1<<0);//turn ir led on
		DDRD |= (1<<0);//turn ir led on
		
		_delay_us(20);//wait for it to fully turn on
		

		ADCSRA |= (1<<ADSC);//start adc conversion to sample sensor with led on
	
		while((ADCSRA&(1<<ADSC))!=0);//busy wait for converstion to end
		DDRD &= ~(1<<0);//turn ir led on
		PORTD  &= ~(1<<0);//turn ir led off	
		adcsample_after=ADCW;//store sensor value
	}
	else if(num == RIGHT)
	{
		ADMUX &= 0xf0;//clear adc port selection 
		ADMUX |= 5;//set adc port to left rx
		//delay for 0.02 seconds
		_delay_ms(10);

		
		//when the following code was in main, while loop started here
		ADCSRA |= (1<<ADSC);//start adc conversion to sample sensor with led off
		while((ADCSRA&(1<<ADSC))!=0);//busy wait for converstion to end
		adcsample_before=ADCW;	//store sensor value


		DDRD |=(1<<4);
		PORTD &= ~(1<<4);//turn ir led on
		_delay_us(20);//wait for it to fully turn on


		ADCSRA |= (1<<ADSC);//start adc conversion to sample sensor with led on
		while((ADCSRA&(1<<ADSC))!=0);//busy wait for converstion to end
		DDRD &=~(1<<4);
		PORTD  &= ~(1<<4);//turn ir led off		
		adcsample_after=ADCW;//store sensor value
	}
	else
	{
		ADMUX &= 0xf0;//clear adc port selection 
		ADMUX |= 6;//set adc port to left rx
		//delay for 0.02 seconds
		_delay_ms(10);

		
		//when the following code was in main, while loop started here
		ADCSRA |= (1<<ADSC);//start adc conversion to sample sensor with led off
		while((ADCSRA&(1<<ADSC))!=0);//busy wait for converstion to end
		adcsample_before=ADCW;	//store sensor value


		DDRB |=(1<<0);
		PORTB &= ~(1<<0);//turn ir led on
		_delay_us(20);//wait for it to fully turn on


		ADCSRA |= (1<<ADSC);//start adc conversion to sample sensor with led on
		while((ADCSRA&(1<<ADSC))!=0);//busy wait for converstion to end
		
		DDRB &= ~(1<<0);
		PORTB  &= ~(1<<0);//turn ir led off		
		adcsample_after=ADCW;//store sensor value
	}


	
		return (adcsample_after - adcsample_before);
		
}


bool bumpSens(SensorNum num)
{
	//compare before and after a/d values
	if(num < distSens(num))//bump detected
	{
		return true; //move backwards
	}
	else
	{
		return false; //move forward
	}
}

LineReturn lineSens()
{
	
	
	setLED(0,0,0);
	_delay_us(200);
	int adcsample_left_before=0;
	int adcsample_left_after=0;	
	int adcsample_right_before=0;
	int adcsample_right_after=0;


	


	ADMUX &= 0xf0;//clear adc port selection 
	ADMUX |= 4;//choose analog pin 
	_delay_ms(1);
	ADCSRA |= (1<<ADSC);//start adc conversion to sample sensor with led off
	while((ADCSRA&(1<<ADSC))!=0);//busy wait for converstion to end
	adcsample_left_before=ADCW;	//store sensor value

	
	ADMUX &= 0xf0;//clear adc port selection 
	ADMUX |= 1;//choose analog pin 
	_delay_ms(1);
	ADCSRA |= (1<<ADSC);//start adc conversion to sample sensor with led off
	while((ADCSRA&(1<<ADSC))!=0);//busy wait for converstion to end
	adcsample_right_before=ADCW;	//store sensor value

	ADMUX &= 0xf0;//clear adc port selection 
	ADMUX |= 4;//choose analog pin 	
	_delay_ms(1);
	DDRD |= (1<<1);//turn ir led on
	PORTD |= (1<<1);		
	_delay_us(30);//wait for it to fully turn on
	ADCSRA |= (1<<ADSC);//start adc conversion to sample sensor with led off
	while((ADCSRA&(1<<ADSC))!=0);//busy wait for converstion to end
	adcsample_left_after=ADCW;	//store sensor value
	PORTD  &= ~(1<<1);//turn ir led off		
	DDRD &= ~(1<<1);//turn ir led on








	ADMUX &= 0xf0;//clear adc port selection 
	ADMUX |= 1;//choose analog pin 
	_delay_us(1000); 
	DDRD |= (1<<1);//turn ir led on
	PORTD |= (1<<1);		
	_delay_us(30);//wait for it to fully turn on
	ADCSRA |= (1<<ADSC);//start adc conversion to sample sensor with led off
	while((ADCSRA&(1<<ADSC))!=0);//busy wait for converstion to end
	adcsample_right_after=ADCW;	//store sensor value
	PORTD  &= ~(1<<1);//turn ir led off		
	DDRD &= ~(1<<1);//turn ir led on


	ledControl((Color) current);
	/*int left=adcsample_left_after-adcsample_left_before;//left
	
	int right=adcsample_right_after+line_after-adcsample_right_before+line_before;//right
	*/
	
	int left=adcsample_left_after;//left
	
	int right=adcsample_right_after+line_after;//right
	if(abs(left-right)<15)
	{
		
		return C;
	}
	if((left)>(right))
	{
		return L;
	}
	else
	{
		return R;
	}
		
}

void Delay(float seconds)
{
	float mil_seconds = seconds * 1000;
	while((mil_seconds - DELAY_LIM)>0)
	{
		mil_seconds -= DELAY_LIM;
		_delay_ms(DELAY_LIM);
	}
	_delay_ms(mil_seconds);
}
//Bo's stuff ends here

inline uint8_t boolToPinLevel(bool value)
{
    if (value)
        return HIGH;
    return LOW;
}


inline bool pinLevelToBool(uint8_t value)
{
    //Only returns true if value is HIGH, otherwise, returns false:
    return (value == HIGH);
}


inline bool XOR(bool term1, bool term2) { return term1 != term2; }


//##The result will be a float between 0 and 100, but with higher resolution than the resolution reached 
//using random(min, max):
inline float Random()
{
    return (random(0, 10000)*0.01);
}


//##
inline void toneWithDelay(uint8_t _pin, unsigned int frequency, unsigned long duration)
{
    tone(_pin, frequency, duration);
    delay(duration * 1.30);
}


inline uint8_t DigitalRead(uint8_t _pin)
{
    pinMode(_pin, INPUT);
    return pinLevelToBool(digitalRead(_pin));
}


inline void DigitalWrite(uint8_t _pin, bool value)
{
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, boolToPinLevel(value));
}

inline float AnalogRead(uint8_t _pin)
{
    //##Optimization: This can be precomputed:
    return (((float)analogRead(_pin))*100.0)/1023.0;
}


inline float PulseIn(uint8_t _pin, bool value, float timeOut)
{
	pinMode(_pin, INPUT);
	return pulseIn(_pin, boolToPinLevel(value), (unsigned long) timeOut);
}


inline void AnalogWrite(uint8_t _pin, float value)
{
    float tempValue = constrain(value, 0, 100);
    tempValue = tempValue*2.55; // =tempValue*255/100;
    pinMode(_pin, OUTPUT);
    analogWrite(_pin, (int)tempValue);
}


long pingMeasureCM(int pin)
{
	if (irReceiver.isEnabledIRIn())
	{
		irReceiver.disableIRIn();
		delay(10);
	}
	return ping.measureCM(pin);
}

/*
void initBoard()
{
    Serial.begin(115200);

    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);

    //##Uses the analog input 0, that may have the same value in some designs, but it's not so prabably:
    randomSeed(analogRead(0));

	//irReceiver.enableIRIn();

	servo0.setPin(6); //This does not attach the servo (so the pin can be used by other library if the servo is not used).
	servo1.setPin(7);
	servo2.setPin(8);
}*/
