#ifndef __DHT11_H__
#define __DHT11_H__

#include "stm32l4xx_hal.h"

//---------------------按键端口定义---------------------------// 
#define DHT11_Dout_LOW()		HAL_GPIO_WritePin(DHT11_GPIO_Port,DHT11_Pin,GPIO_PIN_RESET) 
#define DHT11_Dout_HIGH()		HAL_GPIO_WritePin(DHT11_GPIO_Port,DHT11_Pin,GPIO_PIN_SET)
#define DHT11_Data_IN()			HAL_GPIO_ReadPin(DHT11_GPIO_Port,DHT11_Pin)

/***************************************************************
* 函数名称: DHT11_Data_TypeDef
* 说    明: DHT11结构体
* 参    数: 无
* 返 回 值: 无
***************************************************************/
typedef struct
{
	uint8_t  humi_high8bit;		//原始数据：湿度高8位
	uint8_t  humi_low8bit;	 	//原始数据：湿度低8位
	uint8_t  temp_high8bit;	 	//原始数据：温度高8位
	uint8_t  temp_low8bit;	 	//原始数据：温度高8位
	uint8_t  check_sum;	 	    //校验和
	float    humidity;        //实际湿度
	float    temperature;     //实际温度  
} DHT11_Data_TypeDef;

/***************************************************************
* 函数名称: msg_for_DHT11
* 说    明: DHT11发送数据的结构体（NB使用）
* 参    数: 无
* 返 回 值: 无
***************************************************************/
typedef struct
{
	char temp[4];
	char hum[4];	
}msg_for_DHT11;

/**************************************************************/
/*****************         DHT11接口        *******************/
/**************************************************************/
void DHT11_Init(void);
uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef * DHT11_Data);
#endif
