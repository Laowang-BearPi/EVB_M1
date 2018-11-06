#include "dht11.h"
/***************************************************************
* �ļ�����: dht11
* ��    ��: ���������ֲ�
* ��    ��: V1.0
* ��д����: 2018-10-1
* ��    ��: ��ʪ�ȴ��������ݶ�ȡ
****************************************************************
* Ӳ��ƽ̨: �������������������ֲ�EVB_M1������ʹ��
* ��    ��: iot-club.taobao.com��https://shop449662000.taobao.com
* ��    ̳: www.iot-club.cn ��bbs.iot-club.cn
****************************************************************/
/***************************************************************
* ��������: DHT11_Delay
* ˵    ��: DHT11΢�������ʱ
* ��    ��: time��ʱʱ��
* �� �� ֵ: ��
***************************************************************/
void DHT11_Delay(uint16_t time)
{
	time *= 10;
	while(time)
		time--;
}

/***************************************************************
* ��������: DHT11_Mode_IPU
* ˵    ��: ʹDHT11-DATA���ű�Ϊ��������ģʽ
* ��    ��: ��
* �� �� ֵ: ��
***************************************************************/
void DHT11_Mode_IPU(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* �������蹦��GPIO���� */
  GPIO_InitStruct.Pin   = DHT11_Pin;
  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
	
}

/***************************************************************
* ��������: DHT11_Mode_Out_PP
* ˵    ��: ʹDHT11-DATA���ű�Ϊ�������ģʽ
* ��    ��: ��
* �� �� ֵ: ��
***************************************************************/
void DHT11_Mode_Out_PP(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* �������蹦��GPIO���� */
  GPIO_InitStruct.Pin = DHT11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct); 	 
}

/***************************************************************
* ��������: DHT11_Init
* ˵    ��: DHT11��ʼ��
* ��    ��: ��
* �� �� ֵ: ��
***************************************************************/
void DHT11_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	DHT11_Mode_Out_PP();
	DHT11_Dout_HIGH(); 
}

/***************************************************************
* ��������: DHT11_ReadByte
* ˵    ��: ��DHT11��ȡһ���ֽڣ�MSB����
* ��    ��: ��
* �� �� ֵ: һ�ֽ�����
***************************************************************/
uint8_t DHT11_ReadByte(void)
{
	uint8_t i, temp=0;
	/*ÿbit��50us�͵�ƽ���ÿ�ʼ����ѯֱ���ӻ�������50us�͵�ƽ����*/  
	/*DHT11 ��26~28us�ĸߵ�ƽ��ʾ��0������70us�ߵ�ƽ��ʾ��1��*/
	/*ͨ����� x us��ĵ�ƽ��������������״̬*/
	for(i=0;i<8;i++)    
	{	 
		while(DHT11_Data_IN()==GPIO_PIN_RESET);
		DHT11_Delay(50); 							//��ʱx us �����ʱ��Ҫ��������0������ʱ�伴��	   	  
		if(DHT11_Data_IN()==GPIO_PIN_SET)			// x us����Ϊ�ߵ�ƽ��ʾ���ݡ�1��
		{
			while(DHT11_Data_IN()==GPIO_PIN_SET);	//�ȴ�����1�ĸߵ�ƽ����	
			temp|=(uint8_t)(0x01<<(7-i));  			//�ѵ�7-iλ��1��MSB���� 
		}
		else										// x us��Ϊ�͵�ƽ��ʾ���ݡ�0��
		{			   
			temp&=(uint8_t)~(0x01<<(7-i)); 			//�ѵ�7-iλ��0��MSB����
		}
	}
	return temp;
}

/***************************************************************
* ��������: DHT11_Read_TempAndHumidity
* ˵    ��: һ�����������ݴ���Ϊ40bit����λ�ȳ�
*			8bitʪ������ + 8bit ʪ��С�� + 8bit �¶����� + 8bit �¶�С�� + 8bit У���
* ��    ��: *DHT11_Data��DHT11���ݽṹ��
* �� �� ֵ: ERROR��  ��ȡ����
  *         SUCCESS����ȡ�ɹ�
***************************************************************/
uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef *DHT11_Data)
{  
	uint8_t temp;
	uint16_t humi_temp;
	
	DHT11_Mode_Out_PP();		//���ģʽ
	DHT11_Dout_LOW();			//��������
	HAL_Delay(18);				//��ʱ18ms	
	DHT11_Dout_HIGH(); 			//��������
	DHT11_Delay(30);   			//��ʱ30us
	DHT11_Mode_IPU();			//������Ϊ���� �жϴӻ���Ӧ�ź� 
	/*�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź��粻��Ӧ����������Ӧ����������*/   
	if(DHT11_Data_IN()==GPIO_PIN_RESET)     
	{
		/*��ѯֱ���ӻ����� ��80us �͵�ƽ ��Ӧ�źŽ���*/  
		while(DHT11_Data_IN()==GPIO_PIN_RESET);

		/*��ѯֱ���ӻ������� 80us �ߵ�ƽ �����źŽ���*/
		while(DHT11_Data_IN()==GPIO_PIN_SET);//��ʼ��������
		DHT11_Data->humi_high8bit= DHT11_ReadByte();
		DHT11_Data->humi_low8bit = DHT11_ReadByte();
		DHT11_Data->temp_high8bit= DHT11_ReadByte();
		DHT11_Data->temp_low8bit = DHT11_ReadByte();
		DHT11_Data->check_sum    = DHT11_ReadByte();
		DHT11_Mode_Out_PP();	//��ȡ���������Ÿ�Ϊ���ģʽ
		DHT11_Dout_HIGH();		//��������
								//�����ݽ��д���
		humi_temp=DHT11_Data->humi_high8bit*100+DHT11_Data->humi_low8bit;
		DHT11_Data->humidity =(float)humi_temp/100;

		humi_temp=DHT11_Data->temp_high8bit*100+DHT11_Data->temp_low8bit;
		DHT11_Data->temperature=(float)humi_temp/100;    
								//����ȡ�������Ƿ���ȷ
		temp = DHT11_Data->humi_high8bit + DHT11_Data->humi_low8bit + 
		DHT11_Data->temp_high8bit+ DHT11_Data->temp_low8bit;
		if(DHT11_Data->check_sum==temp)
		{
			return SUCCESS;
		}
		else 
			return ERROR;
	}	
	else
		return ERROR;
}
