/*
 * nRF 24L01 Transmitter New.c
 *
 * Created: 19.02.2020 12:15:13
 * Author : Админ
 */ 

#define F_CPU 8000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>



// Commands--------------------------------------------------------------------
#define R_REGISTER 0x00          // +n Read register n
#define W_REGISTER 0x20          // +n Write register n
#define R_RX_PAYLOAD 0x61        // Receive data from the top slot of the receiver queue
#define W_TX_PAYLOAD 0xA0        // Write to the transmitter queue data to be sent
#define FLUSH_TX 0xE1            // Transmitter queue reset
#define FLUSH_RX 0xE2            // Receiver queue reset
#define REUSE_TX_PL 0xE3         // Reuse last transmitted packet
#define R_RX_PL_WID 0x60         // Read the receiver data size at the beginning of the receiver queue
#define W_ACK_PAYLOAD 0xA8       // +p Record data for sending with a confirmation packet on channel p
#define W_TX_PAYLOAD_NOACK 0xB0  // Write to transmitter queue data for sending, for which confirmation is not required
#define NOP 0xFF                 // Nothing to do
// Registers-------------------------------------------------------------------------------------
#define CONFIG      0x00 // Settings register
#define EN_AA       0x01 // Auto confirm selection
#define EN_RXADDR   0x02 // transmitter channel selection
#define SETUP_AW    0x03 // Adress size setting
#define SETUP_RETR  0x04 // Resending setting
#define RF_CH       0x05 // Channel number. From 0 to 125
#define RF_SETUP    0x06 // Сhannel setting
#define STATUS      0x07 // Status register
#define OBSERVE_TX  0x08 // Number of retransmissions and lost packets
#define RPD         0x09 // Мощность принимаемого сигнала. Если младший бит = 1, то уровень более -64dBm
#define RX_ADDR_P0  0x0A // 3-5 байт (начиная с младшего байта). Адрес канала 0 приёмника.
#define RX_ADDR_P1  0x0B // 3-5 байт (начиная с младшего байта). Адрес канала 1 приёмника.
#define RX_ADDR_P2  0x0C // Младший байт адреса канала 2 приёмника. Старшие байты из RX_ADDR_P1
#define RX_ADDR_P3  0x0D // Младший байт адреса канала 3 приёмника. Старшие байты из RX_ADDR_P1
#define RX_ADDR_P4  0x0E // Младший байт адреса канала 4 приёмника. Старшие байты из RX_ADDR_P1
#define RX_ADDR_P5  0x0F // Младший байт адреса канала 5 приёмника. Старшие байты из RX_ADDR_P1
#define TX_ADDR     0x10 // 3-5 байт (начиная с младшего байта). Адрес удалённого устройства для передачи
#define RX_PW_P0    0x11 // Размер данных при приёме по каналу 0: от 1 до 32. 0 - канал не используется.
#define RX_PW_P1    0x12 // Размер данных при приёме по каналу 1: от 1 до 32. 0 - канал не используется.
#define RX_PW_P2    0x13 // Размер данных при приёме по каналу 2: от 1 до 32. 0 - канал не используется.
#define RX_PW_P3    0x14 // Размер данных при приёме по каналу 3: от 1 до 32. 0 - канал не используется.
#define RX_PW_P4    0x15 // Размер данных при приёме по каналу 4: от 1 до 32. 0 - канал не используется.
#define RX_PW_P5    0x16 // Размер данных при приёме по каналу 5: от 1 до 32. 0 - канал не используется.
#define FIFO_STATUS 0x17 // Состояние очередей FIFO приёмника и передатчика
#define DYNPD       0x1C // Выбор каналов приёмника для которых используется произвольная длина пакетов.
#define FEATURE     0x1D // Регистр опций
// Registers bits-----------------------------------------------------------------------------------------------------------
// CONFIG
#define MASK_RX_DR  6 // Запрещает прерывание по RX_DR (получение пакета)
#define MASK_TX_DS  5 // Запрещает прерывание по TX_DS (завершение отправки пакета)
#define MASK_MAX_RT 4 // Запрещает прерывание по MAX_RT (превышение числа повторных попыток отправки)
#define EN_CRC      3 // Включает CRC
#define CRCO        2 // Размер поля CRC: 0 - 1 байт; 1 - 2 байта
#define PWR_UP      1 // Включение питания
#define PRIM_RX     0 // Выбор режима: 0 - PTX (передатчик) 1 - PRX (приёмник)
// EN_AA
#define ENAA_P5 5 // Включает автоподтверждение данных, полученных по каналу 5
#define ENAA_P4 4 // Включает автоподтверждение данных, полученных по каналу 4
#define ENAA_P3 3 // Включает автоподтверждение данных, полученных по каналу 3
#define ENAA_P2 2 // Включает автоподтверждение данных, полученных по каналу 2
#define ENAA_P1 1 // Включает автоподтверждение данных, полученных по каналу 1
#define ENAA_P0 0 // Включает автоподтверждение данных, полученных по каналу 0
// EN_RXADDR
#define ERX_P5 5 // Включает канал 5 приёмника
#define ERX_P4 4 // Включает канал 4 приёмника
#define ERX_P3 3 // Включает канал 3 приёмника
#define ERX_P2 2 // Включает канал 2 приёмника
#define ERX_P1 1 // Включает канал 1 приёмника
#define ERX_P0 0 // Включает канал 0 приёмника
// SETUP_AW
#define AW 0 // Два бита, Выбирает ширину поля адреса: 1 - 3 байта; 2 - 4 байта; 3 - 5 байт.
#define SETUP_AW_3BYTES_ADDRESS (1 << AW)
#define SETUP_AW_4BYTES_ADDRESS (2 << AW)
#define SETUP_AW_5BYTES_ADDRESS (3 << AW)
// SETUP_RETR
#define ARD 4 // 4 бита. Задаёт значение задержки перед повторной отправкой пакета: 250 x (n + 1) мкс
#define ARC 0 // 4 бита. Количество повторных попыток отправки, 0 - повторная отправка отключена.
#define SETUP_RETR_DELAY_250MKS  (0 << ARD)
#define SETUP_RETR_DELAY_500MKS  (1 << ARD)
#define SETUP_RETR_DELAY_750MKS  (2 << ARD)
#define SETUP_RETR_DELAY_1000MKS (3 << ARD)
#define SETUP_RETR_DELAY_1250MKS (4 << ARD)
#define SETUP_RETR_DELAY_1500MKS (5 << ARD)
#define SETUP_RETR_DELAY_1750MKS (6 << ARD)
#define SETUP_RETR_DELAY_2000MKS (7 << ARD)
#define SETUP_RETR_DELAY_2250MKS (8 << ARD)
#define SETUP_RETR_DELAY_2500MKS (9 << ARD)
#define SETUP_RETR_DELAY_2750MKS (10 << ARD)
#define SETUP_RETR_DELAY_3000MKS (11 << ARD)
#define SETUP_RETR_DELAY_3250MKS (12 << ARD)
#define SETUP_RETR_DELAY_3500MKS (13 << ARD)
#define SETUP_RETR_DELAY_3750MKS (14 << ARD)
#define SETUP_RETR_DELAY_4000MKS (15 << ARD)

