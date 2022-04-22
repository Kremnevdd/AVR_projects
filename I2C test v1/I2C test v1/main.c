/*
 * I2C test v1.c
 *
 * Created: 18.01.2020 0:54:20
 * Author : Админ
 */ 
#define F_CPU 4000000
#include <avr/io.h>
#include <util/delay.h>
int state = 1;
int i;
volatile char D ;



void send(){
	 for(int p=7; p>=0; p--){
	    PORTB |= (D>>p)&(1<<PB0);
		PORTB |=(1<<PB1);
	    	_delay_ms(300);
			PORTB &=~(1<<PB0);
		   PORTB &=~(1<<PB1);	
		   _delay_ms(300);		
}
state =3;
       }
void start(){	
		PORTB |= (1<<PB0);
	_delay_ms(150);
	PORTB &= ~(1<<PORTB0);
	_delay_ms(150);
	PORTB |= (1<<PB0);
	_delay_ms(150);
	PORTB &= ~(1<<PORTB0);
	_delay_ms(150);
	PORTB |= (1<<PB0);
	_delay_ms(150);
	PORTB &= ~(1<<PORTB0);
	_delay_ms(150);	
	state=2;
}
void stop(){
	 PORTB |=(1<<PB1);
	_delay_ms(150);
    PORTB &=~(1<<PB1);
   _delay_ms(150);
   PORTB |=(1<<PB1);
   _delay_ms(150);
   PORTB &=~(1<<PB1);
   _delay_ms(150);
   PORTB |=(1<<PB1);
   _delay_ms(150);
   PORTB &=~(1<<PB1);
   _delay_ms(150);
	
}
void ask(){
	if (state==4)
	{ stop();
		state==1;
	} 
	else if(state !=4);
	{
		while (state !=4)
	{if ((PINB&(1<<PB4))==0){ 
		state=4;  
		}
	else
	{}
	}
	}
	}
	
	

int main(void)
{ 
	D=0x25;
	while (1)
{DDRB|= (1<<PB0)|(1<<PB1);
	PORTB|=(1<<PB4);

switch (state)
{
case 1:
start();
	break;
case 2:
send();
	break;
case 3:
ask();
	break;


}

   
    }
}

