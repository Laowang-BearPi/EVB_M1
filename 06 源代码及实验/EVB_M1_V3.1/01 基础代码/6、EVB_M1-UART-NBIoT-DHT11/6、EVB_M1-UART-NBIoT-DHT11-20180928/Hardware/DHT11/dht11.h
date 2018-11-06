#ifndef __DHT11_H__
#define __DHT11_H__

#include "stm32l4xx_hal.h"

//---------------------�����˿ڶ���---------------------------// 
#define DHT11_Dout_LOW()		HAL_GPIO_WritePin(DHT11_GPIO_Port,DHT11_Pin,GPIO_PIN_RESET) 
#define DHT11_Dout_HIGH()		HAL_GPIO_WritePin(DHT11_GPIO_Port,DHT11_Pin,GPIO_PIN_SET)
#define DHT11_Data_IN()			HAL_GPIO_ReadPin(DHT11_GPIO_Port,DHT11_Pin)

/***************************************************************
* ��������: DHT11_Data_TypeDef
* ˵    ��: DHT11�ṹ��
* ��    ��: ��
* �� �� ֵ: ��
***************************************************************/
typedef struct
{
	uint8_t  humi_high8bit;		//ԭʼ���ݣ�ʪ�ȸ�8λ
	uint8_t  humi_low8bit;	 	//ԭʼ���ݣ�ʪ�ȵ�8λ
	uint8_t  temp_high8bit;	 	//ԭʼ���ݣ��¶ȸ�8λ
	uint8_t  temp_low8bit;	 	//ԭʼ���ݣ��¶ȸ�8λ
	uint8_t  check_sum;	 	    //У���
	float    humidity;        //ʵ��ʪ��
	float    temperature;     //ʵ���¶�  
} DHT11_Data_TypeDef;

/***************************************************************
* ��������: msg_for_DHT11
* ˵    ��: DHT11�������ݵĽṹ�壨NBʹ�ã�
* ��    ��: ��
* �� �� ֵ: ��
***************************************************************/
typedef struct
{
	char temp[4];
	char hum[4];	
}msg_for_DHT11;

/**************************************************************/
/*****************         DHT11�ӿ�        *******************/
/**************************************************************/
void DHT11_Init(void);
uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef * DHT11_Data);
#endif
