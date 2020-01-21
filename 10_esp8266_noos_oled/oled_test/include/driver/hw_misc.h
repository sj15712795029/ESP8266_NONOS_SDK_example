/******************************************************************************
  * @file    hw_misc.h
  * @author  Yu-ZhongJun
  * @version V0.0.1
  * @date    2020-01-20
  * @brief   hw misc header file
******************************************************************************/
#ifndef __HW_MISC_H__
#define __HW_MISC_H__

#include "user_interface.h"
#include "osapi.h"
#include "c_types.h"

#include "driver/hw_conf.h"
#include "driver/i2c_master.h"

#define HW_DEBUG(format,...) {if (DEBUG_ENABLE > 0) os_printf(format,##__VA_ARGS__);}

typedef enum
{
	HW_ERR_OK = 0,

	/* DHT11 ERR */
	HW_ERR_DHT11_NO_RESPONSE,
	HW_ERR_DHT11_NO_DATA_START,
	HW_ERR_DHT11_DATA_ERR,

	/* OLED err */
	HW_ERR_OLED_INVALID_DIRECT,
	HW_ERR_OLED_INVALID_INTERVAL,
	HW_ERR_OLED_INVALID_OFFSET,
	HW_ERR_OLED_INVALID_PAGE,
	HW_ERR_OLED_INVALID_COL,

}hw_err_e;

void ICACHE_FLASH_ATTR hw_hex_dump(uint8_t *data,uint32_t len);
void ICACHE_FLASH_ATTR hw_delay_ms(uint32_t cms);

#endif
