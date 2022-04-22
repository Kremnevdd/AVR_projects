/*
 * GeneratoR-Penetrator.c
 *
 * Created: 24.02.2020 18:42:34
 * Author : Админ
 */ 
#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
volatile char shrek [4];
volatile char byte[4];
volatile char kek;
volatile long int freq = 10;
volatile int i=0;
long int delay;
volatile char recieve=0;
void timer_setup(){
DDRB|= (1<<PORTB1);	
TCCR1A|= (1<< COM1A0);
TCCR1B |= (1<<CS12)|(1<< CS10)|(1<<WGM12);
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
  UCSR0A|= (1<<RXC0);
  
}

ISR(USART_RX_vect){
	byte[i]=UDR0;
	i++;
	if (i==4){
		i=0;
		memcpy(&freq,&byte,4);
		OCR1A = (F_CPU/(freq*2*1024)-1);
		
	}
	UDR0 = freq;
	
}
