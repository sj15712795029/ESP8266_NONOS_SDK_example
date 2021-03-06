#include "weather/weather.h"
#include "json/cJSON.h"

uint8_t day_and_night = DAY_TIME;
uint8_t last_weather_code = 0;

const uint8_t weather_sunny_0_bmp[] =  /* �������� */
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x60,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x1E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x10,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x08,0x08,0x00,
    0x00,0xC0,0x30,0x18,0x0C,0x04,0x02,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x02,
    0x02,0x04,0x0C,0x18,0x30,0xC0,0x00,0x00,0x08,0x08,0x04,0x00,0x00,0x00,0x00,0x00,
    0x00,0x08,0x08,0x08,0x08,0x00,0x00,0x00,0x7F,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x7F,0x00,
    0x00,0x00,0x08,0x08,0x08,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x08,0x08,0x00,
    0x00,0x01,0x06,0x0C,0x18,0x10,0x20,0x20,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x20,
    0x20,0x10,0x18,0x0C,0x06,0x01,0x00,0x00,0x08,0x08,0x10,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x03,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x04,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

const uint8_t weather_sunny_1_bmp[] =  /* �������� */
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x80,0x60,0x30,0x18,0x0C,0x04,0x02,0x02,0x02,0x81,0xC1,0x21,0x11,0x0A,0x06,0x06,
    0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x03,0x0C,0x18,0x10,0x20,0x20,0x40,0x40,0x40,0x43,0x44,0x48,0x50,0x70,0x20,
    0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

const uint8_t weather_partly_cloudy_0_bmp[] =  /* ���������� */
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x60,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x1E,0x00,0x00,0x00,0x60,0xB0,0x90,0x98,0x08,0x08,0x18,0x10,0xB0,
    0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x08,0x08,0x00,
    0x00,0xC0,0x30,0x18,0x0C,0x04,0x02,0x02,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x19,0x31,0xC1,0x01,0x00,0x08,0x08,0x04,0x00,0x00,0x00,0x00,0x00,
    0x00,0x08,0x08,0x08,0x08,0x80,0x80,0x80,0x47,0x20,0x20,0x20,0x20,0x40,0x80,0x80,
    0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x7F,0x00,
    0x00,0x00,0x08,0x08,0x08,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x09,0x10,0x10,
    0x10,0x10,0x20,0x10,0x10,0x18,0x08,0x08,0x09,0x47,0x40,0x40,0x40,0x40,0x40,0x20,
    0x20,0x10,0x18,0x0C,0x06,0x01,0x00,0x00,0x08,0x08,0x10,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x03,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x04,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

const uint8_t weather_partly_cloudy_1_bmp[] = /* ���������� */
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x40,0x40,0x40,0x20,0x20,
    0x20,0x60,0x40,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x38,0x0C,0x02,
    0x01,0x00,0x00,0x00,0x00,0xC0,0x30,0x0C,0x06,0x03,0x01,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x0F,0x38,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x07,0x38,0x40,
    0x80,0x00,0x00,0x78,0x4C,0x84,0x84,0x86,0x83,0x01,0x00,0x00,0x00,0x01,0x03,0x06,
    0x04,0x84,0x8C,0x78,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x41,
    0x43,0x22,0x44,0x4C,0x88,0x88,0x08,0x08,0x08,0x09,0x0A,0x0E,0x04,0x00,0x00,0x00,
    0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x07,0x09,0x08,0x08,0x08,0x10,0x10,0x10,0x10,0x10,0x00,0x09,0x0F,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};


