/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

#include "agent_tiny_demo.h"

//#define DEFAULT_SERVER_IPV4 "139.159.209.89"/*Huawei */
#define DEFAULT_SERVER_IPV4 "180.101.147.115"/*dianxin */
#define LWM2M_LIFE_TIME     50000

char * g_endpoint_name = "20180702";
#ifdef WITH_DTLS
char * g_endpoint_name_s = "20180626";
unsigned char g_psk_value[16] = {0x16,0x96,0x14,0x6e,0xfe,0x24,0x8f,0x40,0x5f,0xd5,0x2e,0x82,0x6e,0x66,0xd7,0x97};
#endif

static void* g_phandle = NULL;
static atiny_device_info_t g_device_info;
static atiny_param_t g_atiny_params;

DHT11_Data_TypeDef  DHT11_Data;
msg_for_Wifi Wifi_send;

void ack_callback(atiny_report_type_e type, int cookie, data_send_status_e status)
{
    printf("type:%d cookie:%d status:%d\n", type,cookie, status);
}

void app_data_report(void)
{
		UINT32 uwRet = LOS_OK;
    data_report_t report_data;
    int ret;
    int cnt = 0;
    report_data.buf = Wifi_send.buf;
    report_data.callback = ack_callback;
    report_data.cookie = 0;
    report_data.len = sizeof(Wifi_send.buf);
    report_data.type = APP_DATA;
	  LOS_TaskDelay(1500);
    while(1)
    {
        report_data.cookie = cnt;
        cnt++;
        ret = atiny_data_report(g_phandle, &report_data);
        printf("report ret:%d\n",ret);
        uwRet=LOS_TaskDelay(250*8);
				if(uwRet !=LOS_OK)
				return;
    }
}

UINT32 creat_report_task()
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;
    UINT32 TskHandle;

    task_init_param.usTaskPrio = 1;
    task_init_param.pcName = "app_data_report";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)app_data_report;
    task_init_param.uwStackSize = 0x1000;

    uwRet = LOS_TaskCreate(&TskHandle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;

}
VOID data_collection_task(VOID)
{
	UINT32 uwRet = LOS_OK;
	short int Lux;
	
	while (1)
  {

	  /****************temperature and humidity*****************/
    if(DHT11_Read_TempAndHumidity(&DHT11_Data)==SUCCESS)
    {
			printf("read DHT11 suc!-->sd is %.1f £¥RH £¬wd is %.1f¡æ \n",DHT11_Data.humidity,DHT11_Data.temperature);
    }
    else
    {
      printf("read DHT11 fialed\n");
			DHT11_Init();      
    }
				/****************BH1750******************/
		Lux=(int)Convert_BH1750();
	  printf("\r\n******************************BH1750 Value is  %d\r\n",Lux);
		
    sprintf((char*)Wifi_send.buf, "%.1f%.1f%5d", DHT11_Data.temperature,DHT11_Data.humidity,Lux);
		
		uwRet=LOS_TaskDelay(1500);
		if(uwRet !=LOS_OK)
		return;
	
  }
}
UINT32 creat_data_collection_task()
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;
    UINT32 TskHandle;

    task_init_param.usTaskPrio = 0;
    task_init_param.pcName = "data_collection_task";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)data_collection_task;
    task_init_param.uwStackSize = 0x1000;

    uwRet = LOS_TaskCreate(&TskHandle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;
}
void agent_tiny_entry(void)
{
    UINT32 uwRet = LOS_OK;
    atiny_param_t* atiny_params;
    atiny_security_param_t  *security_param = NULL;
    atiny_device_info_t *device_info = &g_device_info;
    if(NULL == device_info)
    {
        return;
    }
#ifdef WITH_DTLS
    device_info->endpoint_name = g_endpoint_name_s;
#else
    device_info->endpoint_name = g_endpoint_name;
#endif
    device_info->manufacturer = "Agent_Tiny";

    atiny_params = &g_atiny_params;
    atiny_params->server_params.binding = "UQ";
    //atiny_params->server_params.life_time = LWM2M_LIFE_TIME;
    atiny_params->server_params.life_time = 20;
    atiny_params->server_params.storing_cnt = 0;

    security_param = &(atiny_params->security_params[0]);

    security_param->bootstrap_mode = BOOTSTRAP_FACTORY;
    security_param->iot_server_ip = DEFAULT_SERVER_IPV4;
    security_param->bs_server_ip = DEFAULT_SERVER_IPV4;

#ifdef WITH_DTLS
    security_param->iot_server_port = "5684";
    security_param->bs_server_port = "5684";

    security_param->psk_Id = g_endpoint_name_s;
    security_param->psk = (char*)g_psk_value;
    security_param->psk_len = 16;
#else
    security_param->iot_server_port = "5683";
    security_param->bs_server_port = "5683";

    security_param->psk_Id = NULL;
    security_param->psk = NULL;
    security_param->psk_len = 0;
#endif

    if(ATINY_OK != atiny_init(atiny_params, &g_phandle))
    {
        return;
    }

    uwRet = creat_report_task();
    if(LOS_OK != uwRet)
    {
        return;
    }
		uwRet = creat_data_collection_task();
    if(LOS_OK != uwRet)
    {
        return ;
    }
    (void)atiny_bind(device_info, g_phandle);
}
