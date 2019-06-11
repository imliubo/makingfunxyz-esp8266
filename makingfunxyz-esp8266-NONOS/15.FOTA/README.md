# FOTA upgrade example

## Tutorial

[[网络篇]ESP8266-NonOS学习笔记(十二)之FOTA无线固件更新升级](https://zhuanlan.zhihu.com/p/60922469/)


## Hardware:
* NODEMCU

## Tool:
* [ESPFlashDownload](https://www.espressif.com/sites/default/files/tools/flash_download_tools_v3.6.4.rar)

## Download:

### SUPPORT FOTA:

#### first compile
change the code like below
```makefile
#Makefile of project line25-line29
BOOT?=new
APP?=1
SPI_SPEED?=40
SPI_MODE?=DIO
SPI_SIZE_MAP?=6
```
generate like below
```bash
!!!
592040477
592040478
Support boot_v1.4 and +
Generate user1.4096.new.6.bin successully in folder bin/upgrade.
boot.bin------------>0x00000
user1.4096.new.6.bin--->0x01000
!!!
```
#### second compile
**you need make clean this project before second compile**
change the code like below
```makefile
#Makefile of project line25-line29
BOOT?=new
APP?=2
SPI_SPEED?=40
SPI_MODE?=DIO
SPI_SIZE_MAP?=6
```
generate like below
```bash
!!!
!!!
1282286818
1282286819
Support boot_v1.4 and +
Generate user2.4096.new.6.bin successully in folder bin/upgrade.
boot.bin------------>0x00000
user2.4096.new.6.bin--->0x101000
!!!
```

* boot.bin-------->0x00000
* user1.4096.new.6.binn---->0x01000
* blank.bin --> 0x3FB000
* esp_init_data_default_v08.bin --> 0x3FC000
* blank.bin --> 0x3FE000

### NOT SUPPORT OTA:

**This demo support OTA!**

## Options:
* CrystalFreq:26M
* SPI SPEED: 40MHz
* SPI MODE: DIO
* FLASH SIZE: 32Mbit-C1
* BAUD: 115200

## Hardware connection:
no haedware connection!

## Attention

**you need change some code: app/include/user_config.h**
```C
#define STA_SSID "CMCC-7uHL"
#define STA_PASS "zxcvbnmlk"

#define OTA_SERVER_IP   "192.168.1.13"
#define OTA_SERVER_PORT 5000
```

## Result:



**If you have any questions, you can email me or submit an issue.**


