/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "osapi.h"

#include "user_interface.h"
#include "ip_addr.h"
#include "mem.h"
#include "espconn.h"
#include "driver/hw_wrapper.h"

#define SPI_FLASH_SIZE_MAP 4
#if ((SPI_FLASH_SIZE_MAP == 0) || (SPI_FLASH_SIZE_MAP == 1))
#error "The flash map is not supported"
#elif (SPI_FLASH_SIZE_MAP == 2)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x81000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0xfb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0xfc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0xfd000
#define SYSTEM_PARTITION_CUSTOMER_PRIV_PARAM_ADDR           0x7c000
#elif (SPI_FLASH_SIZE_MAP == 3)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x81000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x1fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x1fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x1fd000
#define SYSTEM_PARTITION_CUSTOMER_PRIV_PARAM_ADDR           0x7c000
#elif (SPI_FLASH_SIZE_MAP == 4)
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x3fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x3fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x3fd000
#elif (SPI_FLASH_SIZE_MAP == 5)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x101000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x1fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x1fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x1fd000
#define SYSTEM_PARTITION_CUSTOMER_PRIV_PARAM_ADDR           0xfc000
#elif (SPI_FLASH_SIZE_MAP == 6)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x101000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x3fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x3fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x3fd000
#define SYSTEM_PARTITION_CUSTOMER_PRIV_PARAM_ADDR           0xfc000
#else
#error "The flash map is not supported"
#endif


static const partition_item_t at_partition_table[] = {
    { SYSTEM_PARTITION_BOOTLOADER, 						0x0, 												0x1000},
    { SYSTEM_PARTITION_RF_CAL,  						SYSTEM_PARTITION_RF_CAL_ADDR, 						0x1000},
    { SYSTEM_PARTITION_PHY_DATA, 						SYSTEM_PARTITION_PHY_DATA_ADDR, 					0x1000},
    { SYSTEM_PARTITION_SYSTEM_PARAMETER, 				SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR, 			0x3000},
};

void ICACHE_FLASH_ATTR user_pre_init(void)
{
    if(!system_partition_table_regist(at_partition_table, sizeof(at_partition_table)/sizeof(at_partition_table[0]),SPI_FLASH_SIZE_MAP)) {
		os_printf("system_partition_table_regist fail\r\n");
		while(1);
	}
}

struct espconn user_tcp_espconn;

void ICACHE_FLASH_ATTR server_recv(void *arg, char *pdata, unsigned short len) {
	os_printf("�յ�PC���������ݣ�%s", pdata);
	espconn_sent((struct espconn *) arg, "�Ѿ��յ�����", strlen("�Ѿ��յ�����"));

}
void ICACHE_FLASH_ATTR server_sent(void *arg) {
	os_printf("���ͳɹ���");
}
void ICACHE_FLASH_ATTR server_discon(void *arg) {
	os_printf("�����Ѿ��Ͽ���");
}

void ICACHE_FLASH_ATTR server_listen(void *arg)  //ע�� TCP ���ӳɹ�������Ļص�����
{
	struct espconn *pespconn = arg;
	espconn_regist_recvcb(pespconn, server_recv);  //����
	espconn_regist_sentcb(pespconn, server_sent);  //����
	espconn_regist_disconcb(pespconn, server_discon);  //�Ͽ�
}
void ICACHE_FLASH_ATTR server_recon(void *arg, sint8 err) //ע�� TCP ���ӷ����쳣�Ͽ�ʱ�Ļص������������ڻص������н�������
{
	os_printf("���Ӵ��󣬴������Ϊ��%d\r\n", err); //%d,�������ʮ��������
}


