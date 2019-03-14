/* main.c -- MQTT client example
*
* Copyright (c) 2014-2015, Tuan PM <tuanpm at live dot com>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
* * Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
* * Neither the name of Redis nor the names of its contributors may be used
* to endorse or promote products derived from this software without
* specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/

#include "ets_sys.h"
#include "driver/uart.h"
#include "osapi.h"
#include "mqtt.h"
#include "wifi.h"
#include "config.h"
#include "debug.h"
#include "gpio.h"
#include "user_interface.h"
#include "mem.h"

#include "modules/hal_temp_hum.h"
#include "sntp.h"
#include "modules/ssd1306.h"

#if ((SPI_FLASH_SIZE_MAP == 0) || (SPI_FLASH_SIZE_MAP == 1))
#error "The flash map is not supported"
#elif (SPI_FLASH_SIZE_MAP == 2)
#define SYSTEM_PARTITION_OTA_SIZE                           0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR                         0x81000
#define SYSTEM_PARTITION_RF_CAL_ADDR                        0xfb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR                      0xfc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR              0xfd000
#elif (SPI_FLASH_SIZE_MAP == 3)
#define SYSTEM_PARTITION_OTA_SIZE                           0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR                         0x81000
#define SYSTEM_PARTITION_RF_CAL_ADDR                        0x1fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR                      0x1fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR              0x1fd000
#elif (SPI_FLASH_SIZE_MAP == 4)
#define SYSTEM_PARTITION_OTA_SIZE                           0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR                         0x81000
#define SYSTEM_PARTITION_RF_CAL_ADDR                        0x3fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR                      0x3fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR              0x3fd000
#elif (SPI_FLASH_SIZE_MAP == 5)
#define SYSTEM_PARTITION_OTA_SIZE                           0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR                         0x101000
#define SYSTEM_PARTITION_RF_CAL_ADDR                        0x1fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR                      0x1fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR              0x1fd000
#elif (SPI_FLASH_SIZE_MAP == 6)
#define SYSTEM_PARTITION_OTA_SIZE                           0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR                         0x101000
#define SYSTEM_PARTITION_RF_CAL_ADDR                        0x3fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR                      0x3fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR              0x3fd000
#else
#error "The flash map is not supported"
#endif


static const partition_item_t at_partition_table[] =
{
    { SYSTEM_PARTITION_BOOTLOADER,                      0x0,                                                0x1000},
    { SYSTEM_PARTITION_OTA_1,                           0x1000,                                             SYSTEM_PARTITION_OTA_SIZE},
    { SYSTEM_PARTITION_OTA_2,                           SYSTEM_PARTITION_OTA_2_ADDR,                        SYSTEM_PARTITION_OTA_SIZE},
    { SYSTEM_PARTITION_RF_CAL,                          SYSTEM_PARTITION_RF_CAL_ADDR,                       0x1000},
    { SYSTEM_PARTITION_PHY_DATA,                        SYSTEM_PARTITION_PHY_DATA_ADDR,                     0x1000},
    { SYSTEM_PARTITION_SYSTEM_PARAMETER,                SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR,             0x3000},
};

void ICACHE_FLASH_ATTR user_pre_init(void)
{
    if(!system_partition_table_regist(at_partition_table, sizeof(at_partition_table) / sizeof(at_partition_table[0]), SPI_FLASH_SIZE_MAP))
    {
        os_printf("system_partition_table_regist fail\r\n");
        while(1);
    }
}

uint8 temp,humd;
os_timer_t DHT11_Read_timer;

void ICACHE_FLASH_ATTR DHT11_Read(void)
{
    char temp_buffer[10],humd_buffer[10];
    os_timer_disarm(&DHT11_Read_timer);//取消定时器
    dh11Read(&temp,&humd);
    os_printf("Temp: %d'C Humd: %d%\n",temp,humd);
    os_sprintf(temp_buffer,":%d'",temp);
    os_sprintf(humd_buffer,":%d%",humd);
    ssd1306_SetCursor(0, 40);
    ssd1306_WriteZhString("温度",White);
    ssd1306_SetCursor(30, 44);
    ssd1306_WriteString(temp_buffer,Font_7x10,White);
    ssd1306_SetCursor(64, 40);
    ssd1306_WriteZhString("湿度",White);
    ssd1306_SetCursor(94, 44);
    ssd1306_WriteString(humd_buffer,Font_7x10,White);
    ssd1306_UpdateScreen();
    os_timer_arm(&DHT11_Read_timer, 5000, true);//使能定时器
}

void user_init(void)
{
    uart_init(115200, 115200);

    dh11Init();

    i2c_master_gpio_init();

    ssd1306_Init();

    ssd1306_Fill(Black);
    ssd1306_SetCursor(8, 0);
    ssd1306_WriteZhString("神奇物联网之旅",White);
    ssd1306_SetCursor(20, 16);
    ssd1306_WriteString("IAMLIUBO",Font_11x18,White);
    ssd1306_UpdateScreen();

    os_timer_disarm(&DHT11_Read_timer);//取消定时器
    os_timer_setfn(&DHT11_Read_timer, (os_timer_func_t *) DHT11_Read,NULL);//定时回调函数
    os_timer_arm(&DHT11_Read_timer, 5000, true);//使能定时器，设置时间为1s

}
