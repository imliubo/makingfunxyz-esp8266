# DHT11 Temp Humd example

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


## Result:

```bash
Temp: 16  Humd: 19
Temp: 16  Humd: 19
Temp: 16  Humd: 19
Temp: 16  Humd: 19
Temp: 17  Humd: 19
Temp: 17  Humd: 19
Temp: 17  Humd: 22
Temp: 18  Humd: 27
Temp: 18  Humd: 31
```

**If you have any questions, you can email me or submit an issue.**

