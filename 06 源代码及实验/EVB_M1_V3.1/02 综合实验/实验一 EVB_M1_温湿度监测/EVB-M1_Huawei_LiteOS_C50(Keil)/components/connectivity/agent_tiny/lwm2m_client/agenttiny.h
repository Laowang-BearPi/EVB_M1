/*----------------------------------------------------------------------------
 * Copyright (c) <2018>, <Huawei Technologies Co., Ltd>
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

/**@defgroup agent AgentTiny
 * @defgroup agenttiny Agenttiny Definition
 * @ingroup agent
 */
#ifndef AGENT_TINY_H
#define AGENT_TINY_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************The following interfaces are implemented by user***********************/

typedef enum
{
    ATINY_GET_BINDING_MODES,
    ATINY_GET_MODEL_NUMBER,
    ATINY_DO_DEV_REBOOT,
    ATINY_GET_MIN_VOLTAGE,
    ATINY_GET_MAX_VOLTAGE,
    ATINY_GET_SERIAL_NUMBER,
    ATINY_GET_BATERRY_LEVEL,
    ATINY_GET_MEMORY_FREE,
    ATINY_GET_DEV_ERR,
    //ATINY_GET_CURRENT_TIME,
    //ATINY_GET_UTC_OFFSET,
    //ATINY_GET_TIMEZONE,
    ATINY_GET_POWER_CURRENT_1,
    ATINY_GET_POWER_CURRENT_2,
    ATINY_GET_POWER_SOURCE_1,
    ATINY_GET_POWER_SOURCE_2,
    ATINY_DO_FACTORY_RESET,
    ATINY_GET_FIRMWARE_VER,
    ATINY_TRIG_FIRMWARE_UPDATE,
    ATINY_GET_FIRMWARE_STATE,
    ATINY_GET_FIRMWARE_RESULT,
    ATINY_GET_NETWORK_BEARER,
    ATINY_GET_SIGNAL_STRENGTH,
    ATINY_GET_CELL_ID,
    ATINY_GET_LINK_QUALITY,
    ATINY_GET_LINK_UTILIZATION,
    ATINY_WRITE_APP_DATA,
    ATINY_UPDATE_PSK,
} atiny_cmd_e;

/**
 *@ingroup agenttiny
 *@brief issue the command.
 *
 *@par Description:
 *This API is used to issue the command.
 *@attention none.
 *
 *@param cmd            [IN]     The command to be issued. @ref atiny_cmd_e.
 *@param arg            [IN/OUT] Buffer to store the command parameters. Agent_tiny is responsible
                                 for memory allocation. if cmd is GET_XXX, arg is outcoming parameter
                                 and this buffer is filled by device; else arg is incoming parameter
                                 and this buffer is filled by agent_tiny.
 *@param len            [IN]     The length of the argument.
 *
 *@retval #int          0 if succeed, or the error number @ref atiny_error_e if failed.
 *@par Dependency: none.
 *@see none.
 */
int atiny_cmd_ioctl(atiny_cmd_e cmd, char* arg, int len);

typedef enum
{
    ATINY_REG_OK,
    ATINY_REG_FAIL,
    ATINY_DATA_SUBSCRIBLE,
    ATINY_DATA_UNSUBSCRIBLE,
} atiny_event_e;

/**
 *@ingroup agenttiny
 *@brief issue the command.
 *
 *@par Description:
 *This API is used to issue the command.
 *@attention none.
 *
 *@param stat           [IN] The event to be issued. @ref atiny_event_e.
 *@param arg            [IN] Buffer to store the event parameters.
 *@param len            [IN] The length of the argument.
 *
 *@retval #int          0 if succeed, or the error number @ref atiny_error_e if failed.
 *@par Dependency: none.
 *@see none.
 */
void atiny_event_notify(atiny_event_e event, const char* arg, int len);

/****************The following interfaces are implemented by agent_tiny*******************/

typedef struct
{
    char* binding;               /*Ŀǰ֧��U����UQ*/
    int   life_time;             /*��ѡ��Ĭ��50000,����̣���Ƶ������update���ģ������������״̬����ʱ�䳤*/
    unsigned int  storing_cnt;   /*storingΪtrueʱ��lwm2m���������ֽڸ���*/
} atiny_server_param_t;

typedef struct
{
    bool  is_bootstrap;
    char* server_ip;
    char* server_port;
    char* psk_Id;
    char* psk;
    unsigned short psk_len;
} atiny_security_param_t;

typedef enum
{
    FIRMWARE_UPDATE_STATE = 0,
    APP_DATA
} atiny_report_type_e;

typedef struct
{
    atiny_server_param_t   server_params;
    atiny_security_param_t security_params[2];
} atiny_param_t;

typedef struct
{
    char* endpoint_name;
    char* manufacturer;
    char* dev_type;
} atiny_device_info_t;

/**
 *@ingroup agenttiny
 *@brief initialize the lwm2m protocal.
 *
 *@par Description:
 *This API is used to initialize the lwm2m protocal.
 *@attention none.
 *
 *@param atiny_params   [IN]  Configure parameters of lwm2m.
 *@param phandle        [OUT] The handle of the agent_tiny.
 *
 *@retval #int          0 if succeed, or the error number @ref atiny_error_e if failed.
 *@par Dependency: none.
 *@see atiny_bind | atiny_deinit.
 */
