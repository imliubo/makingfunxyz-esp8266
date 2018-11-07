## Major Changes

### SDK

- Memory optimization: iram can be used as memory, thus saving about 17KB memory for user application.

### AT 

- Update AT's version to 1.7.0.0.
- Limited by the size of the AT bin files, only 1024+1024 flash map is supported by default. Therefore, please choose 16Mbit/32Mbit-C1 flash size when flashing these AT bin files with Espressif's Flash Download Tools.

## New Features and Optimizations

### SDK

- Add partition table to manage flash partition, more details are in documents/CN/Partition Table 说明.md or documents/EN/Partition Table.md.
	- system_partition_table_regist
	- system_partition_get_ota_partition_size
	- system_partition_get_item
- Add WiFi APIs to set sleep level of modem sleep and light sleep.
	- wifi_set_sleep_level
	- wifi_get_sleep_level
	- wifi_set_listen_interval
	- wifi_get_listen_interval
- Add signalling measurement feature
	- wifi_enable_signaling_measurement
	- wifi_disable_signaling_measurement
- Support random DHCP transaction id
- Add libhal.a for the new compiler

### AT
- Add command AT+SYSMSG_DEF and AT+SYSMSG_CUR to instead of AT+SYSMSG.
- Add AT+CIPCLOSEMODE to select TCP close mode.
- Add a parameter in AT+CWJAP_CUR and AT+CWJAP_DEF to disable the connection to open/wep router
- Add AT+CIPRECVLEN to get the length of received data in passive receiving mode.
- Add AT+CIPRECVMODE to set TCP receive mode.
- Add commands to configure SSL client authentication
	- AT+CIPSSLCCONF?
	- AT+CIPSSLCCONF=<ssl_mode>
- Replace ssl with mbedtls in AT
- Make SmartConfig optional, users can disable it in application for smaller bin size and more memory.
- Disable AT+RFAUTOTRACE command.

## Bug Fix

### SDK
- Fix issue that system crashes when calling os_printf_plus in interrupt handler.
- fix issue that mbedtls cannot set the fragment to be 2048.
- fix issue that OTA may crash with some server.
- Fix issue that beacon info does not update after wifi_station_scan.
- Fix softAP compatibility issue.
- Fix issue that fails to scan hidden AP in station mode.

### AT
- Fix issue that GPIO related commands always return OK even without GPIO number.
- Fix issue that extra 0x0A displayed in SNTP time string