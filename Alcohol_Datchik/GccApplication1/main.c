/*
 * GccApplication1.c
 *
 * Created: 14.12.2019 13:20:56
 * Author : Админ
 */ 
#define F_CPU 16000000
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

volatile int A[30] ;
int n;
unsigned char digit[] =
{
	0x3f, //0
	0x06, //1
	0x5b, //2
	0x4f, //3
	0x66, //4
	0x6d, //5
	0x7d, //6
	0x07, //7
	0x7f, //8
	0x6f //9
};
 void setup (){
	 DDRB = 0b00001111;
	 DDRD= 0b00111000;
	 PORTD|=(1<<PORTD2);
	 EICRA |=(1<<ISC01);
	 EIMSK |=(1<<INT0);
	 sei();
	 ADMUX |= (1<<5)|(1<<6);
	 ADCSRA |= (1<<ADEN)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);
	 
 }
 void Number (){
	 PORTD &= !(1<<PORTD4);
	 for(int p=7; p>=0; p--){	 
		PORTD &= ~(1<<PORTD3);
		PORTB &= ~(1<<PORTB0);
	    PORTB |= (digit[n]>>p)&(1<<PORTB0);
	    PORTD |= (1<<PORTD3);
	}
	PORTD |= (1<<PORTD4);	 
}
int main(void)
{
	setup();
    while (1) 
    {
		int max = A[0];
	    for (int i = 1; i < 30; i++) {
		    if (A[i] > max) {
			    max = A[i];
		    }
	    }
		PORTB |=(1<<PORTB3);
		n = max/100; 
		Number();
		PORTB &= ~(1<<PORTB1);
		_delay_ms(5);
		n= max/10;
		n=n%10;
		PORTB |= (1<<PORTB1);
		Number();
		PORTB &= ~(1<<PORTB2);
		_delay_ms(5);
		n= max%10;
		PORTB |=(1<<PORTB2);
		Number(3);
		PORTB &= ~(1<<PORTB3);
		_delay_ms(5);
    }
}
ISR (INT0_vect){
	for(int i=0; i<30;i++){
		ADCSRA|= (1<<ADSC);
		while(ADCSRA &(1<<ADSC));
		A[i]=ADCH;
		_delay_ms(50);
	}
}
