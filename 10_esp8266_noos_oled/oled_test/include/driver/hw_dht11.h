#ifndef __DHT11_H__
#define __DHT11_H__

#include "hw_misc.h"

#define DHT11_GPIO 10
#define DHT11_RESPONSE_US 80
#define DHT11_DATA_START_US 50

typedef struct
{
	uint8_t humidity_h;		/* ʪ���������� */
	uint8_t humidity_l;		/* ʪ��С������ */
	uint8_t temperature_h;	/* �¶��������� */
	uint8_t temperature_l;	/* �¶�С������ */
	uint8_t negative;		/* �¶��Ƿ�Ϊ���� 0:���� 1:����  */
}dht11_data_t;

uint8_t ICACHE_FLASH_ATTR hw_dht11_init(void);
uint8_t ICACHE_FLASH_ATTR hw_dht11_read(dht11_data_t *dht11_data);

#endif
