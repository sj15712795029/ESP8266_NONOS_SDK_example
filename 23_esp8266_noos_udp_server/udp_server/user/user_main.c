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


struct espconn user_udp_espconn;
void ICACHE_FLASH_ATTR user_udp_sent_cb(void *arg)   //发送
{
	os_printf("\r\n发送成功！\r\n");

}

void ICACHE_FLASH_ATTR user_udp_recv_cb(void *arg,    //接收
		char *pdata, unsigned short len) {
	os_printf("接收数据：%s", pdata);

	//每次发送数据确保端口参数不变
	user_udp_espconn.proto.udp = (esp_udp *) os_zalloc(sizeof(esp_udp));
	user_udp_espconn.type = ESPCONN_UDP;
	user_udp_espconn.proto.udp->local_port = 10000;
	user_udp_espconn.proto.udp->remote_port = 10000;
	const char udp_remote_ip[4] = { 255, 255, 255, 255 };
	os_memcpy(user_udp_espconn.proto.udp->remote_ip, udp_remote_ip, 4);

	espconn_sent((struct espconn *) arg, "已经收到啦！", strlen("已经收到啦!"));
}

void ICACHE_FLASH_ATTR udp_client_test()
{
	wifi_set_broadcast_if(0x01);	 //设置 ESP8266 发送 UDP广播包时，从 station 接口发送
	user_udp_espconn.proto.udp = (esp_udp *) os_zalloc(sizeof(esp_udp));//分配空间
	user_udp_espconn.type = ESPCONN_UDP;	 		  //设置类型为UDP协议
	user_udp_espconn.proto.udp->local_port = 10000;	 		  //本地端口
	user_udp_espconn.proto.udp->remote_port = 10000;	 		  //目标端口
	const char udp_remote_ip[4] = { 255, 255, 255, 255 };	 	//目标IP地址（广播）
	os_memcpy(user_udp_espconn.proto.udp->remote_ip, udp_remote_ip, 4);

	espconn_regist_recvcb(&user_udp_espconn, user_udp_recv_cb);	 		//接收
	espconn_regist_sentcb(&user_udp_espconn, user_udp_sent_cb);	 		//发送
	espconn_create(&user_udp_espconn);	 		  //建立 UDP 传输
	espconn_sent(&user_udp_espconn, "连接服务器", strlen("连接服务器"));

}

#define		ESP8266_STA_SSID	"WIRELESS_LINK" /* wifi 名称 */
#define		ESP8266_STA_PASS	"12345678"		/* wifi 密码 */

static void wifi_handle_event_cb(System_Event_t *evt)
{
	os_printf("------wifi_handle_event_cb %d---\n",evt->event);
	switch (evt->event)
	{
	case EVENT_STAMODE_CONNECTED:/*连接上路由器 */

		os_printf("Connected to SSID %s, Channel %d\n",
		  evt->event_info.connected.ssid,
		  evt->event_info.connected.channel);
	  break;

	case EVENT_STAMODE_DISCONNECTED:/* 和路由器断开 */

		os_printf("Disconnect from SSID %s, reason %d\n",
		  evt->event_info.disconnected.ssid,
		  evt->event_info.disconnected.reason);
	  break;

	case EVENT_STAMODE_AUTHMODE_CHANGE:
		os_printf("mode: %u -> %u\n",
		  evt->event_info.auth_change.old_mode,
		  evt->event_info.auth_change.new_mode);
	  break;

	case EVENT_STAMODE_GOT_IP:/* 连接上路由器,并获取了IP */
		os_printf("IP:" IPSTR ",Mask:" IPSTR ",GW:" IPSTR "\n",
		  IP2STR(&evt->event_info.got_ip.ip),
		  IP2STR(&evt->event_info.got_ip.mask),
		  IP2STR(&evt->event_info.got_ip.gw));

		udp_client_test();
	  break;

	case EVENT_STAMODE_DHCP_TIMEOUT:/* 连接上路由器,但是路由器给WIFI模块分配IP等信息超时了 */
		os_printf("STA MODE_DHCP_TIMEOUT\n");
	  break;

	case EVENT_SOFTAPMODE_STACONNECTED: /* AP模式下,有设备连接WIFI模块的无线 */

		os_printf("Station: " MACSTR "join, AID = %d\n",
		  MAC2STR(evt->event_info.sta_connected.mac),
		  evt->event_info.sta_connected.aid);
	  break;

	case EVENT_SOFTAPMODE_STADISCONNECTED: /* AP模式下,有设备断开和WIFI模块的无线连接 */

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

