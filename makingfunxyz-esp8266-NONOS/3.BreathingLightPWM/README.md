# PWM control LED example

## Hardware:
* NODEMCU
* LED

## Tool:
* [ESPFlashDownload](https://www.espressif.com/sites/default/files/tools/flash_download_tools_v3.6.4.rar)

## Download:

### SUPPORT OTA:

change some code like below
```makefile
#Makefile of project line25-line29
BOOT?=new
APP?=1
SPI_SPEED?=40
SPI_MODE?=DIO
SPI_SIZE_MAP?=6
```

* boot_v1.7.bin --> 0x00000
* user1.4096.new.6.bin --> 0x01000
* blank.bin --> 0x3FE000
* esp_init_data_default_v08.bin --> 0x3FC000

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
LED|<---->|NODEMCU
-|-|-
positive electrode|<---->| D6(GPIO12)

## Result:

will up vedio on youku!