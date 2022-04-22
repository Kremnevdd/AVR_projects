/*
 * UART GenHz.c
 *
 * Created: 21.02.2020 11:59:21
 * Author : Админ
 */ 
 
#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
volatile char shrek [4];
 volatile char byte[4];
 volatile int N=1024;
 volatile long int freq = 2;
volatile char i=0;
long int delay;
volatile char recieve=0;
void timer_setup(){
DDRB|= (1<<PORTB1);	
TCCR1A|= (1<< COM1A0);
TCCR1B |= (1<< CS12)|(1<<CS10)|(1<<WGM12);
OCR1A = (F_CPU/(freq*2*1024)-1);
	
	
}


void UART_setup (){
	UCSR0B |= (1<<TXEN0)|(1<<RXEN0)|(1<<RXCIE0);
	UCSR0C |= (1<<UCSZ00)|(1<<UCSZ01);
	UBRR0L = 103; // 16MHz: baudrate 9600
	sei();
}

	


int main(void)

{
  UART_setup();
  timer_setup();
  while(!(UCSR0A & (1<<UDRE0)));
	 UDR0=0x33;
  while(1)
  {
	 
  }
}

ISR(USART_RX_vect){
	
	byte[i]=UDR0;
	i++;
	
	if (i==4){
		i=0;
		//recieve=1;
  		  memcpy(&freq,&byte,4);
			if ((freq<8000000) && (freq>122))
			{
			
				N=1;
				TCCR1B = (1<<CS10)|(1<<WGM12);
			}
			else if ((freq<1000000) && (freq> 15))
			{
				N=8;
				TCCR1B = (1<<CS11)|(1<<WGM12);
			}
			else if ((freq <125000) && (freq> 2))
			{ 
				N=64;
				TCCR1B = (1<< CS11)|(1<<CS10)|(1<<WGM12);
			}
			else if ((freq< 31250) && (freq> 1))
			{
				N=256;
				TCCR1B = (1<< CS12)|(1<<WGM12);
			}
		  OCR1A = (F_CPU/(freq*2*N)-1);
		  TCNT1 =0;
	}
}