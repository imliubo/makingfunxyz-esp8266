/*
                   _     _                 __
                  | |   (_)               / _|
   _ __ ___    __ _ | | __ _  _ __    __ _ | |_  _   _  _ __     __  __ _   _  ____
  | '_ ` _ \  / _` || |/ /| || '_ \  / _` ||  _|| | | || '_ \    \ \/ /| | | ||_  /
  | | | | | || (_| ||   < | || | | || (_| || |  | |_| || | | | _  >  < | |_| | / /
  |_| |_| |_| \__,_||_|\_\|_||_| |_| \__, ||_|   \__,_||_| |_|(_)/_/\_\ \__, |/___|
                                    __/ |                              __/ |
                                   |___/                              |___/
   MIT License

   Copyright (c) 2018 imliubo

   Github  https://github.com/imliubo
   Website https://www.makingfun.xyz
   Zhihu   https://www.zhihu.com/people/MAKINGFUNXYZ

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/
#include "EspMQTTClient.h"
#include <DHT.h>

#define DHTPIN 3
#define DHTTYPE DHT11   // DHT 11

void onConnectionEstablished();

/*
  EspMQTTClient client(
  "ssid",                 // Wifi ssid
  "pass",                 // Wifi password
  onConnectionEstablished,// Connection established callback
  "ip",                   // MQTT broker ip
  1883,                   // MQTT broker port
  "mqttusr",              // MQTT username
  "mqttpass",             // MQTT password
  "test",                 // Client name
  true,                   // Enable web updater
  true                    // Enable debug messages
  );
*/

EspMQTTClient client(
  "imliubo",                                // Wifi ssid
  "IAMLIUBO",                               // Wifi password
  onConnectionEstablished,                  // MQTT connection established callback
  "k0en8zk.mqtt.iot.bj.baidubce.com",       // MQTT broker ip
  1883,                                     // MQTT broker port
  "k0en8zk/esp8266_device",                 // MQTT username
  "WrE5UUE4Cax9c3P2",                       // MQTT password
  "baiduIoT-esp8266-device1",               // Client name
  false,
  false
);
DHT dht(DHTPIN, DHTTYPE);

long lastMsg = 0;
char msg[20];

void setup()
{
  Serial.begin(115200);

  dht.begin();
}

void LED_Control_Callback(const String & payload) {
  Serial.println("LED callback");
  Serial.println(payload);
}
void Temp_Humd_Report() {
  float humd = dht.readHumidity();
  float temp = dht.readTemperature();
  snprintf (msg, 20, "Temp:%d  Humd:%d",temp,humd);
  
  Serial.println("======Temp Humd Report======");
  Serial.println(msg);
  
  client.publish("/esp8266_test/humd", String(humd));
  client.publish("/esp8266_test/temp", String(temp));
}

void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial
  client.subscribe("/esp8266_test/LEDcontrol", LED_Control_Callback);
}

void loop()
{
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    Temp_Humd_Report();
  }
}
