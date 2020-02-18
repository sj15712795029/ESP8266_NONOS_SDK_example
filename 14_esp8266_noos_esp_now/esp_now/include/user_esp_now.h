#ifndef _USER_ESP_NOW_H_
#define _USER_ESP_NOW_H_

void demo_send(uint8_t *mac_addr, uint8_t *data, uint8_t len);
void user_esp_now_init(void);
void user_esp_now_timer_init(void);

#endif /* _USER_ESP_NOW_H_ */
