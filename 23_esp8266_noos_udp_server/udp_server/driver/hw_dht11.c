#include "driver/hw_dht11.h"


static ICACHE_FLASH_ATTR uint8_t hw_dht11_read_byte();


uint8_t ICACHE_FLASH_ATTR hw_dht11_init()
{
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_SD_DATA3_U, FUNC_GPIO10);

	return HW_ERR_OK;
}


uint8_t ICACHE_FLASH_ATTR hw_dht11_read(dht11_data_t *dht11_data)
{
	uint8_t try_count = 0;
	uint8_t check_sum = 0;
	/* 1.拉高GPIO */
	GPIO_OUTPUT_SET(GPIO_ID_PIN(DHT11_GPIO),1);
	hw_delay_ms(10);

	/* 2.拉低，发送开始信号 */
	GPIO_OUTPUT_SET(GPIO_ID_PIN(DHT11_GPIO),0);
	hw_delay_ms(20);

	/* 3.拉高，设置为输入，等待dht11相应低电平 */
	GPIO_OUTPUT_SET(GPIO_ID_PIN(DHT11_GPIO),1);
	os_delay_us(5);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_SD_DATA3_U,	FUNC_GPIO10);
	GPIO_DIS_OUTPUT(DHT11_GPIO);
	while( GPIO_INPUT_GET(GPIO_ID_PIN(DHT11_GPIO)) == 1 \
			&& try_count < DHT11_RESPONSE_US)
	{
		os_delay_us(1);
		try_count++;
	}

	if(try_count >= DHT11_RESPONSE_US)
		return HW_ERR_DHT11_NO_RESPONSE;

	try_count = 0;

	/* 4.判断相应高电平 */
	while( GPIO_INPUT_GET(GPIO_ID_PIN(DHT11_GPIO)) == 0 \
				&& try_count < DHT11_RESPONSE_US)
	{
		os_delay_us(1);
		try_count++;
	}

	if(try_count >= DHT11_RESPONSE_US)
			return HW_ERR_DHT11_NO_RESPONSE;

	try_count = 0;
	/* 5. 读取数据  */
	dht11_data->humidity_h = hw_dht11_read_byte();
	dht11_data->humidity_l = hw_dht11_read_byte();
	dht11_data->temperature_h = hw_dht11_read_byte();
	dht11_data->temperature_l = hw_dht11_read_byte();
	check_sum = hw_dht11_read_byte();
	if((dht11_data->temperature_l & 0x80) == 0x80)
		dht11_data->negative = 1;
	else
		dht11_data->negative = 0;

	dht11_data->temperature_l &= 0x7f;
	/* 6.校验数据 */
	if(check_sum != ((dht11_data->humidity_h + dht11_data->humidity_l \
			+ dht11_data->temperature_h + dht11_data->temperature_l) & 0xff))
	{
		HW_DEBUG("dht11 data error\n");
		return HW_ERR_DHT11_DATA_ERR;
	}

	return HW_ERR_OK;

}

static uint8_t ICACHE_FLASH_ATTR hw_dht11_read_byte()
{
	uint8_t index = 0;
	uint8_t try_count = 0;
	uint8_t read_data = 0;

	for(index = 0; index < 8; index++)
	{

		/* 1.等到数据开始信号 */
		while( GPIO_INPUT_GET(GPIO_ID_PIN(DHT11_GPIO)) == 1 \
					&& try_count < DHT11_DATA_START_US)
		{
			os_delay_us(1);
			try_count++;
		}
		if(try_count >= DHT11_DATA_START_US)
				return HW_ERR_DHT11_NO_DATA_START;

		try_count = 0;
		/* 2.等到数据开始 */
		while( GPIO_INPUT_GET(GPIO_ID_PIN(DHT11_GPIO)) == 0 \
					&& try_count < DHT11_DATA_START_US)
		{
			os_delay_us(1);
			try_count++;
		}
		if(try_count >= DHT11_DATA_START_US)
				return HW_ERR_DHT11_NO_DATA_START;


		/* 数据位的低电平结束后，是数据位的高电平 */
		/*  数据"0"的高电平时长 == [23～27us] */
		/*  数据"1"的高电平时长 == [68～74us] */
		os_delay_us(40);	/* 跳过数据"0"的高电平部分 */

		read_data <<= 1;
		read_data |= GPIO_INPUT_GET(GPIO_ID_PIN(DHT11_GPIO));
	}

	return read_data;

}
