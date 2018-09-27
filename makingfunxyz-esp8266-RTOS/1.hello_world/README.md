# hello world example

## Hardware:
* NODEMCU

## Tool:
If you have not used ESP-IDF yet, please read this document first.

* [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/stable/index.html)
* [Start a project with ESP8266 RTOS](https://github.com/espressif/ESP8266_RTOS_SDK/blob/master/README.md)


## Configure:
```bash
make menucofig
```

* Serial flasher config
	* Serial port --> choose your hardware port
	* Flash SPI mode --> according your hardware, if use MODEMCU --> DIO
	* Flash SPI speed --> 40MHz
	* Flash size --> 4MB
	* save 

## Build and Flash:
```bash
make flash
```

## Monitor:
```bash
make monitor
```

## Rebuild:
if you change your code!
```bash
make app-flash
```

## Hardware connection:
* None

## Result:
# 