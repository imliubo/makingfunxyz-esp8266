#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"

#include "driver/uart.h"
#include "modules/config.h"
#include "modules/wifi.h"
#include "debug.h"
#include "user_interface.h"
#include "mem.h"
#include "user_config.h"

#include "sntp.h"

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

typedef unsigned long u32_t;
static ETSTimer sntp_timer;
os_timer_t checkTimer_wifistate;

void sntpfn()
{
    u32_t ts = 0;
    ts = sntp_get_current_timestamp();
    os_printf("\r\nwww.makingfun.xyz\r\n");
    os_printf("time : %s", sntp_get_real_time(ts));
    os_printf("www.makingfun.xyz\r\n");
    if (ts == 0) {
        os_printf("did not get a valid time from sntp server\n");
    } else {
//            os_timer_disarm(&sntp_timer);
    }
}

void Check_WifiState(void) {
	uint8 getState;
	getState = wifi_station_get_connect_status();
	if (getState == STATION_GOT_IP) {
		os_printf("WiFi connection is successful!\r\n");
		os_timer_disarm(&checkTimer_wifistate);
		sntp_setservername(0, "pool.ntp.org");        // set sntp server after got ip address
		sntp_init();
		os_timer_disarm(&sntp_timer);
		os_timer_setfn(&sntp_timer, (os_timer_func_t *)sntpfn, NULL);
		os_timer_arm(&sntp_timer, 10000, 1);//10s
	}else{
		os_printf("WiFi connection failed!\r\n");
	}

}

void wifiConnectCb(uint8_t status)
{
	os_timer_disarm(&checkTimer_wifistate);
	os_timer_setfn(&checkTimer_wifistate, (os_timer_func_t *) Check_WifiState,NULL);
	os_timer_arm(&checkTimer_wifistate, 1000, true);
}


static const partition_item_t at_partition_table[] = {
    { SYSTEM_PARTITION_BOOTLOADER, 						0x0, 												0x1000},
    { SYSTEM_PARTITION_OTA_1,   						0x1000, 											SYSTEM_PARTITION_OTA_SIZE},
    { SYSTEM_PARTITION_OTA_2,   						SYSTEM_PARTITION_OTA_2_ADDR, 						SYSTEM_PARTITION_OTA_SIZE},
    { SYSTEM_PARTITION_RF_CAL,  						SYSTEM_PARTITION_RF_CAL_ADDR, 						0x1000},
    { SYSTEM_PARTITION_PHY_DATA, 						SYSTEM_PARTITION_PHY_DATA_ADDR, 					0x1000},
    { SYSTEM_PARTITION_SYSTEM_PARAMETER, 				SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR, 			0x3000},
};

void ICACHE_FLASH_ATTR user_pre_init(void)
{
    if(!system_partition_table_regist(at_partition_table, sizeof(at_partition_table)/sizeof(at_partition_table[0]),SPI_FLASH_SIZE_MAP)) {
		os_printf("system_partition_table_regist fail\r\n");
		while(1);
	}
}


void user_init(void)
{
    uart_init(BIT_RATE_115200, BIT_RATE_115200);
    os_printf("\r\nwww.makingfun.xyz\r\n");
    wifi_set_opmode(0x01);

    WIFI_Connect(STA_SSID, STA_PASS, wifiConnectCb);

    os_printf("\r\nSystem started ...\r\n");
}
