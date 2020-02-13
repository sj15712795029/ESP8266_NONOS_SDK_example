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
#include "smartconfig.h"
#include "airkiss.h"
#include "driver/hw_dht11.h"



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


#define JSON_FORMAT "{\"reported\":{\"temperature\":%d.%d,\"humidity\":%d.%d}}"
MQTT_Client mqttClient;

os_timer_t mqtt_publish_timer;

void ICACHE_FLASH_ATTR mqtt_publish_timer_cb(void *timer_arg)
{
    os_printf("mqtt_publish_timer_cb\n");
    uint8_t json_buf[64] = {0};
    dht11_data_t dht11_data = {0};
    if(hw_dht11_read(&dht11_data) == HW_ERR_OK)
    {
        HW_DEBUG("湿度:%d.%d %%\n",dht11_data.humidity_h,dht11_data.humidity_l);
        if(dht11_data.negative)
            HW_DEBUG("温度度:-%d.%d *C\n",dht11_data.temperature_h,dht11_data.temperature_l);
        else
            HW_DEBUG("温度度:%d.%d *C\n",dht11_data.temperature_h,dht11_data.temperature_l);
    }
    else
        HW_DEBUG("dht11 read error\n");
    os_sprintf(json_buf,JSON_FORMAT,dht11_data.temperature_h,dht11_data.temperature_l,\
               dht11_data.humidity_h,dht11_data.humidity_l);
    MQTT_Publish(&mqttClient, "$baidu/iot/shadow/dht11_test/update", json_buf, os_strlen(json_buf), 0, 0);
}

void mqttConnectedCb(uint32_t *args)
{
    INFO("MQTT: Connected\r\n");

    os_timer_disarm(&mqtt_publish_timer);	/* 关闭定时器 */
    os_timer_setfn(&mqtt_publish_timer,(os_timer_func_t *)mqtt_publish_timer_cb, NULL);	/* 设置sw timer callback */
    os_timer_arm(&mqtt_publish_timer, 5*1000, 1);  /* 使能定时器 */

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

static const partition_item_t at_partition_table[] =
{
    { SYSTEM_PARTITION_BOOTLOADER, 						0x0, 												0x1000},
    { SYSTEM_PARTITION_RF_CAL,  						SYSTEM_PARTITION_RF_CAL_ADDR, 						0x1000},
    { SYSTEM_PARTITION_PHY_DATA, 						SYSTEM_PARTITION_PHY_DATA_ADDR, 					0x1000},
    { SYSTEM_PARTITION_SYSTEM_PARAMETER, 				SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR, 			0x3000},
};

void ICACHE_FLASH_ATTR user_pre_init(void)
{
    if(!system_partition_table_regist(at_partition_table, sizeof(at_partition_table)/sizeof(at_partition_table[0]),SPI_FLASH_SIZE_MAP))
    {
        os_printf("system_partition_table_regist fail\r\n");
        while(1);
    }
}


void ICACHE_FLASH_ATTR smartconfig_done(sc_status status, void *pdata)
{
    switch(status)
    {
    case SC_STATUS_WAIT:
        HW_DEBUG("SC_STATUS_WAIT\n");
        break;
    case SC_STATUS_FIND_CHANNEL:
        HW_DEBUG("SC_STATUS_FIND_CHANNEL\n");
        break;
    case SC_STATUS_GETTING_SSID_PSWD:
        HW_DEBUG("SC_STATUS_GETTING_SSID_PSWD\n");
        sc_type *type = pdata;
        if (*type == SC_TYPE_ESPTOUCH)
        {
            HW_DEBUG("SC_TYPE:SC_TYPE_ESPTOUCH\n");
        }
        else
        {
            HW_DEBUG("SC_TYPE:SC_TYPE_AIRKISS\n");
        }
        break;
    case SC_STATUS_LINK:
        HW_DEBUG("SC_STATUS_LINK\n");
        struct station_config *sta_conf = pdata;

        wifi_station_set_config(sta_conf);
        wifi_station_disconnect();
        wifi_station_connect();
        break;
    case SC_STATUS_LINK_OVER:
        HW_DEBUG("SC_STATUS_LINK_OVER\n");
        if (pdata != NULL)
        {
            //SC_TYPE_ESPTOUCH
            uint8 phone_ip[4] = {0};

            os_memcpy(phone_ip, (uint8*)pdata, 4);
            HW_DEBUG("Phone ip: %d.%d.%d.%d\n",phone_ip[0],phone_ip[1],phone_ip[2],phone_ip[3]);
        }
        else
        {
            //SC_TYPE_AIRKISS - support airkiss v2.0
        }
        smartconfig_stop();
        break;
    }

}


LOCAL void enter_smart_config()
{
    HW_DEBUG("enter_smart_config\n");
    smartconfig_set_type(SC_TYPE_ESPTOUCH_AIRKISS); //SC_TYPE_ESPTOUCH,SC_TYPE_AIRKISS,SC_TYPE_ESPTOUCH_AIRKISS
    smartconfig_start(smartconfig_done);

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

    hw_dht11_init();
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
    wifi_station_get_config(&sta_config);
    if(os_strlen(sta_config.ssid) == 0)
    {
        HW_DEBUG("FLASH no wifi info,so direct enter smart config");
        enter_smart_config();
    }
    else
        wifi_station_connect();

    INFO("\r\nSystem started ...\r\n");
}
