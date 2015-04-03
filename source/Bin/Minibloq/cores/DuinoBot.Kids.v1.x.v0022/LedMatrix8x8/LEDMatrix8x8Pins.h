#ifndef LEDMatrix8x8Pins__h
#define LEDMatrix8x8Pins__h

#define   	C8	    	0		// PF0
#define   	C7	    	6		// PC6
#define   	C6	    	7		// PD7
#define   	C5	    	4		// PD4
#define   	C4	    	1		// PD1
#define   	C3	    	0		// PD0
#define   	C2	    	7		// PB7
#define   	C1	    	0		// PB0

#define   	C8_PORT  	PORTF
#define   	C7_PORT  	PORTC  
#define   	C6_PORT		PORTD  
#define   	C5_PORT  	PORTD  
#define   	C4_PORT  	PORTD  
#define   	C3_PORT  	PORTD  
#define   	C2_PORT  	PORTB  
#define   	C1_PORT  	PORTB  

#define   	C8_DDR  	DDRF
#define		C7_DDR  	DDRC
#define   	C6_DDR  	DDRD
#define   	C5_DDR  	DDRD
#define  	C4_DDR  	DDRD
#define   	C3_DDR  	DDRD
#define   	C2_DDR  	DDRB
#define   	C1_DDR  	DDRB

// For the multiplexer:

#define 	R0 			1		// PB1
#define 	R1			2		// PB2
#define 	R2			3		// PB3

#define 	R0_PORT  	PORTB
#define 	R1_PORT  	PORTB
#define 	R2_PORT  	PORTB

#define 	R0_DDR  	DDRB
#define 	R1_DDR  	DDRB
#define 	R2_DDR  	DDRB

#define		R0_MUX_CHAN	0
#define		R1_MUX_CHAN	1
#define		R2_MUX_CHAN	2
#define		R3_MUX_CHAN	3
#define		R4_MUX_CHAN	4
#define		R5_MUX_CHAN	5
#define		R6_MUX_CHAN	6
#define		R7_MUX_CHAN	7

#endif
