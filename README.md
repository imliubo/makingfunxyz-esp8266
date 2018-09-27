# makingfunxyz-esp8266
个人主页：[www.makingfun.xyz](www.makingfun.xyz)

## makingfunxyz-esp8266-NONOS example

- [x] [ESP8266-nonOS-SNTP](https://github.com/imliubo/makingfunxyz-esp8266/tree/master/makingfunxyz-esp8266-NONOS/1.SNTP)

## makingfunxyz-esp8266-RTOS example

- [x] [ESP8266-RTOS-hello world](https://github.com/imliubo/makingfunxyz-esp8266/tree/master/makingfunxyz-esp8266-RTOS/1.hello_world)

好久没有关注ESP8266，最近看到ESP8266的SDK进行了一次大升级，正在逐步往ESP32的风格靠近，最重要的是ESP-IDF也将会支持ESP8266的开发，可以预见的是将来ESP8266和ESP32的开发将会更加便利。

## ESP8266 SDK v3.0 更新说明
* BootLoader 兼容 ESP-IDF
* 大部分 API 接口兼容 ESP-IDF
* 重构 Wi-Fi 底层架构，增强稳定性
* 支持 ESP Log，用户可以配置不同的打印信息等级
* 支持 NVS 和 Partition 存储方式
* 支持开启 Menu 配置界面，支持命令行下载，命令行串口监控等工具
* 支持 UART，GPIO 驱动，其他更多的驱动将在后续版本添加
* 完善对 newlib 的支持， 增加了支持多线程、Nano 和 Normal 模式、时间管理接口
* 更新 FreeRTOS 至版本 v10.0.1
* 更新 LWIP 至版本 v2.0.3
* 更新 mbedTLS 至版本 v2.8.0
* 提供 AWS IoT，MQTT，OTA，HTTP，HTTPS 等应用示例

## 环境搭建失败，具体原因不详

没想到，简简单单的一个环境搭建就失败了，真是让人感觉到羞愧，最可怕的是连失败的原因都不晓得，好在有人遇到了跟我类似的问题，不是我自己一个人的问题，具体原因还在等官方回复，等待~
原因已找到,万万让人没有想到的是，竟然是解压工具的锅，估计是不同平台下的压缩工具算法不同，导致解压出来的文件少了很多。解决办法查看[这里](https://github.com/espressif/ESP8266_RTOS_SDK/issues/317)。

原文链接：[乐鑫 物联网操作系统 即将同时支持 ESP8266 和 ESP32](https://mp.weixin.qq.com/s/x8fIFkns7DDFmqX1Wjn5mQ)

