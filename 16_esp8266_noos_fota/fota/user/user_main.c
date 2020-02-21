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
#include "driver/hw_wrapper.h"
#include "espconn.h"
#include "ip_addr.h"
#include "mem.h"
#include "upgrade.h"

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
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x81000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x3fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x3fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x3fd000
#define SYSTEM_PARTITION_CUSTOMER_PRIV_PARAM_ADDR           0x7c000
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

#define SYSTEM_PARTITION_CUSTOMER_PRIV_PARAM                SYSTEM_PARTITION_CUSTOMER_BEGIN
static const partition_item_t at_partition_table[] = {
		{ SYSTEM_PARTITION_BOOTLOADER, 						0x0, 												0x1000},
		{ SYSTEM_PARTITION_OTA_1,   						0x1000, 											SYSTEM_PARTITION_OTA_SIZE},
		{ SYSTEM_PARTITION_OTA_2,   						SYSTEM_PARTITION_OTA_2_ADDR, 						SYSTEM_PARTITION_OTA_SIZE},
		{ SYSTEM_PARTITION_RF_CAL,  						SYSTEM_PARTITION_RF_CAL_ADDR, 						0x1000},
		{ SYSTEM_PARTITION_PHY_DATA, 						SYSTEM_PARTITION_PHY_DATA_ADDR, 					0x1000},
		{ SYSTEM_PARTITION_SYSTEM_PARAMETER, 				SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR, 			0x3000},
		{ SYSTEM_PARTITION_CUSTOMER_PRIV_PARAM,             SYSTEM_PARTITION_CUSTOMER_PRIV_PARAM_ADDR,          0x1000},
};

void ICACHE_FLASH_ATTR user_pre_init(void)
{
	os_printf("SPI_FLASH_SIZE_MAP %d\n",SPI_FLASH_SIZE_MAP);
    if(!system_partition_table_regist(at_partition_table, sizeof(at_partition_table)/sizeof(at_partition_table[0]),SPI_FLASH_SIZE_MAP)) {
		os_printf("system_partition_table_regist fail\r\n");
		while(1);
	}
}

struct espconn http_tcp_conn;
uint8_t get_bin_buffer[256] = {0};

#define USER1_BIN_NAME "user1.4096.new.4.bin"
#define USER2_BIN_NAME "user2.4096.new.4.bin"
#define DNS_SERVER "wireless-ota.bj.bcebos.com"
#define GET_BIN_STRING "GET /%s HTTP/1.0\r\nHost: wireless-ota.bj.bcebos.com\r\n\r\n"

LOCAL void ICACHE_FLASH_ATTR
fota_upgrade_rsp(void *arg)
{
    struct upgrade_server_info *server = arg;
    struct espconn *pespconn = server->pespconn;

    if (server->upgrade_flag == true) {
        HW_DEBUG("[IAMLIUBO] fota upgarde successfully\n");
        system_upgrade_reboot();
    } else {
    	HW_DEBUG("[IAMLIUBO] fota upgrade failed\n");
    }

    os_free(server->url);
    server->url = NULL;
    os_free(server);
    server = NULL;
}

LOCAL void ICACHE_FLASH_ATTR
fota_upgrade_begin(struct espconn *pespconn, struct upgrade_server_info *server)
{
    uint8 user_bin[30] = {0};

    server->pespconn = pespconn;

    os_memcpy(server->ip, pespconn->proto.tcp->remote_ip, 4);

    server->port = 80;


    server->check_cb = fota_upgrade_rsp;
    server->check_times = 120000;

    if (server->url == NULL) {
        server->url = (uint8 *)os_zalloc(128);
    }

    if (system_upgrade_userbin_check() == UPGRADE_FW_BIN1) {
        os_memcpy(user_bin, USER2_BIN_NAME, os_strlen(USER2_BIN_NAME));
    } else if (system_upgrade_userbin_check() == UPGRADE_FW_BIN2) {
        os_memcpy(user_bin, USER1_BIN_NAME, os_strlen(USER1_BIN_NAME));
    }

    os_sprintf(server->url, GET_BIN_STRING,user_bin);

    HW_DEBUG("[IAMLIUBO] %s\n",server->url);


    if (system_upgrade_start(server) == false) {
    	HW_DEBUG("[IAMLIUBO] upgrade is already started\n");
    }
}