const uint8_t weather_cloud_bmp[] =  /* ���� */
{
    0x00,0x00,0x00,0xE0,0x90,0x10,0x18,0x08,0x08,0x08,0x08,0x10,0x10,0x60,0x00,0x00,
    0x00,0x00,0x00,0x80,0x80,0x40,0x40,0x20,0x20,0x20,0x20,0x40,0x40,0x80,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,
    0x01,0x01,0x01,0x20,0xF8,0x84,0x04,0x02,0x02,0x02,0x02,0x01,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x02,0x02,0x04,0x0C,0xF8,0x00,0x00,0x00,
    0x00,0xE0,0x30,0x10,0x10,0x10,0x08,0x04,0x04,0x02,0x02,0x00,0x00,0x01,0x01,0x12,
    0x12,0x22,0xC2,0x04,0x04,0x04,0x08,0x08,0x08,0x08,0x04,0x04,0x04,0x04,0x04,0x04,
    0x02,0x02,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x02,0x02,0x04,0x04,0x04,
    0x08,0x08,0x08,0x04,0x04,0x04,0xC4,0x24,0x02,0x02,0x01,0x00,0x0C,0x06,0x02,0x02,
    0x01,0x01,0x01,0x02,0x02,0x04,0x08,0x08,0x10,0x10,0xE0,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
    0x02,0x06,0x04,0x04,0x04,0x08,0x08,0x08,0x08,0x08,0x08,0x0C,0x04,0x04,0x04,0x04,
    0x04,0x02,0x01,0x00,0x00,0x00,0x00,0x00
};


const uint8_t weather_overcast_bmp[] =  /* ���� */
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x80,0x80,0x80,0xC0,0x20,0x10,0x10,0x08,0x08,0x08,0x04,0x04,0x04,0x08,
    0x08,0x08,0x08,0x10,0x20,0x60,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0xF8,0x04,0x02,0x02,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
    0x01,0x01,0x01,0x02,0x06,0x0C,0xB8,0x40,0x00,0x00,0x00,0x01,0x03,0x06,0x04,0x0C,
    0x08,0x08,0x08,0x08,0x18,0x10,0x20,0x20,0x20,0x20,0x40,0x40,0x40,0x40,0x20,0x20,
    0x20,0x20,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x08,0x08,0x04,0x02,0x01,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};


const uint8_t weather_weather_shower_bmp[] =  /* ���� */
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x60,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x1E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x10,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x08,0x08,0x00,
    0x00,0xC0,0x30,0x18,0x0C,0x04,0x02,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x82,
    0x82,0x84,0x8C,0x88,0x80,0x80,0x00,0x00,0x08,0x08,0x04,0x00,0x00,0x00,0x00,0x00,
    0x00,0x08,0x08,0x08,0x08,0x00,0x00,0x00,0x7F,0x80,0x00,0x00,0x00,0x00,0xF0,0x98,
    0x08,0x04,0x04,0x04,0x04,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,
    0x04,0x04,0x04,0x08,0x18,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x08,0x08,0x00,
    0x00,0x01,0x06,0x0C,0x18,0x10,0x20,0x01,0x01,0x02,0x82,0xE2,0x06,0x04,0x04,0x08,
    0x08,0x88,0x68,0x08,0x04,0x04,0x04,0x04,0xC4,0x24,0x02,0x02,0x01,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x03,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x1C,0x00,0x00,0x00,0x01,0x00,0x00,0x1C,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

const uint8_t weather_thundershower_bmp[] =  /* ������ */
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0xC0,0x20,0x10,0x10,0x08,
    0x08,0x08,0x04,0x04,0x04,0x04,0x08,0x08,0x08,0x10,0x30,0x60,0x80,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x04,0x02,0x02,0x01,0x01,0x01,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x02,0x06,0x0C,0xB8,0x40,0x00,0x00,
    0x00,0x01,0x03,0x02,0x04,0x04,0x08,0x08,0x08,0x08,0x18,0x10,0x30,0x20,0x20,0x20,
    0x40,0x40,0x40,0x40,0x20,0x20,0x20,0x20,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,
    0x88,0x08,0x04,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x20,0x30,0x2C,0x26,0xA3,0xE0,0x20,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x10,
    0x1E,0x01,0x00,0x80,0x00,0x00,0x00,0x1C,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x18,0x04,0x03,0x01,0x00,0x00,0x00,
    0x00,0x00,0x0C,0x03,0x00,0x00,0x00,0x00,0x00,0x08,0x0F,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