#define SETUP_RETR_NO_RETRANSMIT (0 << ARC)
#define SETUP_RETR_UP_TO_1_RETRANSMIT (1 << ARC)
#define SETUP_RETR_UP_TO_2_RETRANSMIT (2 << ARC)
#define SETUP_RETR_UP_TO_3_RETRANSMIT (3 << ARC)
#define SETUP_RETR_UP_TO_4_RETRANSMIT (4 << ARC)
#define SETUP_RETR_UP_TO_5_RETRANSMIT (5 << ARC)
#define SETUP_RETR_UP_TO_6_RETRANSMIT (6 << ARC)
#define SETUP_RETR_UP_TO_7_RETRANSMIT (7 << ARC)
#define SETUP_RETR_UP_TO_8_RETRANSMIT (8 << ARC)
#define SETUP_RETR_UP_TO_9_RETRANSMIT (9 << ARC)
#define SETUP_RETR_UP_TO_10_RETRANSMIT (10 << ARC)
#define SETUP_RETR_UP_TO_11_RETRANSMIT (11 << ARC)
#define SETUP_RETR_UP_TO_12_RETRANSMIT (12 << ARC)
#define SETUP_RETR_UP_TO_13_RETRANSMIT (13 << ARC)
#define SETUP_RETR_UP_TO_14_RETRANSMIT (14 << ARC)
#define SETUP_RETR_UP_TO_15_RETRANSMIT (15 << ARC)

