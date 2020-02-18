

#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"
#include "espnow.h"


#define CONTROLLER
//#define SLAVE


uint8_t controller_mac[6] = {0xec, 0xfa, 0xbc, 0xc2, 0x26, 0xbc}; /* controller从station发出 */
uint8_t slave_mac[6] = {0xA6, 0xcf, 0x12, 0xd9, 0xd4, 0x89};/* slave从softap发出 */


#define ESP_NOW_SEND_INTERVAL   3000
static os_timer_t g_esp_now_timer;

/*
 * 函数：user_esp_now_recv_cb
 * 说明：ESP-NOW接收回调函数
 */
static void ICACHE_FLASH_ATTR
user_esp_now_recv_cb(uint8_t *macaddr, uint8_t *data, uint8_t len)
{
    int i;
    static u16 ack_count=0;
    uint8_t ack_buf[16];
    uint8_t recv_buf[17];
    os_printf("now from[");

    for(i = 0; i < 6; i++){
        os_printf("%02X ", macaddr[i]);
    }
    os_printf(" len: %d]:", len);

    os_bzero(recv_buf, 17);
    os_memcpy(recv_buf, data, len<17?len:16);
    os_printf("%s",recv_buf);
    os_printf("\r\n");

    if (os_strncmp(data, "ACK", 3) == 0){
        return;
    }else{

    }

    os_sprintf(ack_buf, "ACK[%08x]", ack_count++);
    esp_now_send(macaddr, ack_buf, os_strlen(ack_buf));
}

/*
 * 函数：user_esp_now_send_cb
 * 说明：ESP-NOW发送回调函数
 */
void ICACHE_FLASH_ATTR
user_esp_now_send_cb(uint8_t *mac_addr, uint8_t status)
{
    int i;
    for(i = 0; i < 6; i++){
        os_printf("%02X ", mac_addr[i]);
    }

    if(1==status){
        os_printf("SEND FAIL!\r\n");
    }else if(0==status){
        os_printf("SEND SUCCESSFUL!\r\n");
    }
}

/*
 * 函数：user_esp_now_send
 * 说明：ESP-NOW数据发送
 */
void ICACHE_FLASH_ATTR
user_esp_now_send(uint8_t *mac_addr, uint8_t *data, uint8_t len)
{
    /* the demo will send to two devices which added by esp_now_add_peer() */
    //u8 result = esp_now_send(NULL, data, len);

    /* send to the specified mac_addr */
	uint8_t result = esp_now_send(mac_addr, data, len);
}

/*
 * 函数：user_esp_now_timer_cb
 * 说明：定时器回调函数
 */
static void ICACHE_FLASH_ATTR
user_esp_now_timer_cb(void* arg)
{
	uint8_t *mac = arg;
	uint8_t* send_data = "Hello World!";
    int result = esp_now_is_peer_exist(mac);
    //os_printf("peer_exist = %d\r\n", result);
    user_esp_now_send(mac, send_data, os_strlen(send_data));
}

/*
 * 函数：user_esp_now_timer_init
 * 说明：定时发送 ESP_NOW 数据包
 */
void ICACHE_FLASH_ATTR
user_esp_now_timer_init(uint8_t *mac)
{
    os_timer_disarm(&g_esp_now_timer);
    os_timer_setfn(&g_esp_now_timer, (os_timer_func_t *)user_esp_now_timer_cb, mac);
    os_timer_arm(&g_esp_now_timer, ESP_NOW_SEND_INTERVAL, 1);
}

/*
 * 函数：user_esp_now_init
 * 说明：ESP-NOW初始化
 */
void ICACHE_FLASH_ATTR
user_esp_now_init(void)
{
    int result;

    if (esp_now_init()==0) {
        os_printf("esp_now init ok\n");

        // 注册 ESP-NOW 收包的回调函数
        esp_now_register_recv_cb(user_esp_now_recv_cb);
        // 注册发包回调函数
        esp_now_register_send_cb(user_esp_now_send_cb);

        /* role
         * ESP_NOW_ROLE_IDLE - 空闲
         * ESP_NOW_ROLE_CONTROLLER - 主机
         * ESP_NOW_ROLE_SLAVE - 从机
         * ESP_NOW_ROLE_COMBO - 主/从机
         */
#if defined(SLAVE)
        wifi_set_opmode_current(SOFTAP_MODE);
        os_printf("==================\r\n");
        os_printf("SLAVE\r\n");
        os_printf("==================\r\n");
        esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
        //esp_now_add_peer(controller_mac, ESP_NOW_ROLE_CONTROLLER, 1, NULL, 16);


#elif defined(CONTROLLER)
        wifi_set_opmode_current(STATION_MODE);
        os_printf("==================\r\n");
        os_printf("CONTROLLER\r\n");
        os_printf("==================\r\n");
        esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
        //esp_now_add_peer(slave_mac, ESP_NOW_ROLE_SLAVE, 1, NULL, 16);


        // 不需要连接Wi-Fi
        wifi_station_disconnect();
        //wifi_station_connect();
        user_esp_now_timer_init(slave_mac);

#endif

    } else {
        os_printf("esp_now init failed\n");
    }
}

/************************************************************************/


