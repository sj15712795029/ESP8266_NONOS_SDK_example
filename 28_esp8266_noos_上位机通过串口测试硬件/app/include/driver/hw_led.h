/******************************************************************************
  * @file    hw_led.h
  * @author  Yu-ZhongJun
  * @version V0.0.1
  * @date    2020-02-06
  * @brief   hw led header file
******************************************************************************/
#ifndef __HW_LED_H__
#define __HW_LED_H__

#include "driver/hw_misc.h"

/* -------------------- LED GPIO PIN CONFIG -------------------------*/
#define LED_GPIO_ID 4
#define LED_GPIO_NAME PERIPHS_IO_MUX_GPIO4_U
#define LED_GPIO_FUNC FUNC_GPIO4
/* -------------------- LED GPIO PIN CONFIG END ---------------------*/

#define LED_ON GPIO_OUTPUT_SET(GPIO_ID_PIN(LED_GPIO_ID),1)
#define LED_OFF GPIO_OUTPUT_SET(GPIO_ID_PIN(LED_GPIO_ID),0)

void ICACHE_FLASH_ATTR hw_led_init(void);

#endif