// RF_SETUP
#define CONT_WAVE   7 // (Только для nRF24L01+) Непрерывная передача несущей (для тестов)
#define RF_DR_LOW   5 // (Только для nRF24L01+) Включает скорость 250кбит/с. RF_DR_HIGH должен быть 0
#define PLL_LOCK    4 // Для тестов
#define RF_DR_HIGH  3 // Выбор скорости обмена (при значении бита RF_DR_LOW = 0): 0 - 1Мбит/с; 1 - 2Мбит/с
#define RF_PWR      1 // 2бита. Выбирает мощность передатчика: 0 - -18dBm; 1 - -16dBm; 2 - -6dBm; 3 - 0dBm

#define RF_SETUP_MINUS18DBM (0 << RF_PWR)
#define RF_SETUP_MINUS12DBM (1 << RF_PWR)
#define RF_SETUP_MINUS6DBM  (2 << RF_PWR)
#define RF_SETUP_0DBM       (3 << RF_PWR)

#define RF_SETUP_1MBPS (0 << RF_DR_HIGH)
#define RF_SETUP_2MBPS (1 << RF_DR_HIGH)
#define RF_SETUP_250KBPS (1 << RF_DR_LOW) // этот режим не должен использоваться с контролем доставки

// STATUS
#define RX_DR   6 // Флаг получения новых данных в FIFO приёмника. Для сброса флага нужно записать 1
#define TX_DS   5 // Флаг завершения передачи. Для сброса флага нужно записать 1
#define MAX_RT  4 // Флаг превышения установленного числа повторов. Без сброса (записать 1) обмен невозможен
#define RX_P_NO 1 // 3 бита. Номер канала, данные для которого доступны в FIFO приёмника. 7 -  FIFO пусто.
#define TX_FULL_STATUS 0 // Признак заполнения FIFO передатчика: 1 - заполнено; 0 - есть доступные слоты
// (переименовано из TX_FULL во избежание путаницы с одноимённым битом из регистра FIFO_STATUS)

// OBSERVE_TX
#define PLOS_CNT  4 // 4 бита. Общее количество пакетов без подтверждения. Сбрасывается записью RF_CH
#define ARC_CNT   0 // 4 бита. Количество предпринятых повторов при последней отправке.

// FIFO_STATUS
#define TX_REUSE      6 // Признак готовности последнего пакета для повторной отправки.
#define TX_FULL_FIFO  5 // Флаг переполнения FIFO очереди передатчика.
// (переименовано из TX_FULL во избежание путаницы с одноимённым битом из регистра STATUS)
#define TX_EMPTY      4 // Флаг освобождения FIFO очереди передатчика.
#define RX_FULL       1 // Флаг переполнения FIFO очереди приёмника.
#define RX_EMPTY      0 // Флаг освобождения FIFO очереди приёмника.

