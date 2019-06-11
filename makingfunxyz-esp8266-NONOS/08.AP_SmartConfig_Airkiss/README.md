# AP SmartConfig AirKiss example

## Tutorial

[[网络篇]ESP8266-NonOS学习笔记(五)之SmartConfig、Airkiss等多种配网](https://zhuanlan.zhihu.com/p/51944852)

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
None

## Result:
**演示视频-> [AP SmartConfig AirKiss example](https://www.bilibili.com/video/av37953711/?p=1)**

**If you have any questions, you can email me or submit an issue.**

