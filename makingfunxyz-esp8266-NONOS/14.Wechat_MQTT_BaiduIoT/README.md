# Wechat MQTT Baidu IoT example

## Tutorial

[[微信篇]ESP8266-NonOS学习笔记(十)之微信小程序控制LED灯 Part 1](https://zhuanlan.zhihu.com/p/60843533)

小程序源码：[Wechat_MQTT_ESP8266_BaiduIoT](https://github.com/imliubo/Wechat_MQTT_ESP8266_BaiduIoT)

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
LED|<---->|D6(GPIO12)

修改温湿度管脚，请修改以下地方：
1. app/include/driver/hal_temp_humd.h 10L: 
```C
#define DHT11_GPIO_PIN      5
```
2. app/driver/hal_temp_humd.c　200L:
```C
PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);
```
修改LED管脚，请修改下面的地方：
1.app/user/user_main.c 227L 228L
```c
PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U,FUNC_GPIO12);//红灯
GPIO_OUTPUT_SET(GPIO_ID_PIN(12),0);
```
2.app/user/user_main.c 203L 210L
```c
GPIO_OUTPUT_SET(GPIO_ID_PIN(12),1);

GPIO_OUTPUT_SET(GPIO_ID_PIN(12),0);
```


请将MQTT_HOST，MQTT_USER，MQTT_PASS修改成在百度天工注册的信息，如果还没注册可以使用我已经注册好的，不保证使用时长。
修改MQTT或Wi-Fi名称，请修改以下地方：
1. app/include/mqtt_config.h　14L:
```C
#define CFG_HOLDER           0x00FF55A5    /* Change this value to load default configurations */

/*DEFAULT CONFIGURATIONS*/

#define MQTT_HOST            "1v1r5ep.mqtt.iot.bj.baidubce.com" // the IP address or domain name of your MQTT server or MQTT broker ,such as "mqtt.yourdomain.com"
#define MQTT_PORT            1883    // the listening port of your MQTT server or MQTT broker
#define MQTT_CLIENT_ID       "imliubo-esp8266"    // the ID of yourself, any string is OK,client would use this ID register itself to MQTT server
#define MQTT_USER            "1v1r5ep/zhihu_iamliubo" // your MQTT login name, if MQTT server allow anonymous login,any string is OK, otherwise, please input valid login name which you had registered
#define MQTT_PASS            "tNVKODyI2chbm5yp" // you MQTT login password, same as above
#define STA_SSID 			 "CMCC-Master"    // your AP/router SSID to config your device networking
#define STA_PASS 			 "zxcvbnmlk" // your AP/router password
```
每次修改后，把**CFG_HOLDER**值加一或者减一

## Result:

视频改天上传到B站。

**If you have any questions, you can email me or submit an issue.**