void ICACHE_FLASH_ATTR http_tcp_sent_cb(void *arg)  //发送
{
    HW_DEBUG("http_tcp_sent_cb\n");
}
void ICACHE_FLASH_ATTR http_tcp_discon_cb(void *arg)  //断开
{
    HW_DEBUG("http_tcp_discon_cb\n");
}
void ICACHE_FLASH_ATTR http_tcp_recv_cb(void *arg,  //接收
                                        char *pdata, unsigned short len)
{
    HW_DEBUG("\n----------- http recv data len %d-----------\n",len);
    HW_DEBUG("%s\n", pdata);
    HW_DEBUG("----------- http recv data end----------\n");
}
void ICACHE_FLASH_ATTR http_tcp_recon_cb(void *arg, sint8 err) //注册 TCP 连接发生异常断开时的回调函数，可以在回调函数中进行重连
{
    HW_DEBUG("connect err,reason %d\r\n", err);
    espconn_connect((struct espconn *) arg);
}
void ICACHE_FLASH_ATTR http_tcp_connect_cb(void *arg)  //注册 TCP 连接成功建立后的回调函数
{
    struct espconn *pespconn = arg;
    struct upgrade_server_info *server = NULL;
    HW_DEBUG("连接成功\n");
    espconn_regist_recvcb(pespconn, http_tcp_recv_cb);  //接收
    espconn_regist_sentcb(pespconn, http_tcp_sent_cb);  //发送
    espconn_regist_disconcb(pespconn, http_tcp_discon_cb);  //断开

    server = (struct upgrade_server_info *)os_zalloc(sizeof(struct upgrade_server_info));

	server->upgrade_version[15] = '\0';

	HW_DEBUG("[IMALIUBO] fota upgarde start\r\n");
	fota_upgrade_begin(pespconn, server);
}

void ICACHE_FLASH_ATTR http_dns_found(const char *name, ip_addr_t *ipaddr,void *arg)
{
    if(ipaddr == NULL)
    {
        HW_DEBUG("DNS GET IP FAIL\n");
        return;
    }
    else if (ipaddr != NULL && ipaddr->addr != 0)
    {
        struct ip_info info;
        HW_DEBUG("DNS GET IP SUCCESS\n");
        wifi_get_ip_info(STATION_IF, &info);
        http_tcp_conn.proto.tcp = (esp_tcp *) os_zalloc(sizeof(esp_tcp));  //分配空间
        http_tcp_conn.type = ESPCONN_TCP;  //设置类型为TCP协议
        http_tcp_conn.proto.tcp->local_port = espconn_port();  //本地端口
        http_tcp_conn.proto.tcp->remote_port = 80;  //目标端口
        os_memcpy(http_tcp_conn.proto.tcp->local_ip, &info.ip, 4);
        os_memcpy(http_tcp_conn.proto.tcp->remote_ip, ipaddr, 4);
        //注册连接成功回调函数和重新连接回调函数
        espconn_regist_connectcb(&http_tcp_conn, http_tcp_connect_cb);//注册 TCP 连接成功建立后的回调函数
        espconn_regist_reconcb(&http_tcp_conn, http_tcp_recon_cb);//注册 TCP 连接发生异常断开时的回调函数，可以在回调函数中进行重连

        espconn_connect(&http_tcp_conn);
    }
}

LOCAL void tcp_connect_init()
{
    ip_addr_t http_server_ip;
    espconn_gethostbyname(&http_tcp_conn, DNS_SERVER, &http_server_ip, http_dns_found);
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

void button_handle_cb(void *para)
{
	static uint8_t led_on = 0;
	uint32	gpio_status;

	gpio_status	=	GPIO_REG_READ(GPIO_STATUS_ADDRESS);
	//clear	interrupt	status
	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS,gpio_status);

	if(gpio_status & (BIT(13)))
	{
		os_printf("button pressed\n");
		if(GPIO_INPUT_GET(13))
		{
			os_printf("high\n");
		}
		else
		{
			os_printf("button press,upgrade start....\n");
			tcp_connect_init();
			GPIO_OUTPUT_SET(GPIO_ID_PIN(4),led_on);
		}

	}

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
	HW_DEBUG("This is wifi fota user bin2\n");
	HW_DEBUG("---------------------------\n");

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U,	FUNC_GPIO13);/* BUTTON:GPIO13 */

	ETS_GPIO_INTR_DISABLE();/* 关闭中断 */
	ETS_GPIO_INTR_ATTACH((ets_isr_t)button_handle_cb, NULL);/* 注册中断处理函数 */
	gpio_pin_intr_state_set(GPIO_ID_PIN(13), GPIO_PIN_INTR_NEGEDGE);/* 下升沿采集 */
	ETS_GPIO_INTR_ENABLE(); /* 使能中断 */

	if (system_upgrade_userbin_check() == UPGRADE_FW_BIN1)
	{
		HW_DEBUG("\r\n[IMALIUBO] user1 \r\n");
	} else if (system_upgrade_userbin_check() == UPGRADE_FW_BIN2) {
		HW_DEBUG("\r\n[IMALIUBO] user2 \r\n");
	}


	wifi_set_opmode(STATION_MODE);
	os_memset(&sta_config, 0, sizeof(struct station_config));
	os_strcpy(sta_config.ssid,ESP8266_STA_SSID);
	os_strcpy(sta_config.password,ESP8266_STA_PASS);

	wifi_station_set_config(&sta_config);
	wifi_set_event_handler_cb(wifi_handle_event_cb);


}

