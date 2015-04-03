#include "AERobot.h"

//Copyright (C) 2014-2015 Wyss Institute (http://wyss.harvard.edu)
//This file is licensed under a Creative Commons Attribution 4.0 International License. (CC BY 4.0)

int previous_direction=0;

int move_f_a=0xca;
int move_f_b=0xca;
int move_b_a=0x33;
int move_b_b=0x33;
int rotate_ccw_a=0xca;
int rotate_ccw_b=0x2a;
int rotate_cw_b=0xca;
int rotate_cw_a=0x2a;

int16_t line_before;
int16_t line_after;
int current;

//##Added 2015.03.26 - 2015.03.30 for miniBloq.v0.82:
void colorLED(Color color)
{
	ledControl(color);
}

void colorLED(float R, float G, float B)
{
	if (R < 0) R = 0.0;
	if (G < 0) G = 0.0;
	if (B < 0) B = 0.0;

	if (R > 100) R = 100.0;
	if (G > 100) G = 100.0;
	if (B > 100) B = 100.0;

	setLED((unsigned char)(R*2.55), (unsigned char)(G*2.55), (unsigned char)(B*2.55));
}

bool lineSens(SensorNum sensor)
{
	if (sensor = LEFT)
		return lineSens() == L;
	if (sensor = CENTER)
		return lineSens() == C;
	if (sensor = RIGHT)
		return lineSens() == R;
	if (sensor = NONE)
		return lineSens() == N;
	return false;
}
//##End 2015.03.26.

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
	uint8_t low,high;
	low=eeprom_read_byte((uint8_t *) (ee_line_light));
	high=eeprom_read_byte((uint8_t *) (ee_line_light)+1);
	line_after=((high<<8)&0xff00)+(low&0xff);

	low=eeprom_read_byte((uint8_t *) (ee_line_dark));
	high=eeprom_read_byte((uint8_t *) (ee_line_dark)+1);
	line_before=((high<<8)&0xff00)+(low&0xff);
	
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
	
	PORTB &= ~(1<<1);
	OCR0A = 0x00;
	PORTC &= ~(1<<0);
	OCR0B = 0x00;
	
	int i;
	for(i=20;i>0;i--)
	{
		setLED(i*5,0,0);
		_delay_ms(20);
		PORTB &= ~(1<<1);
		OCR0A = 0x00;
		PORTC &= ~(1<<0);
		OCR0B = 0x00;
	}
	DDRD &= ~(1<<7);
	PORTD &= ~(1<<7);

	wdt_enable(WDTO_30MS);
	while(1){};
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
	//##Modified 2015.03.30: Optimization and bug correction (comparing against num wasn't correct):
	//compare before and after a/d values
	return objectThreshold < distSens(num);
	
	/*
	//compare before and after a/d values
	if(num < distSens(num))//bump detected
	{
		return true; //move backwards
	}
	else
	{
		return false; //move forward
	}
	*/
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
	_delay_us(10);//wait for it to fully turn on
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
	_delay_us(10);//wait for it to fully turn on
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

void initAERobot()
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
		PORTB &= ~(1<<1);
		OCR0A = 0x00;
		PORTC &= ~(1<<0);
		OCR0B = 0x00;

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
	
	//##2015.03.25: This is to prevent the bug that forces the user to add the delay (or an instruction like move(FORWARD)) at the beginning of his program:
	_delay_ms(100);
	
	//##2015.03.26: This adds fast blinking at the beginning of the user application, so he knows that it downloaded ok, and that the hardware is ok also:
	for (unsigned char i=0; i<3; i++) {
		ledControl(BLUE);
		_delay_ms(70);
		ledControl(BLACK);
		_delay_ms(70);
	}
	_delay_ms(100); //Just to put a small interval between the last blink and the user code.
}