const uint8_t weather_light_rain_bmp[] =  /* С�� */
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0xC0,0x20,0x10,0x10,0x08,
    0x08,0x08,0x04,0x04,0x04,0x04,0x08,0x08,0x08,0x10,0x20,0x60,0x80,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x04,0x02,0x02,0x01,0x01,0x01,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x02,0x06,0x0C,0xF8,0x00,0x00,0x00,
    0x00,0x01,0x03,0x06,0x04,0x0C,0x08,0x08,0x08,0x08,0x18,0x10,0x20,0x20,0x20,0x20,
    0x40,0x40,0x40,0x40,0x20,0x20,0x20,0x20,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,
    0x08,0x08,0x04,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x3C,0x02,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x30,
    0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

const uint8_t weather_moderate_rain_bmp[] =  /* ���� */
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0xC0,0x20,0x10,0x10,0x08,
    0x08,0x08,0x04,0x04,0x04,0x04,0x08,0x08,0x08,0x10,0x20,0x60,0x80,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x04,0x02,0x02,0x01,0x01,0x01,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x02,0x06,0x0C,0xB8,0x40,0x00,0x00,
    0x00,0x01,0x03,0x02,0x04,0x04,0x08,0x08,0x08,0x08,0x18,0x10,0x20,0x20,0x20,0x20,
    0x40,0x40,0x40,0x40,0x20,0x20,0x20,0x20,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,
    0x08,0x08,0x04,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x10,0x0E,0x01,0x00,0x80,0x00,0x00,0x00,0x00,0x38,0x06,0x00,0x00,0x80,0x00,
    0x00,0x00,0x10,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x03,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

const uint8_t weather_heavy_rain_bmp[] =  /* ����/����/�ش��� */
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0xC0,0x20,0x10,0x10,0x08,
    0x08,0x08,0x04,0x04,0x04,0x04,0x08,0x08,0x08,0x10,0x30,0x60,0x80,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x04,0x02,0x02,0x01,0x01,0x01,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x02,0x06,0x0C,0xB8,0x40,0x00,0x00,
    0x00,0x01,0x03,0x02,0x04,0x04,0x08,0x08,0x88,0x08,0x18,0x10,0x30,0x20,0x20,0x20,
    0x40,0x40,0x40,0x40,0x20,0x20,0x20,0x20,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,
    0x08,0x08,0x04,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,
    0x03,0x00,0x80,0x00,0x00,0x00,0x30,0x0E,0x00,0x00,0x80,0x00,0x00,0x00,0x1C,0x03,
    0x00,0x80,0x00,0x00,0x00,0x18,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x01,0x00,0x00,0x00,0x00,0x00,
    0x00,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

const uint8_t weather_sleet_bmp[] =  /* ����/���ѩ */
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x60,0x30,0x10,0x08,
    0x08,0x08,0x04,0x04,0x04,0x04,0x08,0x08,0x08,0x10,0x30,0x60,0xC0,0x80,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x08,0x04,0x06,0x02,0x02,0x01,
    0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x02,0x06,0x04,0x98,0xE0,0x00,0x00,
    0x00,0x01,0x02,0x04,0x08,0x08,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x30,
    0x20,0x20,0x20,0x20,0x20,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,
    0x08,0x08,0x04,0x06,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x1E,0x01,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x18,0x0F,0x00,0x00,0x00,0x80,0x00,
    0x00,0x04,0x04,0x3F,0x04,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x01,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x02,0x02,0x1F,0x02,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

const uint8_t weather_light_snow_bmp[] =  /* ��ѩ/Сѩ */
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x60,0x30,0x10,0x08,
    0x08,0x08,0x04,0x04,0x04,0x04,0x08,0x08,0x08,0x10,0x30,0x60,0xC0,0x80,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x08,0x04,0x04,0x02,0x02,0x01,
    0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x02,0x02,0x04,0x18,0xE0,0x00,0x00,
    0x00,0x01,0x02,0x04,0x08,0x08,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,
    0x20,0x20,0x20,0x20,0x20,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,
    0x08,0x08,0x04,0x06,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x04,0x04,0x3F,0x04,0x04,0x04,0xC0,0x00,0x04,0x04,0x3F,
    0x04,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x01,0x01,0x0F,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0
};

