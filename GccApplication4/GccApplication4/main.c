/*
 * GccApplication4.c
 *
 * Created: 19.01.2020 19:48:49
 * Author : Админ
 */ 
#define F_CPU 4000000L 
#include <avr/io.h>
#include <util/delay.h>
int D;
void send(){
	 PORTB &=~(1<<PB2);// LATCH
	 for(int p=7; p>=0; p--){
	    PORTB |= (D>>p)&(1<<PB0);// DS
		PORTB |=(1<<PB1);//SCK
	    	//_delay_ms(200);
			PORTB &=~(1<<PB0);
		   PORTB &=~(1<<PB1);	
		  // _delay_ms(200);		
}
     PORTB |= (1<<PB2);//LaTch
	// _delay_ms(100);
}
int main(void)
{DDRB |= (1<<PB0)|(1<<PB1)|(1<<PB2);
	DDRB &=~(1<<PB3);
	PORTB |=(1<<PB3);
    int c =1;
    while (1) 
    {
		switch (c)
		{
				case 1 : 
				D=0b0111000;
				send();
				if ((!PINB &(1<<PB3))==0)
				{
					c=2;
				}
				break;
			case 2 : 
			D=0b10101010;
			send();
			if ((!PINB &(1<<PB3))==0)
			{
				c=3;
			}
			break;
			case  3 : 
			D=0b11001101;
			send();
			if ((!PINB &(1<<PB3))==0)
			{
				c=1;
			}
			break;
		}
		 //D=0xF3;
		//send();
		
    }
}