int atiny_init(atiny_param_t* atiny_params, void** phandle);

/**
 *@ingroup agenttiny
 *@brief main task of the lwm2m protocal.
 *
 *@par Description:
 *This API is used to implement the lwm2m protocal, and interactive with lwm2m server.
 *@attention none.
 *
 *@param device_info    [IN] The information of devices to be bound.
 *@param phandle        [IN] The handle of the agent_tiny.
 *
 *@retval #int          0 if succeed, or the error number @ref atiny_error_e if failed.
 *@par Dependency: none.
 *@see atiny_init | atiny_deinit.
 */
int atiny_bind(atiny_device_info_t* device_info, void* phandle);

/**
 *@ingroup agenttiny
 *@brief stop the device and release resources.
 *
 *@par Description:
 *This API is used to stop the device and release resources.
 *@attention none.
 *
 *@param phandle        [IN] The handle of the agent_tiny.
 *
 *@retval none.
 *@par Dependency: none.
 *@see atiny_init | atiny_bind.
 */
void atiny_deinit(void* phandle);

#define MAX_REPORT_DATA_LEN      1024
#define MAX_BUFFER_REPORT_CNT    8
#define MAX_SEND_ERR_NUM 10
#define MAX_RECV_ERR_NUM 10

typedef enum
{
    NOT_SENT = 0,
    SENT_WAIT_RESPONSE,
    SENT_FAIL,
    SENT_TIME_OUT,
    SENT_SUCCESS,
    SENT_GET_RST,
    SEND_PENDING,
} data_send_status_e;

typedef void (*atiny_ack_callback) (atiny_report_type_e type, int cookie, data_send_status_e status);

typedef struct _data_report_t
{
    atiny_report_type_e type;     /*�����ϱ�����*/
    int cookie;                   /*����cookie,������ack�ص��У����ֲ�ͬ������*/
    int len;                      /*���ݳ��ȣ���Ӧ����MAX_REPORT_DATA_LEN*/
    uint8_t* buf;                 /*���ݻ������׵�ַ*/
    atiny_ack_callback callback;  /*ack�ص�*/
} data_report_t;

/**
 *@ingroup agenttiny
 *@brief report the data to the server.
 *
 *@par Description:
 *This API is used to report the data to the server.
 *@attention none.
 *
 *@param phandle        [IN] The handle of the agent_tiny.
 *@param report_data    [IN] Data to be reported.
 *
 *@retval #ATINY_OK           Report succeed.
 *@retval #ATINY_MSG_CONGEST  The lwm2m protocal stack is congested. It is occured when
                              the number of report messages are greater than storing_cnt
                              in atiny_server_param_t.
 *@retval #ATINY_BUF_OVERFLOW The length of the buffer in report_data is exceeded
                              MAX_REPORT_DATA_LEN.
 *@par Dependency: none.
 *@see none.
 */
int atiny_data_report(void* phandle, data_report_t* report_data);

typedef enum
{
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERR,
    LOG_FATAL,

    LOG_MAX
} atiny_log_e;

/**
 *@ingroup agenttiny
 *@brief set log level.
 *
 *@par Description:
 *This API is used to set log level. The log informations whose level is not less than
  the level set up in this interface are displayed.
 *@attention none.
 *
 *@param level          [IN] Log level to be set up.
 *
 *@retval none.
 *@par Dependency: none.
 *@see atiny_get_log_level.
 */
void atiny_set_log_level(atiny_log_e level);

/**
 *@ingroup agenttiny
 *@brief get log level.
 *
 *@par Description:
 *This API is used to get log level set by atiny_set_log_level.
 *@attention none.
 *
 *@param none.
 *
 *@retval #atiny_log_e  Log level.
 *@par Dependency: none.
 *@see atiny_set_log_level.
 */
atiny_log_e atiny_get_log_level(void);

/**
 *@ingroup agenttiny
 *@brief reconnect lwm2m server.
 *
 *@par Description:
 *This API is used to reconnect lwm2m server.
 *@attention none.
 *
 *@param phandle        [IN] The handle of the agent_tiny.
 *
 *@retval #int          0 if succeed, or the error number @ref atiny_error_e if failed.
 *@par Dependency: none.
 *@see atiny_init | atiny_deinit.
 */
int atiny_reconnect(void* phandle);

typedef enum
{
    ATINY_OK                   = 0,
    ATINY_ARG_INVALID          = -1,
    ATINY_BUF_OVERFLOW         = -2,
    ATINY_MSG_CONGEST          = -3,
    ATINY_MALLOC_FAILED        = -4,
    ATINY_RESOURCE_NOT_FOUND   = -5,
    ATINY_RESOURCE_NOT_ENOUGH  = -6,
    ATINY_CLIENT_UNREGISTERED  = -7,
    ATINY_SOCKET_CREATE_FAILED = -8,
} atiny_error_e;


#ifdef __cplusplus
}
#endif

#endif

