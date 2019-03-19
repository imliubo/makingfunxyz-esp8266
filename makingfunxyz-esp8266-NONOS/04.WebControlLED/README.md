# Web Button control LED example

## Hardware:
* NODEMCU
* LED

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

* makingfunxyz.html --> 0x1FC000

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
LED|<---->|NODEMCU
-|-|-
positive electrode|<---->| D6(GPIO12)

## Result:

![img](https://github.com/imliubo/makingfunxyz-esp8266/blob/master/makingfunxyz-esp8266-NONOS/04.WebControlLED/img.jpg)