const uint8_t weather_moderate_snow_bmp[] =  /* ��ѩ */
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x60,0x30,0x10,0x08,
    0x08,0x08,0x04,0x04,0x04,0x04,0x08,0x08,0x08,0x10,0x30,0x60,0xC0,0x80,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x08,0x04,0x04,0x02,0x02,0x01,
    0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x02,0x02,0x04,0x98,0xE0,0x00,0x00,
    0x00,0x01,0x02,0x04,0x08,0x08,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,
    0x20,0x20,0x20,0x20,0x20,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,
    0x08,0x08,0x04,0x06,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x04,0x04,0x3F,0x04,0x04,0x04,0xC0,0x00,0x04,0x04,0x3F,0x04,0x04,0x04,0xC0,
    0x00,0x04,0x04,0x3F,0x04,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x0F,
    0x01,0x01,0x01,0x00,0x00,0x01,0x01,0x0F,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

const uint8_t weather_heavy_snow_bmp[] =  /* ��ѩ/��ѩ */
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x60,0x30,0x10,0x08,
    0x08,0x08,0x04,0x04,0x04,0x04,0x08,0x08,0x08,0x10,0x30,0x60,0xC0,0x80,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x08,0x04,0x06,0x02,0x02,0x01,
    0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x02,0x02,0x04,0x18,0xE0,0x00,0x00,
    0x00,0x01,0x02,0x04,0x08,0x08,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,
    0x20,0x20,0x20,0x20,0x20,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,
    0x08,0x08,0x04,0x06,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x04,0x3F,
    0x04,0x04,0x04,0xC0,0x00,0x04,0x04,0x3F,0x04,0x04,0x04,0xC0,0x00,0x04,0x04,0x3F,
    0x04,0x04,0x04,0xC0,0x00,0x04,0x04,0x3F,0x04,0x04,0x04,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x0F,0x01,0x01,0x01,0x00,
    0x00,0x01,0x01,0x0F,0x01,0x01,0x01,0x00,0x00,0x01,0x01,0x0F,0x01,0x01,0x01,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

