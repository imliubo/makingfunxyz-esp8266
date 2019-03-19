# Web Config Wi-Fi SSID PWD example

## Tutorial

[[网络篇]ESP8266-NonOS学习笔记(六)之网页配置Wi-Fi名称和密码](https://zhuanlan.zhihu.com/p/52420696)

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

* makingfun.html --> 0x1FC000
* WebConfig.html --> 0x1FE000
* WiFiDone.html  --> 0x200000

0x1FC000(hex) => 2080768
508 = 2080768/4096

```C
ret = spi_flash_read(508*4096, (uint32 *)html, INDEX_SIZE);
```

## Options:
* CrystalFreq:26M
* SPI SPEED: 40MHz
* SPI MODE: DIO
* FLASH SIZE: 32Mbit-C1
* BAUD: 115200

## Hardware connection:
None

## Result:
**演示视频-> [Web Config Wi-Fi SSID PWD](https://www.bilibili.com/video/av37953711/?p=2)**

**If you have any questions, you can email me or submit an issue.**