void tcp_server_init(uint32_t local_port) {
	user_tcp_espconn.proto.tcp = (esp_tcp *) os_zalloc(sizeof(esp_tcp)); //����ռ�
	user_tcp_espconn.type = ESPCONN_TCP; //��������ΪTCPЭ��
	user_tcp_espconn.proto.tcp->local_port = local_port; //���ض˿�

	espconn_regist_connectcb(&user_tcp_espconn, server_listen); //ע�� TCP ���ӳɹ�������Ļص�����
	espconn_regist_reconcb(&user_tcp_espconn, server_recon); //ע�� TCP ���ӷ����쳣�Ͽ�ʱ�Ļص������������ڻص������н�������
	espconn_accept(&user_tcp_espconn); //���� TCP server����������
	espconn_regist_time(&user_tcp_espconn, 180, 0); //���ó�ʱ�Ͽ�ʱ�� ��λ���룬���ֵ��7200 ��

}


void ICACHE_FLASH_ATTR tcp_server_test()
{
	os_printf("tcp_server_test\n");
	tcp_server_init(10000);
}

#define		ESP8266_STA_SSID	"WIRELESS_LINK" /* wifi ���� */
#define		ESP8266_STA_PASS	"12345678"		/* wifi ���� */

static void wifi_handle_event_cb(System_Event_t *evt)
{
	os_printf("------wifi_handle_event_cb %d---\n",evt->event);
	switch (evt->event)
	{
	case EVENT_STAMODE_CONNECTED:/*������·���� */

		os_printf("Connected to SSID %s, Channel %d\n",
		  evt->event_info.connected.ssid,
		  evt->event_info.connected.channel);
	  break;

	case EVENT_STAMODE_DISCONNECTED:/* ��·�����Ͽ� */

		os_printf("Disconnect from SSID %s, reason %d\n",
		  evt->event_info.disconnected.ssid,
		  evt->event_info.disconnected.reason);
	  break;

	case EVENT_STAMODE_AUTHMODE_CHANGE:
		os_printf("mode: %u -> %u\n",
		  evt->event_info.auth_change.old_mode,
		  evt->event_info.auth_change.new_mode);
	  break;

	case EVENT_STAMODE_GOT_IP:/* ������·����,����ȡ��IP */
		os_printf("IP:" IPSTR ",Mask:" IPSTR ",GW:" IPSTR "\n",
		  IP2STR(&evt->event_info.got_ip.ip),
		  IP2STR(&evt->event_info.got_ip.mask),
		  IP2STR(&evt->event_info.got_ip.gw));

		tcp_server_test();
	  break;

	case EVENT_STAMODE_DHCP_TIMEOUT:/* ������·����,����·������WIFIģ�����IP����Ϣ��ʱ�� */
		os_printf("STA MODE_DHCP_TIMEOUT\n");
	  break;

	case EVENT_SOFTAPMODE_STACONNECTED: /* APģʽ��,���豸����WIFIģ������� */

		os_printf("Station: " MACSTR "join, AID = %d\n",
		  MAC2STR(evt->event_info.sta_connected.mac),
		  evt->event_info.sta_connected.aid);
	  break;

	case EVENT_SOFTAPMODE_STADISCONNECTED: /* APģʽ��,���豸�Ͽ���WIFIģ����������� */

		os_printf("station: " MACSTR "leave, AID = %d\n",
		  MAC2STR(evt->event_info.sta_disconnected.mac),
		  evt->event_info.sta_disconnected.aid);
	  break;

	case EVENT_SOFTAPMODE_PROBEREQRECVED:

		os_printf("Station PROBEREQ: " MACSTR " RSSI = %d\n",
		  MAC2STR(evt->event_info.ap_probereqrecved.mac),
		  evt->event_info.ap_probereqrecved.rssi);
	  break;

	default:
	  break;
	}

	os_printf("-----------------------------\n");
}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
user_init(void)
{
	struct station_config sta_config;
	uart_init(BIT_RATE_115200,BIT_RATE_115200);

	HW_DEBUG("\n---------------------------\n");
	HW_DEBUG("This is wifi station mode test\n");
	HW_DEBUG("---------------------------\n");

	wifi_set_opmode(STATION_MODE);
	os_memset(&sta_config, 0, sizeof(struct station_config));
	os_strcpy(sta_config.ssid,ESP8266_STA_SSID);
	os_strcpy(sta_config.password,ESP8266_STA_PASS);

	wifi_station_set_config(&sta_config);
	wifi_set_event_handler_cb(wifi_handle_event_cb);

}