// DYNDP
#define DPL_P5 5 // Включает приём пакетов произвольной длины по каналу 5
#define DPL_P4 4 // Включает приём пакетов произвольной длины по каналу 4
#define DPL_P3 3 // Включает приём пакетов произвольной длины по каналу 3
#define DPL_P2 2 // Включает приём пакетов произвольной длины по каналу 2
#define DPL_P1 1 // Включает приём пакетов произвольной длины по каналу 1
#define DPL_P0 0 // Включает приём пакетов произвольной длины по каналу 0

// FEATURE
#define EN_DPL      2 // Включает поддержку приёма и передачи пакетов произвольной длины
#define EN_ACK_PAY  1 // Разрешает передачу данных с пакетами подтверждения приёма
#define EN_DYN_ACK  0 // Разрешает использование W_TX_PAYLOAD_NOACK


#define DD_MOSI PB3
#define DD_MISO PB4
#define DD_SCK PB5
#define DD_CE PB2
#define DD_CSN PB1
#define RADIO_PORT PORTB


void SPI_Setup(){
	DDRB |= (1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_CE)|(1<<DD_CSN);
	SPCR |=(1<<SPIE)|(1<<SPE)|(1<<MSTR); // Уточнить частоту передачи и CE
	
}
uint8_t SPI_send (uint8_t data){    // Transmitte and receive with SPI interface
	SPDR=data;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}
void port_init (){                   // Ports initialization
	RADIO_PORT |= (1<<DD_CSN);
	RADIO_PORT &=~ (1<<DD_CE);
}
uint8_t write_reg (uint8_t adr,uint8_t reg ){    // write data "reg" to adr register, returns status register
	RADIO_PORT &=~ (1<<DD_CSN);
	SPI_send(adr|W_REGISTER);// &31 ??????
	uint8_t status = SPI_send(reg);
	RADIO_PORT |= (1<<DD_CSN);
    return status;
} 
uint8_t read_reg (uint8_t adr){          // read "adr" register
	RADIO_PORT &=~ (1<<DD_CSN);
	SPI_send(adr|R_REGISTER);
	uint8_t answer = SPI_send(NOP);
	RADIO_PORT |= (1<<DD_CSN);
	return answer;
}
uint8_t command (uint8_t cmd){           // execute "cmd" command, returns status register
	RADIO_PORT &=~ (1<<DD_CSN);
	uint8_t status = SPI_send(cmd);
	RADIO_PORT |= (1<<DD_CSN);
	return status;
}
uint8_t radio_on (){                                           //вообще странная функция - спиздил у мужика, 1 из 10 функций, наверн обойдусь без нее, вообще не понравилось  
	uint8_t self_adress [] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};    // разобраться с адресами хыхыхыхыхыхы
	uint8_t remote_adress[] = {0xC2, 0xC2, 0xC2, 0xC2, 0xC2};   // хыхыхыхыхыхыхыхыхых
	uint8_t radio_channel =3;
	RADIO_PORT &=~ (1<<DD_CE);
	for (uint8_t cnt =100; cnt=0; cnt-- ){
		write_reg(CONFIG, (1 << EN_CRC) | (1 << CRCO) | (1 << PRIM_RX));/// ne uveren chto srabotayet
		if (read_reg(CONFIG)==((1 << EN_CRC) | (1 << CRCO) | (1 << PRIM_RX)))
		{
			break;
		} 
		
		if (cnt==0)
		{
			return 0;
		}
}
// Вектор прерываний по IRQ написать по востребованию.
int main(void)
{
	radio_on();
	write_reg(EN_AA, (1<<ENAA_P1));
	write_reg(EN_RXADDR, (1 << ERX_P0) | (1 << ERX_P1));
	write_reg (SETUP_AW, SETUP_AW_5BYTES_ADDRESS);
	write_reg(SETUP_RETR, SETUP_RETR_DELAY_250MKS | SETUP_RETR_UP_TO_2_RETRANSMIT);
	write_reg(RF_CH, radio_channel);
	write_reg(RF_SETUP, RF_SETUP_1MBPS | RF_SETUP_0DBM);
	
	
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

