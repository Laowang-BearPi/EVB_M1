#ifndef __KEY_H_
#define	__KEY_H_

#include "stm32l4xx_hal.h"

//---------------------按键端口定义---------------------------// 
#define KEY1 HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3)//读取按键1，即F1按键
#define KEY2 HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)//读取按键2，即F2按键
#define KEY3 HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)//读取按键3，即F3按键
#define KEY4 HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0)//读取按键4，即F4按键

//---------------------声明按键外部变量----------------------// 
extern uint8_t keyValue;

/***************************************************************
* 函数名称: KEY_VALUE
* 说    明：KEYx枚举结构体
* 参    数: 无
* 返 回 值: 无
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
/**************         KEY接口        ************************/
/**************************************************************/
uint8_t KeyScan(void);
#endif
/**************************END OF FILE*************************/
