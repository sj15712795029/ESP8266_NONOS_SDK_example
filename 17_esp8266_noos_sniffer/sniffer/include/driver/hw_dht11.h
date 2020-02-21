#ifndef __DHT11_H__
#define __DHT11_H__

#include "hw_misc.h"

#define DHT11_GPIO 10
#define DHT11_RESPONSE_US 80
#define DHT11_DATA_START_US 50

typedef struct
{
	uint8_t humidity_h;		/* 湿度整数部分 */
	uint8_t humidity_l;		/* 湿度小数部分 */
	uint8_t temperature_h;	/* 温度整数部分 */
	uint8_t temperature_l;	/* 温度小数部分 */
	uint8_t negative;		/* 温度是否为负数 0:正数 1:负数  */
}dht11_data_t;

uint8_t ICACHE_FLASH_ATTR hw_dht11_init(void);
uint8_t ICACHE_FLASH_ATTR hw_dht11_read(dht11_data_t *dht11_data);

#endif
