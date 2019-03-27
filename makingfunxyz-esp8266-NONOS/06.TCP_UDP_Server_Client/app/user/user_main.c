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
#include "modules/tcpServerClient.h"

#if  TCP_CLIENT
struct espconn tcp_client;
#else
struct espconn *tcp_server;
#endif
os_timer_t wifistate_checktimer;
os_timer_t send_data_timer;

void ICACHE_FLASH_ATTR
TCP_Send_data(void){
#if  TCP_CLIENT
	tcp_client_send_data(&tcp_client,"hi this is ESP8266 client!",strlen("hi this is ESP8266 client!"));
#else
	tcp_server_send_data(tcp_server,"hi this is ESP8266 server!",strlen("hi this is ESP8266 server!"));
#endif
}

void ICACHE_FLASH_ATTR
WifiStatus_Check(void){
	uint8 wifiStatus;
	wifiStatus = wifi_station_get_connect_status();
	if (wifiStatus == STATION_GOT_IP) {
		os_printf("WiFi connection is successful!\r\n");
		os_timer_disarm(&wifistate_checktimer);
		struct ip_info local_ip;
		wifi_get_ip_info(STATION_IF,&local_ip);
#if  TCP_CLIENT
		tcp_client_init(&tcp_client,TCP_SERVER_IP,&local_ip.ip,TCP_SERVER_PORT);//TCP Client初始化，Client与Server只能二选一
		os_timer_disarm(&send_data_timer);
		os_timer_setfn(&send_data_timer, (os_timer_func_t *) TCP_Send_data,NULL);
		os_timer_arm(&send_data_timer, 2000, true);
#else
		tcp_server_init(tcp_server,TCP_LOCAL_PORT);//TCP Server初始化,Client与Server只能二选一
		os_timer_disarm(&send_data_timer);
		os_timer_setfn(&send_data_timer, (os_timer_func_t *) TCP_Send_data,NULL);
		os_timer_arm(&send_data_timer, 2000, true);
#endif

	}else{
		os_printf("WiFi connection failed!\r\n");
	}
}

/**
 * Wi-Fi连接回调函数
 */
void ICACHE_FLASH_ATTR
wifiConnectCb(uint8_t status){

	os_timer_disarm(&wifistate_checktimer);
	os_timer_setfn(&wifistate_checktimer, (os_timer_func_t *) WifiStatus_Check,NULL);
	os_timer_arm(&wifistate_checktimer, 2000, true);
}

void ICACHE_FLASH_ATTR
user_init(void)
{
    os_printf("\nHello World! ZHIHU IAMLIUBO\n\n");

    wifi_set_opmode(0x01);

    WIFI_Connect(STA_SSID, STA_PASS, wifiConnectCb);
}


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
