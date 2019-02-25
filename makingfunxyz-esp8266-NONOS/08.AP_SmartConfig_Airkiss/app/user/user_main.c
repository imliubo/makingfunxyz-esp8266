/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "ets_sys.h"
#include "osapi.h"
#include "ip_addr.h"
#include "espconn.h"
#include "mem.h"


#include "user_interface.h"
#include "modules/wifi.h"
#include "user_config.h"
#include "espconn.h"
#include "modules/AP_Web_SmartConfig_Airkiss_test.h"

#if ((SPI_FLASH_SIZE_MAP == 0) || (SPI_FLASH_SIZE_MAP == 1))
#error "The flash map is not supported"
#elif (SPI_FLASH_SIZE_MAP == 2)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x81000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0xfb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0xfc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0xfd000
#elif (SPI_FLASH_SIZE_MAP == 3)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x81000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x1fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x1fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x1fd000
#elif (SPI_FLASH_SIZE_MAP == 4)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x81000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x3fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x3fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x3fd000
#elif (SPI_FLASH_SIZE_MAP == 5)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x101000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x1fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x1fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x1fd000
#elif (SPI_FLASH_SIZE_MAP == 6)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x101000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x3fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x3fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x3fd000
#else
#error "The flash map is not supported"
#endif

static const partition_item_t at_partition_table[] = {
    { SYSTEM_PARTITION_BOOTLOADER, 						0x0, 												0x1000},
    { SYSTEM_PARTITION_OTA_1,   						0x1000, 											SYSTEM_PARTITION_OTA_SIZE},
    { SYSTEM_PARTITION_OTA_2,   						SYSTEM_PARTITION_OTA_2_ADDR, 						SYSTEM_PARTITION_OTA_SIZE},
    { SYSTEM_PARTITION_RF_CAL,  						SYSTEM_PARTITION_RF_CAL_ADDR, 						0x1000},
    { SYSTEM_PARTITION_PHY_DATA, 						SYSTEM_PARTITION_PHY_DATA_ADDR, 					0x1000},
    { SYSTEM_PARTITION_SYSTEM_PARAMETER, 				SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR, 			0x3000},
};


void ICACHE_FLASH_ATTR
user_pre_init(void){
    if(!system_partition_table_regist(at_partition_table, sizeof(at_partition_table)/sizeof(at_partition_table[0]),SPI_FLASH_SIZE_MAP)) {
		os_printf("system_partition_table_regist fail\r\n");
		while(1);
	}
}

os_timer_t key_read_timer;
static void ICACHE_FLASH_ATTR
key_read_value(void){
	os_timer_disarm(&key_read_timer);//取消定时器
	if(GPIO_INPUT_GET(GPIO_ID_PIN(14)) == 0x00){//如果按键按下，低电平
		os_delay_us(20000);//延时20ms消抖
		if(GPIO_INPUT_GET(GPIO_ID_PIN(14)) == 0x00){//再次判断状态，防止误触
			AP_test();
		}
	}else if(GPIO_INPUT_GET(GPIO_ID_PIN(12)) == 0x00){
		os_delay_us(20000);//延时20ms消抖
		if(GPIO_INPUT_GET(GPIO_ID_PIN(12)) == 0x00){//再次判断状态，防止误触
			SmartConfig_test();
		}
	}else if(GPIO_INPUT_GET(GPIO_ID_PIN(5)) == 0x00){
		os_delay_us(20000);//延时20ms消抖
		if(GPIO_INPUT_GET(GPIO_ID_PIN(5)) == 0x00){//再次判断状态，防止误触
			Airkiss_test();
		}
	}else if(GPIO_INPUT_GET(GPIO_ID_PIN(4)) == 0x00){
		os_delay_us(20000);//延时20ms消抖
		if(GPIO_INPUT_GET(GPIO_ID_PIN(4)) == 0x00){//再次判断状态，防止误触
			Web_config_test();
		}
	}
	os_timer_arm(&key_read_timer, 10, true);//使能定时器
}

void ICACHE_FLASH_ATTR
user_init(void)
{
    os_printf("\nAP Smartconfig Airkiss EXAMPLE! ZHIHU IAMLIUBO\n\n");

    wifi_set_opmode(STATION_MODE);
	wifi_station_set_auto_connect(TRUE);

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U,FUNC_GPIO14);//按键
	GPIO_DIS_OUTPUT(GPIO_ID_PIN(14));//按键设置为输入模式
	PIN_PULLUP_EN(PERIPHS_IO_MUX_MTMS_U);//引脚上拉使能

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U,FUNC_GPIO12);//按键
	GPIO_DIS_OUTPUT(GPIO_ID_PIN(12));//按键设置为输入模式
	PIN_PULLUP_EN(PERIPHS_IO_MUX_MTDI_U);//引脚上拉使能

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U,FUNC_GPIO5);//按键
	GPIO_DIS_OUTPUT(GPIO_ID_PIN(5));//按键设置为输入模式
	PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO5_U);//引脚上拉使能

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U,FUNC_GPIO4);//按键
	GPIO_DIS_OUTPUT(GPIO_ID_PIN(4));//按键设置为输入模式
	PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO4_U);//引脚上拉使能


	os_timer_disarm(&key_read_timer);
	os_timer_setfn(&key_read_timer, (os_timer_func_t *) key_read_value,NULL);//定时回调函数
	os_timer_arm(&key_read_timer, 1000, true);//设置时间为100ms



}
