/******************************************************************************
  * @file    weather.h
  * @author  Wireless link team
  * @version V0.0.1
  * @date    2020-02-08
  * @brief   weather file
******************************************************************************/
#ifndef WEATHER_H_H_H
#define WEATHER_H_H_H

#include "driver/hw_wrapper.h"

/******** HTTP SOURCE CONFIG ********/
#define SENIVERSE_WEATHER
//#define BAIDU_WEATHER
/***********************************/

/* OLED���ֹ滮 */
#define WEATHER_MENU_FONT_PAGE 0
#define WEATHER_MENU_FONT1_4_COL 16
#define WEATHER_MENU_FONT2_4_COL 32
#define WEATHER_MENU_FONT3_4_COL 48
#define WEATHER_MENU_FONT4_4_COL 64

#define WEATHER_IMG_START_COL 4	/* ͼƬ��ʼ�� */
#define WEATHER_IMG_END_COL 44		/* ͼƬ������ */
#define WEATHER_IMG_START_PAGE 3	/* ͼƬ��ʼҳ */
#define WEATHER_IMG_END_PAGE 8		/* ͼƬ����ҳ */

#define WEATHER_FONT1_START_COL 64 	/* ��һ���� */
#define WEATHER_FONT2_START_COL 80 	/* �ڶ����� */
#define WEATHER_FONT3_START_COL 96 	/* �������� */
#define WEATHER_FONT4_START_COL 112 	/* ���ĸ��� */
#define WAATHER_FONT_PAGE 3 /* ���ֵ���ʾҳ */

#define HUM_FONT_COL 88
#define HUM_FONT_PADDING 4
#define HUM_FONT_PAGE 0

#define DAY_TIME 0		 /* ���� */
#define NIGHT_TIME 1	/* ���� */


#if defined BAIDU_WEATHER
/* �ٶ������� */
#define CITY_STRING "�ൺ"
#define CITY_ID 283
#define DNS_SERVER "gwgp-n6uzuwmjrou.n.bdcloudapi.com"
#define APP_CODE "4b1270235a6a4399a777dbbe3f961741"
#define GET_STRING_CITY_NAME "GET  /weather/query?city=%s HTTP/1.1\r\nHost: %s\r\nX-Bce-Signature: AppCode/%s\r\n\r\n"
#define GET_STRING_CITY_ID "GET  /weather/query?cityid=%d HTTP/1.1\r\nHost: %s\r\nX-Bce-Signature: AppCode/%s\r\n\r\n"
#elif defined SENIVERSE_WEATHER
/* ��֪���� */
#define CITY_STRING "qingdao"
#define DNS_SERVER "api.seniverse.com"
#define PRIVATE_KEY "S_H9Ck2At7W5iflT5"
#define GET_STRING_CITY_ID "GET /v3/weather/now.json?key=%s&location=%s&language=en HTTP/1.1\r\nContent-Type: text/html;charset=utf-8\r\nHost: %s\r\n\r\n"
#define STRING_SPLIT "{\"results\":"
typedef enum
{
    weather_sunny = 0, 				/* ��������*/
    weather_sunny_1 = 1, 			/* ��������*/
    weather_cloud = 4,				/* ���� */
    weather_partly_cloudy = 5,	/* ������ */
    weather_partly_cloudy_1 = 6,	/* ���������� */
    weather_mostly_cloudy = 7,	/* �󲿶��� */
    weather_overcast = 9,			/* �� */
    weather_shower = 10,			/* ���� */
    weather_thundershower = 11,/* ������ */
    weather_thundershower_with_hail = 12,/* ��������� */
    weather_light_rain = 13,			/* С�� */
    weather_moderate_rain = 14,	/* ���� */
    weather_heavy_rain = 15,			/* ���� */
    weather_storm = 16,				/* ���� */
    weather_heavy_storm = 17,		/* ���� */
    weather_severe_storm = 18,		/* �ش��� */
    weather_ice_rain = 19,				/* ���� */
    weather_sleet = 20,					/* ���ѩ */
    weather_snow_flurry = 21,		/* ��ѩ */
    weather_light_snow = 22,			/* Сѩ */
    weather_moderate_snow = 23,	/* ��ѩ */
    weather_heavy_snow = 24,		/* ��ѩ */
    weather_snowstorm = 25,			/* ��ѩ */
    weather_dust = 26,					/* ���� */
    weather_sand = 27,					/* ��ɳ */
    weather_duststorm = 28,			/* ɳ���� */
    weather_sandstorm = 29,			/* ǿɳ���� */
    weather_foggy = 30,				/* �� */
    weather_haze = 31,					/* �� */
    weather_windy = 32,				/* �� */
    weather_blustery = 33,				/* ��� */
    weather_hurricane = 34,			/* 쫷� */
    weather_tropical_storm = 35,	/* �ȴ��籩 */
    weather_tornado = 36,				/* ����� */
    weather_cold = 37,					/* �� */
    weather_hot = 38,					/* �� */
    weather_unknown = 39,			/* δ֪ */
} weather_code_e;
#endif

void ICACHE_FLASH_ATTR weather_parse(uint8_t *data,uint16_t len);
void ICACHE_FLASH_ATTR weather_ui_init(void);
void ICACHE_FLASH_ATTR weather_first_time_stamp_update(uint8_t *time_stamp);
void ICACHE_FLASH_ATTR weather_time_stamp_update(uint8_t *time_stamp);

#endif