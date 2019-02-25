/*
 * Adafruit_NeoPixel.h
 *
 *  Created on: 2018Äê9ÔÂ27ÈÕ
 *      Author: LIUBO
 */

#ifndef APP_INCLUDE_MODULES_ADAFRUIT_NEOPIXEL_H_
#define APP_INCLUDE_MODULES_ADAFRUIT_NEOPIXEL_H_

#include "c_types.h"
#include "user_interface.h"
#include "ets_sys.h"
#include "gpio.h"

//thanks for https://github.com/cnlohr/ws2812esp8266
//thanks for https://github.com/adafruit/Adafruit_NeoPixel

#define WSGPIO 0 //must use the ESP8266 GPIO0 as the pin to drive WS2812B RGB LED!!!
//user can change
#define PIXEL_MAX 78 //the total numbers of LEDs you are used in your project

//You will have to 	os_intr_lock();  	os_intr_unlock();

void ICACHE_FLASH_ATTR setAllPixelColor(uint8_t r, uint8_t g, uint8_t b);
void ICACHE_FLASH_ATTR setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
void SetPixelColor(uint16_t n, uint32_t c);
uint32_t Color(uint8_t r, uint8_t g, uint8_t b);
uint32_t Wheel(uint8_t WheelPos);
void rainbowCycle(uint8_t wait) ;
void theaterChase(uint32_t c, uint8_t wait);
void theaterChaseRainbow(uint8_t wait);
void colorWipe(uint32_t c, uint8_t wait);
void WS2812B_Test(void);
void ICACHE_FLASH_ATTR WS2812B_Init(void);



#endif /* APP_INCLUDE_MODULES_ADAFRUIT_NEOPIXEL_H_ */
