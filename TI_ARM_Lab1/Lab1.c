#include "lm4f232h5qd.h"

void Delay(long i)        // задержка на i пустых циклов
{
  while(i > 0)
  {
    i--;
  }
}

void main()
{    
  SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOD; // включение тактирования порта GPIOD
  GPIO_PORTD_LOCK_R = 0x4C4F434B;    // снятие блокировки регистра GPIOCR
  GPIO_PORTD_CR_R = 0xFF;            // разрешение управления выходами порта GPIOD
  GPIO_PORTD_DIR_R = 0xFF;           // установка направления передачи GPIOD на выход
  GPIO_PORTD_DEN_R = 0xFF;           // подключение физической линии к порту

#if 1
  // Вывод константы на светодиоды
  GPIO_PORTD_DATA_R = 0x83; //0x80 + 0x02 + 0x01
  while(1);
#endif

#if 0
  // Мигание всеми светодиодами
  while(1)                           // бесконечный цикл
  {
    GPIO_PORTD_DATA_R = 0xFF;        // включение светодиодов
    Delay(100);                      // пауза
    GPIO_PORTD_DATA_R = 0x00;        // выключение светодиодов
    Delay(100);                      // пауза
  }    
#endif

#if 0
  // Эффект "бегущего огня"
  while(1)                           
  {
    char a, i;
    a = 0x01;
    for( i = 0; i < 8; i++ )
    {
      GPIO_PORTD_DATA_R = a;  // вывод a в порт управления светодиодами
      Delay(100);             // пауза        
      a <<= 1;                // сдвиг переменной a влево на один разряд
    }
  }
#endif
  
  // индивидуальное задание согласно номеру рабочего места
  
}
