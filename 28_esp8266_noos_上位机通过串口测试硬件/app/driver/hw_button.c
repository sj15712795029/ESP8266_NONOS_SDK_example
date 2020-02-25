#include "driver/hw_button.h"

struct keys_param switch_param;
struct single_key_param *switch_signle;


void ICACHE_FLASH_ATTR hw_button_init(key_function short_press,key_function long_press)
{
	

	switch_signle = key_init_single( BUTTON_GPIO_ID, BUTTON_GPIO_NAME,
	                                     BUTTON_GPIO_FUNC,
										 long_press,
										 short_press);
    switch_param.key_num = 1;
    switch_param.single_key = &switch_signle;

    key_init( &switch_param );
}
