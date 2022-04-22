#include "lm4f232h5qd.h"

void Delay(long i)        // �������� �� i ������ ������
{
  while(i > 0)
  {
    i--;
  }
}

void main()
{    
  SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOD; // ��������� ������������ ����� GPIOD
  GPIO_PORTD_LOCK_R = 0x4C4F434B;    // ������ ���������� �������� GPIOCR
  GPIO_PORTD_CR_R = 0xFF;            // ���������� ���������� �������� ����� GPIOD
  GPIO_PORTD_DIR_R = 0xFF;           // ��������� ����������� �������� GPIOD �� �����
  GPIO_PORTD_DEN_R = 0xFF;           // ����������� ���������� ����� � �����

#if 1
  // ����� ��������� �� ����������
  GPIO_PORTD_DATA_R = 0x83; //0x80 + 0x02 + 0x01
  while(1);
#endif

#if 0
  // ������� ����� ������������
  while(1)                           // ����������� ����
  {
    GPIO_PORTD_DATA_R = 0xFF;        // ��������� �����������
    Delay(100);                      // �����
    GPIO_PORTD_DATA_R = 0x00;        // ���������� �����������
    Delay(100);                      // �����
  }    
#endif

#if 0
  // ������ "�������� ����"
  while(1)                           
  {
    char a, i;
    a = 0x01;
    for( i = 0; i < 8; i++ )
    {
      GPIO_PORTD_DATA_R = a;  // ����� a � ���� ���������� ������������
      Delay(100);             // �����        
      a <<= 1;                // ����� ���������� a ����� �� ���� ������
    }
  }
#endif
  
  // �������������� ������� �������� ������ �������� �����
  
}
