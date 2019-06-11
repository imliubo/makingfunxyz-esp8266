# MQTT DHT11 Temp Humd example

## Tutorial


## Hardware:
* NODEMCU

## Tool:
* [ESPFlashDownload](https://www.espressif.com/sites/default/files/tools/flash_download_tools_v3.6.4.rar)

## Download:

### SUPPORT OTA:

**This demo do not support OTA!**

### NOT SUPPORT OTA:

change some code like below
```makefile
#Makefile of project line25-line29
BOOT?=none
APP?=0
SPI_SPEED?=40
SPI_MODE?=DIO
SPI_SIZE_MAP?=6
```

* eagle.flash.bin-------->0x00000
* eagle.irom0text.bin---->0x10000
* blank.bin --> 0x3FE000
* esp_init_data_default_v08.bin --> 0x3FC000

## Options:
* CrystalFreq:26M
* SPI SPEED: 40MHz
* SPI MODE: DIO
* FLASH SIZE: 32Mbit-C1
* BAUD: 115200

## Hardware connection:

DHT11|<---->|NODEMCU
-|-|-
Data|<---->| D1(GPIO5)

修改管脚，请修改以下地方：
1. app/include/driver/hal_temp_humd.h 10L: 
```C
#define DHT11_GPIO_PIN      5
```
2. app/driver/hal_temp_humd.c　200L:
```C
PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);
```

修改MQTT或Wi-Fi名称，请修改以下地方：
1. app/include/mqtt_config.h　14L:
```C
#define CFG_HOLDER           0x00FF55A4    /* Change this value to load default configurations */

/*DEFAULT CONFIGURATIONS*/

#define MQTT_HOST            "192.168.0.106" // the IP address or domain name of your MQTT server or MQTT broker ,such as "mqtt.yourdomain.com"
#define MQTT_PORT            1883    // the listening port of your MQTT server or MQTT broker
#define MQTT_CLIENT_ID       "imliubo-esp8266"    // the ID of yourself, any string is OK,client would use this ID register itself to MQTT server
#define MQTT_USER            "admin" // your MQTT login name, if MQTT server allow anonymous login,any string is OK, otherwise, please input valid login name which you had registered
#define MQTT_PASS            "admin" // you MQTT login password, same as above
#define STA_SSID 			 "CMCC-Master"    // your AP/router SSID to config your device networking
#define STA_PASS 			 "zxcvbnmlk" // your AP/router password
```
每次修改后，把**CFG_HOLDER**值加一或者减一

## Result:

串口：

![串口打印](https://makingfun.oss-cn-qingdao.aliyuncs.com/zhihu-IAMLIUBO/terminal.png)

手机MQTT助手：

![手机MQTT助手：](https://makingfun.oss-cn-qingdao.aliyuncs.com/zhihu-IAMLIUBO/mqtt_assistant.png)

**If you have any questions, you can email me or submit an issue.**

