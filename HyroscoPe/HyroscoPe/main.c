/*
 * HyroscoPe.c
 *
 * Created: 16.03.2020 16:08:33
 * Author : Админ
 */ 
#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>


#define W 0
#define R 1
#define R_PWR_M  0x6B
#define USER_CTRL 0x6A
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define GYRO_XOUT_H  0x43
#define GYRO_XOUT_L  0x44
#define GYRO_YOUT_H  0x45
#define GYRO_YOUT_L  0x46
#define GYRO_ZOUT_H  0x47
#define GYRO_ZOUT_L  0x48
#define GYRO_CONFIG  0x1B
#define ACCEL_CONFIG 0x1C
#define CONFIG       0x1A
#define SMPRT_DIV   0x19
#define FF_THR 0x1D
#define FF_DUR 0x1E
#define MOT_THR 0x1F
#define MOT_DUR 0x20
#define ZRMOT_THR 0x21
#define ZRMOT_DUR 0x22


char SLA =0x68;
char x_gyro[2];
char y_gyro[2];
char z_gyro[2];
char x_accel[2];
char y_accel[2];
char z_accel[2];

int16_t x_hyro_full = 0;
int16_t y_hyro_full = 0;
int16_t z_hyro_full = 0;
int16_t x_accel_full = 0;
int16_t y_accel_full = 0;
int16_t z_accel_full = 0;
int16_t GYRO_X;
int16_t GYRO_Y;
int16_t GYRO_Z;
int16_t ACCEL_X;
int16_t ACCEL_Y;
int16_t ACCEL_Z;
char sas[2];
int16_t ZUZ;




void UART_setup(void)
{
	UCSR0B |= (1<<TXEN0)|(1<<RXEN0);
	UCSR0C |= (1<<UCSZ00)|(1<<UCSZ01);
	UBRR0L = 103; // 16MHz: baudrate 9600

}
void UART_sent (char data)
{
	UDR0 = data;
	while(!(UCSR0A & (1<<UDRE0)));
}
void TWI_start(void)
{
	TWBR = 12;    // Bit rate register
	TWSR = 0;     // Status register (Prescaler bits 00 )
	TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWSTA)|(1<<TWEN); // Control register (Start condition)		
	while(!(TWCR & (1<<TWINT)));
}
void TWI_STOP (void)
{
	TWCR = (1<< TWINT)|(1<<TWSTO)|(1<<TWEN);
//	while(!(TWCR & (1<<TWINT)));
	_delay_us(1);
}
void adr_W_R (uint8_t b)
{
	TWDR = (SLA<<1)|b;
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while(!(TWCR & (1<<TWINT)));
}
void TWI_send(uint8_t sent)
{
	TWDR = sent;
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while(!(TWCR & (1<<TWINT))); 
}
uint8_t TWI_recieve (void)
{
	//while(!(TWCR & (1<<TWINT)));
	 uint8_t data = TWDR;
	//TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	
	// while(!(TWCR & (1<<TWINT)));
	 
	 

	 return data;
}
uint8_t recieve_data_from(uint8_t addr)
{
	TWI_start();
	adr_W_R(W);
	TWI_send(addr);
	TWI_start();
	adr_W_R(R);
	TWCR = (1<<TWINT)|(1<<TWEN);
	 while(!(TWCR & (1<<TWINT)));

	uint8_t data_from = TWI_recieve();
	return data_from;	
}
void write_data_at (uint8_t addr, uint8_t data)
{
	TWI_start();
	adr_W_R(W);
	TWI_send(addr);
	TWI_send(data);
	TWI_STOP();
}
void hyro_Init(void)
{
	UART_setup();
	
	write_data_at(R_PWR_M,0x01);
	
	write_data_at(GYRO_CONFIG,0x00);
	
	write_data_at(ACCEL_CONFIG,0x00);
	
    write_data_at(CONFIG,0x00);
}


int main(void)
{ 
	hyro_Init();
   while (1) 
    {
		
		
		TWI_start();
		/*
		x_gyro[1] = recieve_data_from(GYRO_XOUT_H);
		TWI_STOP();
		x_gyro[0]=  recieve_data_from(GYRO_XOUT_L);
		TWI_STOP();
		
		y_gyro[1] = recieve_data_from(GYRO_YOUT_H);
		TWI_STOP();
		y_gyro[0] = recieve_data_from(GYRO_YOUT_L);
		TWI_STOP();
		
		z_gyro[1] = recieve_data_from(GYRO_ZOUT_H);
		TWI_STOP();
		z_gyro[0] = recieve_data_from(GYRO_ZOUT_L);
		TWI_STOP();
		*/
		
		x_accel[0] = recieve_data_from(ACCEL_XOUT_H);
		TWI_STOP();
		x_accel[1] = recieve_data_from(ACCEL_XOUT_L);
	    TWI_STOP();		
		/*
		y_accel[1]= recieve_data_from(ACCEL_YOUT_H);
		TWI_STOP();	
		y_accel[0]= recieve_data_from(ACCEL_YOUT_L);
		TWI_STOP();	
		
		z_accel[1] = recieve_data_from(ACCEL_ZOUT_H);
		TWI_STOP();	
		z_accel[0] = recieve_data_from(ACCEL_ZOUT_L);
		TWI_STOP();	
		memcpy(&x_hyro_full,&x_gyro,2);
		memcpy(&y_hyro_full,&y_gyro,2);
		memcpy(&z_hyro_full,&z_gyro,2);
		memcpy(&x_accel_full,&x_accel,2);
		memcpy(&y_accel_full,&y_accel,2);
		memcpy(&z_accel_full,&z_accel,2);*/
		//x_accel_full =(x_accel[1]<<8)|(x_accel[0]);
		
		/*GYRO_X = 0.8*GYRO_X+0.2*x_hyro_full;
		GYRO_Y = 0.8*GYRO_Y+0.2*y_hyro_full;
		GYRO_Z = 0.8*GYRO_Z+0.2*z_hyro_full;*/
		//ACCEL_X = 0.8*ACCEL_X+0.2*x_accel_full;
		 // memcpy(&x_accel,&ACCEL_X,2);
		  UART_sent(x_accel[1]);
		UART_sent(x_accel[0]);
		/*
		ACCEL_Y = 0.8*ACCEL_Y+0.2*y_accel_full;
		ACCEL_Z = 0.8*ACCEL_Z+0.2*z_accel_full;
		
		 memcpy(&x_gyro,&GYRO_X,2);
		   memcpy(&y_gyro,&GYRO_Y,2);
		   memcpy(&z_gyro,&GYRO_Z,2);
		   memcpy(&x_accel,&ACCEL_X,2);
		  memcpy(&y_accel,&ACCEL_Y,2);
		memcpy(&z_accel,&ACCEL_Z,2);
		
	 UART_sent(x_gyro[0]);
  UART_sent(x_gyro[1]);
  UART_sent(y_gyro[0]);
	       UART_sent(y_gyro[1]);
			UART_sent(z_gyro[0]);
			UART_sent(z_gyro[1]);
			UART_sent(x_accel[0]);
			UART_sent(x_accel[1]);
			UART_sent(y_accel[0]);
			UART_sent(y_accel[1]);
			UART_sent(z_accel[0]);
			UART_sent(z_accel[1]);
			*/
		
		_delay_ms(200);
    }	
}

