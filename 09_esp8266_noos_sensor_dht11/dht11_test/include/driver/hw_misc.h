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

#if DEBUG_ENABLE > 0
#define HW_DEBUG os_printf
#else
#define HW_DEBUG
#endif

typedef enum
{
	HW_ERR_OK = 0,

	/* DHT11 ERR */
	HW_ERR_DHT11_NO_RESPONSE,
	HW_ERR_DHT11_NO_DATA_START,
	HW_ERR_DHT11_DATA_ERR,


}hw_err_e;

void ICACHE_FLASH_ATTR hw_hex_dump(uint8_t *data,uint32_t len);
void ICACHE_FLASH_ATTR hw_delay_ms(uint32_t cms);

#endif
