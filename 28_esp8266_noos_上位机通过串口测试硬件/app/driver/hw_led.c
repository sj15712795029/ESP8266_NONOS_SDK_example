#include "driver/hw_led.h"

void ICACHE_FLASH_ATTR hw_led_init(void)
{
	PIN_FUNC_SELECT(LED_GPIO_NAME,	LED_GPIO_FUNC);
}