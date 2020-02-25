/******************************************************************************
  * @file    hw_button.h
  * @author  Yu-ZhongJun
  * @version V0.0.1
  * @date    2020-02-06
  * @brief   hw button header file
******************************************************************************/
#ifndef __HW_BUTTON_H__
#define __HW_BUTTON_H__

#include "driver/hw_misc.h"

/* -------------------- BUTTON GPIO PIN CONFIG -------------------------*/
#define BUTTON_GPIO_ID 13
#define BUTTON_GPIO_NAME PERIPHS_IO_MUX_MTCK_U
#define BUTTON_GPIO_FUNC FUNC_GPIO13
/* -------------------- BUTTON GPIO PIN CONFIG END ---------------------*/

void ICACHE_FLASH_ATTR hw_button_init(key_function short_press,key_function long_press);

#endif