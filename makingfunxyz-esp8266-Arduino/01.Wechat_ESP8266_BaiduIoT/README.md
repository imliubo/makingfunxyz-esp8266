## Wechat mini program control ESP8266 with Baidu IoT platform example

这是微信小程序控制ESP8266 Arduino版本代码。

### Wechat mini program source

- [Wechat_MQTT_ESP8266_BaiduIoT](https://github.com/imliubo/Wechat_MQTT_ESP8266_BaiduIoT)

### Dependent library

- [EspMQTTClient plapointe6](https://github.com/plapointe6/EspMQTTClient)

- [DHT-sensor-library adafruit](https://github.com/adafruit/DHT-sensor-library)

#### Please attention！

Please add this file [Adafruit_Sensor.h](https://github.com/adafruit/Adafruit_Sensor/blob/master/Adafruit_Sensor.h) to the DHT-sensor-library Dir.

### NEED MODIFY!

Modify some information to suit your project!
```C
EspMQTTClient client(
  "imliubo",                                // Wifi ssid
  "IAMLIUBO",                               // Wifi password
  onConnectionEstablished,                  // MQTT connection established callback
  "1v1r5ep.mqtt.iot.bj.baidubce.com",       // MQTT broker ip
  1883,                                     // MQTT broker port
  "1v1r5ep/zhihu_iamliubo",                 // MQTT username
  "tNVKODyI2chbm5yp",                       // MQTT password
  "baiduIoT-esp8266-device1",               // Client name
  false,
  false
);
```

### Hardware:

* NODEMCU
* DHT11 sensor

### Tool:
* [Arduino IDE](https://www.arduino.cc/en/Main/Software)

### Hardware connection:

DHT11|<---->|NODEMCU
-|-|-
Data|<---->| D4(GPIO2)


**If you have any questions, you can email me or submit an issue.**
