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

/* OLED布局规划 */
#define WEATHER_MENU_FONT_PAGE 0
#define WEATHER_MENU_FONT1_4_COL 16
#define WEATHER_MENU_FONT2_4_COL 32
#define WEATHER_MENU_FONT3_4_COL 48
#define WEATHER_MENU_FONT4_4_COL 64

#define WEATHER_IMG_START_COL 4	/* 图片开始列 */
#define WEATHER_IMG_END_COL 44		/* 图片结束列 */
#define WEATHER_IMG_START_PAGE 3	/* 图片开始页 */
#define WEATHER_IMG_END_PAGE 8		/* 图片结束页 */

#define WEATHER_FONT1_START_COL 64 	/* 第一个字 */
#define WEATHER_FONT2_START_COL 80 	/* 第二个字 */
#define WEATHER_FONT3_START_COL 96 	/* 第三个字 */
#define WEATHER_FONT4_START_COL 112 	/* 第四个字 */
#define WAATHER_FONT_PAGE 3 /* 文字的显示页 */

#define HUM_FONT_COL 88
#define HUM_FONT_PADDING 4
#define HUM_FONT_PAGE 0

#define DAY_TIME 0		 /* 白天 */
#define NIGHT_TIME 1	/* 晚上 */


#if defined BAIDU_WEATHER
/* 百度云天气 */
#define CITY_STRING "青岛"
#define CITY_ID 283
#define DNS_SERVER "gwgp-n6uzuwmjrou.n.bdcloudapi.com"
#define APP_CODE "4b1270235a6a4399a777dbbe3f961741"
#define GET_STRING_CITY_NAME "GET  /weather/query?city=%s HTTP/1.1\r\nHost: %s\r\nX-Bce-Signature: AppCode/%s\r\n\r\n"
#define GET_STRING_CITY_ID "GET  /weather/query?cityid=%d HTTP/1.1\r\nHost: %s\r\nX-Bce-Signature: AppCode/%s\r\n\r\n"
#elif defined SENIVERSE_WEATHER
/* 心知天气 */
#define CITY_STRING "qingdao"
#define DNS_SERVER "api.seniverse.com"
#define PRIVATE_KEY "S_H9Ck2At7W5iflT5"
#define GET_STRING_CITY_ID "GET /v3/weather/now.json?key=%s&location=%s&language=en HTTP/1.1\r\nContent-Type: text/html;charset=utf-8\r\nHost: %s\r\n\r\n"
#define STRING_SPLIT "{\"results\":"
typedef enum
{
    weather_sunny = 0, 				/* 白天晴天*/
    weather_sunny_1 = 1, 			/* 晚上晴天*/
    weather_cloud = 4,				/* 多云 */
    weather_partly_cloudy = 5,	/* 晴间多云 */
    weather_partly_cloudy_1 = 6,	/* 晚上晴间多云 */
    weather_mostly_cloudy = 7,	/* 大部多云 */
    weather_overcast = 9,			/* 阴 */
    weather_shower = 10,			/* 阵雨 */
    weather_thundershower = 11,/* 雷阵雨 */
    weather_thundershower_with_hail = 12,/* 雷阵雨冰雹 */
    weather_light_rain = 13,			/* 小雨 */
    weather_moderate_rain = 14,	/* 中雨 */
    weather_heavy_rain = 15,			/* 大雨 */
    weather_storm = 16,				/* 暴雨 */
    weather_heavy_storm = 17,		/* 大暴雨 */
    weather_severe_storm = 18,		/* 特大暴雨 */
    weather_ice_rain = 19,				/* 冰雨 */
    weather_sleet = 20,					/* 雨夹雪 */
    weather_snow_flurry = 21,		/* 阵雪 */
    weather_light_snow = 22,			/* 小雪 */
    weather_moderate_snow = 23,	/* 中雪 */
    weather_heavy_snow = 24,		/* 大雪 */
    weather_snowstorm = 25,			/* 暴雪 */
    weather_dust = 26,					/* 浮沉 */
    weather_sand = 27,					/* 扬沙 */
    weather_duststorm = 28,			/* 沙尘暴 */
    weather_sandstorm = 29,			/* 强沙尘暴 */
    weather_foggy = 30,				/* 雾 */
    weather_haze = 31,					/* 霾 */
    weather_windy = 32,				/* 风 */
    weather_blustery = 33,				/* 大风 */
    weather_hurricane = 34,			/* 飓风 */
    weather_tropical_storm = 35,	/* 热带风暴 */
    weather_tornado = 36,				/* 龙卷风 */
    weather_cold = 37,					/* 冷 */
    weather_hot = 38,					/* 热 */
    weather_unknown = 39,			/* 未知 */
} weather_code_e;
#endif

void ICACHE_FLASH_ATTR weather_parse(uint8_t *data,uint16_t len);
void ICACHE_FLASH_ATTR weather_ui_init(void);
void ICACHE_FLASH_ATTR weather_first_time_stamp_update(uint8_t *time_stamp);
void ICACHE_FLASH_ATTR weather_time_stamp_update(uint8_t *time_stamp);

#endif