const uint8_t weather_unknow_bmp[] =  /* ����/��ɳ/ɳ����/ǿɳ����/����/��/���/������*/
{
    0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,0x30,0x10,0x08,0x08,0x04,0x04,0x04,0x02,
    0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x04,0x04,0x04,0x08,0x08,0x10,0x10,0x20,0x40,
    0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x38,0x0C,0x03,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x20,0x10,0x10,0x08,0x08,0x10,0x10,0x10,0x60,
    0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x06,0x38,0xE0,0x00,0x00,0x00,
    0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0xF0,0x10,0x08,0x0C,0x06,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0xE3,0x1C,0x00,0x00,0x00,0x01,0x0E,0x18,0x60,0xC0,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x60,0x30,0x0E,0x03,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x06,0x04,0x08,0x08,0x10,0x10,0x10,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x10,0x10,0x10,0x08,0x08,0x04,0x06,0x02,0x01,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

LOCAL void ICACHE_FLASH_ATTR show_weather(uint8_t weather);
LOCAL void ICACHE_FLASH_ATTR show_temperature(int8_t temperature);

void ICACHE_FLASH_ATTR weather_ui_init(void)
{

    /* ����Ԥ���˵� */
    hw_oled_show_chinese(WEATHER_MENU_FONT1_4_COL,WEATHER_MENU_FONT_PAGE,60);
    hw_oled_show_chinese(WEATHER_MENU_FONT2_4_COL,WEATHER_MENU_FONT_PAGE,61);
    hw_oled_show_chinese(WEATHER_MENU_FONT3_4_COL,WEATHER_MENU_FONT_PAGE,64);
    hw_oled_show_chinese(WEATHER_MENU_FONT4_4_COL,WEATHER_MENU_FONT_PAGE,65);

}

void ICACHE_FLASH_ATTR weather_parse(uint8_t *data,uint16_t len)
{

#if defined SENIVERSE_WEATHER

    uint8_t *json_data = os_strstr(data,STRING_SPLIT);
    if(json_data)
    {
        HW_DEBUG("----------json data-----------\n");
        HW_DEBUG("%s\n",json_data);
        HW_DEBUG("----------json data end--------\n");
        cJSON* parse_json = cJSON_Parse((const char *)json_data);

        cJSON *results_json = cJSON_GetObjectItem(parse_json, "results");
        if(results_json)
        {
            cJSON * array_json = cJSON_GetArrayItem(results_json,0);
            if(array_json)
            {
                cJSON *weather_now = cJSON_GetObjectItem(array_json, "now");
                if(weather_now)
                {
                    uint8_t* text_value = (uint8_t*)((cJSON *)cJSON_GetObjectItem(weather_now,"text"))->valuestring;
                    uint8_t* code_value = (uint8_t*)((cJSON *)cJSON_GetObjectItem(weather_now,"code"))->valuestring;
                    uint8_t* temperature_value = (uint8_t*)((cJSON *)cJSON_GetObjectItem(weather_now,"temperature"))->valuestring;
                    uint8_t weather_code = atoi(code_value);
                    int8_t temperature = atoi(temperature_value);
                    HW_DEBUG("text_value %s\n",text_value);
                    HW_DEBUG("code_value string(%s) int(%d)\n",code_value,weather_code);
                    HW_DEBUG("temperature_value string(%s) int(%d)\n",temperature_value,temperature);
                    show_weather(weather_code);
                    show_temperature(temperature);

                }
            }
        }

        cJSON_Delete(parse_json);
    }
#elif defined BAIDU_WEATHER

#endif
}

void ICACHE_FLASH_ATTR weather_first_time_stamp_update(uint8_t *time_stamp)
{
    uint8_t time_hour = atoi(time_stamp);
    os_printf("time_hour %d\n",time_hour);

    if(time_hour  >= 6 && time_hour < 18)
        day_and_night = DAY_TIME;
    else
        day_and_night = NIGHT_TIME;
}

void ICACHE_FLASH_ATTR weather_time_stamp_update(uint8_t *time_stamp)
{

    if(os_strcmp(time_stamp,"06:00:00") == 0)
    {
        day_and_night = DAY_TIME;
        show_weather(last_weather_code);
    }

    if(os_strcmp(time_stamp,"18:00:00") == 0)
    {
        day_and_night = NIGHT_TIME;
        show_weather(last_weather_code);
    }
}

LOCAL void ICACHE_FLASH_ATTR show_weather(uint8_t weather)
{
    HW_DEBUG("show_weather(%d)\n",weather);
    last_weather_code = weather;

#if defined SENIVERSE_WEATHER
    hw_oled_show_string(WEATHER_FONT1_START_COL, WAATHER_FONT_PAGE, "        ", FONT16X8_LENGTH);
    switch(weather)
    {
    case weather_sunny:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_sunny_0_bmp);

        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,0);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,58);
        break;
    }
    case weather_sunny_1:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_sunny_1_bmp);

        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,0);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,58);
        break;
    }
    case weather_cloud:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_cloud_bmp);
        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,1);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,2);

        break;
    }

    case weather_partly_cloudy:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_partly_cloudy_0_bmp);

        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,0);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,66);
        hw_oled_show_chinese(WEATHER_FONT3_START_COL,WAATHER_FONT_PAGE,1);
        hw_oled_show_chinese(WEATHER_FONT4_START_COL,WAATHER_FONT_PAGE,2);
        break;
    }
    case weather_partly_cloudy_1:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_partly_cloudy_1_bmp);

        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,0);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,66);
        hw_oled_show_chinese(WEATHER_FONT3_START_COL,WAATHER_FONT_PAGE,1);
        hw_oled_show_chinese(WEATHER_FONT4_START_COL,WAATHER_FONT_PAGE,2);
        break;
    }
    case weather_mostly_cloudy:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_cloud_bmp);
        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,67);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,68);
        hw_oled_show_chinese(WEATHER_FONT3_START_COL,WAATHER_FONT_PAGE,1);
        hw_oled_show_chinese(WEATHER_FONT4_START_COL,WAATHER_FONT_PAGE,2);
        break;
    }
    case weather_overcast:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_overcast_bmp);
        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,3);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,58);
        break;
    }
    case weather_shower:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_weather_shower_bmp);
        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,4);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,5);
        break;
    }
    case weather_thundershower:
    case weather_thundershower_with_hail:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_thundershower_bmp);
        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,6);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,7);
        hw_oled_show_chinese(WEATHER_FONT3_START_COL,WAATHER_FONT_PAGE,8);
        break;
    }
    case weather_light_rain:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_light_rain_bmp);
        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,9);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,10);
        break;
    }
    case weather_moderate_rain:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_moderate_rain_bmp);
        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,11);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,12);
        break;
    }
    case weather_heavy_rain:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_heavy_rain_bmp);
        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,13);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,14);
        break;
    }
    case weather_storm:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_heavy_rain_bmp);
        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,15);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,16);
        break;
    }
    case weather_heavy_storm:
    case weather_severe_storm:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_heavy_rain_bmp);
        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,17);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,18);
        hw_oled_show_chinese(WEATHER_FONT3_START_COL,WAATHER_FONT_PAGE,19);
        break;
    }

    case weather_ice_rain:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_sleet_bmp);
        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,20);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,21);
        break;
    }
    case weather_sleet:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_sleet_bmp);
        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,22);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,23);
        hw_oled_show_chinese(WEATHER_FONT3_START_COL,WAATHER_FONT_PAGE,24);
        break;
    }
    case weather_snow_flurry:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_light_snow_bmp);
        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,25);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,26);
        break;
    }
    case weather_light_snow:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_light_snow_bmp);
        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,27);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,28);
        break;
    }
    case weather_moderate_snow:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_moderate_snow_bmp);
        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,29);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,30);
        break;
    }
    case weather_heavy_snow:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_heavy_snow_bmp);
        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,31);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,32);
        break;
    }
    case weather_snowstorm:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_heavy_snow_bmp);
        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,33);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,34);
        break;
    }
    case weather_dust:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_unknow_bmp);
        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,35);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,36);
        break;
    }
    case weather_sand:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_unknow_bmp);
        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,37);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,38);
        break;
    }
    case weather_duststorm:
    case weather_sandstorm:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_unknow_bmp);
        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,39);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,40);
        hw_oled_show_chinese(WEATHER_FONT3_START_COL,WAATHER_FONT_PAGE,41);
        break;
    }

    case weather_foggy:
    case weather_haze:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_unknow_bmp);
        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,42);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,43);
        break;
    }


    case weather_windy:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_unknow_bmp);
        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,59);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,44);
        break;
    }
    case weather_blustery:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_unknow_bmp);
        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,45);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,46);
        break;
    }
    case weather_hurricane:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_unknow_bmp);
        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,47);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,48);
        break;
    }
    case weather_tropical_storm:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_unknow_bmp);
        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,49);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,50);
        break;
    }
    case weather_tornado:
    {
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_unknow_bmp);
        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,51);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,52);
        hw_oled_show_chinese(WEATHER_FONT3_START_COL,WAATHER_FONT_PAGE,53);
        break;
    }

    default:
        HW_DEBUG("WARNING:weather code(%d) is invalid\n",weather);
        hw_oled_draw_bmp(WEATHER_IMG_START_COL,WEATHER_IMG_START_PAGE,WEATHER_IMG_END_COL,WEATHER_IMG_END_PAGE,weather_unknow_bmp);
        hw_oled_show_chinese(WEATHER_FONT1_START_COL,WAATHER_FONT_PAGE,56);
        hw_oled_show_chinese(WEATHER_FONT2_START_COL,WAATHER_FONT_PAGE,57);
        break;
    }

#elif defined BAIDU_WEATHER

#endif
}

LOCAL void ICACHE_FLASH_ATTR show_temperature(int8_t temperature)
{
    uint8_t temperature_buf[16] = {0};
    hw_oled_show_string(HUM_FONT_COL, HUM_FONT_PAGE, "      ", FONT16X8_LENGTH);
    os_sprintf(temperature_buf,"%d",temperature);

    hw_oled_show_string(HUM_FONT_COL, HUM_FONT_PAGE, temperature_buf, FONT16X8_LENGTH);
    hw_oled_show_chinese(HUM_FONT_COL+os_strlen(temperature_buf)*FONT16X8_WEIGHT + HUM_FONT_PADDING,HUM_FONT_PAGE,69);
}


