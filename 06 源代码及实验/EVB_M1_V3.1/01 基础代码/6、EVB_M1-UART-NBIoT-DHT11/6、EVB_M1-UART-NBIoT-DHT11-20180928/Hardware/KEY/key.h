#ifndef __KEY_H_
#define	__KEY_H_

#include "stm32l4xx_hal.h"

//---------------------�����˿ڶ���---------------------------// 
#define KEY1 HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3)//��ȡ����1����F1����
#define KEY2 HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)//��ȡ����2����F2����
#define KEY3 HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)//��ȡ����3����F3����
#define KEY4 HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0)//��ȡ����4����F4����

//---------------------���������ⲿ����----------------------// 
extern uint8_t keyValue;

/***************************************************************
* ��������: KEY_VALUE
* ˵    ����KEYxö�ٽṹ��
* ��    ��: ��
* �� �� ֵ: ��
***************************************************************/
typedef enum
{
	KEY_VALUE_NONE = 0,
	KEY1_VALUE,
	KEY2_VALUE,
	KEY3_VALUE,
	KEY4_VALUE
}KEY_VALUE;

/**************************************************************/
/**************         KEY�ӿ�        ************************/
/**************************************************************/
uint8_t KeyScan(void);
#endif
/**************************END OF FILE*************************/
