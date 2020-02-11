#include "driver/hw_misc.h"


#define MAX_COL 16
#define SHOW_LINE_SIZE 16
void ICACHE_FLASH_ATTR hw_hex_dump(uint8_t *data,uint32_t len)
{
    int line;
    int curline = 0;
    int curcol = 0;
    char showline[SHOW_LINE_SIZE];
    int data_pos = 0;

    if(len % MAX_COL)
    {
        line = len/MAX_COL+1;
    }
    else
    {
        line = len/MAX_COL;
    }

    for(curline = 0; curline < line; curline++)
    {
        os_sprintf(showline,"%08xh:",curline*MAX_COL);
        HW_DEBUG("%s",showline);
        for(curcol = 0; curcol < MAX_COL; curcol++)
        {
            if(data_pos < len)
            {
                HW_DEBUG("%02x ",data[data_pos]);
                data_pos++;
                continue;
            }
            else
            {
                break;
            }
        }
        HW_DEBUG("\n");
    }
}

void ICACHE_FLASH_ATTR hw_delay_ms(uint32_t cms)
{
	for(;cms > 0; cms--)
	{
		if(cms % 1000 == 0)
			system_soft_wdt_feed(); /* 此部分为1s喂狗一次 */
	  os_delay_us(1000);
	}
}

