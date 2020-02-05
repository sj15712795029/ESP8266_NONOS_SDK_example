/* main.c -- MQTT client example
*
* Copyright (c) 2014-2015, Tuan PM <tuanpm at live dot com>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
* * Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
* * Neither the name of Redis nor the names of its contributors may be used
* to endorse or promote products derived from this software without
* specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/

#include "ets_sys.h"
#include "driver/uart.h"
#include "osapi.h"
#include "mqtt.h"
#include "config.h"
#include "debug.h"
#include "gpio.h"
#include "user_interface.h"
#include "mem.h"


#define SPI_FLASH_SIZE_MAP 4
#if ((SPI_FLASH_SIZE_MAP == 0) || (SPI_FLASH_SIZE_MAP == 1))
#error "The flash map is not supported"
#elif (SPI_FLASH_SIZE_MAP == 2)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x81000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0xfb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0xfc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0xfd000
#elif (SPI_FLASH_SIZE_MAP == 3)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x81000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x1fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x1fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x1fd000
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
#elif (SPI_FLASH_SIZE_MAP == 6)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x101000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x3fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x3fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x3fd000
#else
#error "The flash map is not supported"
#endif

MQTT_Client mqttClient;


void mqttConnectedCb(uint32_t *args)
{
    MQTT_Client* client = (MQTT_Client*)args;
    INFO("MQTT: Connected\r\n");
    MQTT_Subscribe(client, "mqtt_test1", 0);
    MQTT_Subscribe(client, "mqtt_test2", 1);
    MQTT_Subscribe(client, "mqtt_test3", 2);

    MQTT_Publish(client, "mqtt_test1", "hello1", 6, 0, 0);
    MQTT_Publish(client, "mqtt_test2", "hello2", 6, 1, 0);
    MQTT_Publish(client, "mqtt_test3", "hello3", 6, 2, 0);

}

void mqttDisconnectedCb(uint32_t *args)
{
    MQTT_Client* client = (MQTT_Client*)args;
    INFO("MQTT: Disconnected\r\n");
}

void mqttPublishedCb(uint32_t *args)
{
    MQTT_Client* client = (MQTT_Client*)args;
    INFO("MQTT: Published\r\n");
}

void mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len)
{
    char *topicBuf = (char*)os_zalloc(topic_len+1),
            *dataBuf = (char*)os_zalloc(data_len+1);

    MQTT_Client* client = (MQTT_Client*)args;

    os_memcpy(topicBuf, topic, topic_len);
    topicBuf[topic_len] = 0;

    os_memcpy(dataBuf, data, data_len);
    dataBuf[data_len] = 0;

    INFO("Receive topic: %s, data: %s \r\n", topicBuf, dataBuf);

    os_free(topicBuf);
    os_free(dataBuf);
}

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

		MQTT_Connect(&mqttClient);
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

void user_init(void)
{
	struct station_config sta_config;
    uart_init(BIT_RATE_115200, BIT_RATE_115200);
    os_delay_us(60000);

    CFG_Load();

    MQTT_InitConnection(&mqttClient, sysCfg.mqtt_host, sysCfg.mqtt_port, sysCfg.security);
    //MQTT_InitConnection(&mqttClient, "192.168.11.122", 1880, 0);

    MQTT_InitClient(&mqttClient, sysCfg.device_id, sysCfg.mqtt_user, sysCfg.mqtt_pass, sysCfg.mqtt_keepalive, 1);
    //MQTT_InitClient(&mqttClient, "client_id", "user", "pass", 120, 1);

	/* 如果云端没有对应的遗嘱主题，调试日志会调试连接上然后立刻断开 */
    //MQTT_InitLWT(&mqttClient, "/lwt", "offline", 0, 0);
    
    MQTT_OnConnected(&mqttClient, mqttConnectedCb);
    MQTT_OnDisconnected(&mqttClient, mqttDisconnectedCb);
    MQTT_OnPublished(&mqttClient, mqttPublishedCb);
    MQTT_OnData(&mqttClient, mqttDataCb);

	wifi_set_event_handler_cb(wifi_handle_event_cb);
	wifi_set_opmode(STATION_MODE);
	os_memset(&sta_config, 0, sizeof(struct station_config));
	os_strcpy(sta_config.ssid,sysCfg.sta_ssid);
	os_strcpy(sta_config.password,sysCfg.sta_pwd);

	wifi_station_set_config(&sta_config);
	
    INFO("\r\nSystem started ...\r\n");
}
