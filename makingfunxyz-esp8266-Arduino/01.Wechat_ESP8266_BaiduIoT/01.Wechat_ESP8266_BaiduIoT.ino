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
#include "string.h"
#include "EspMQTTClient.h"
#include <DHT.h>

#define DHTPIN 2        //D4
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
  "1v1r5ep.mqtt.iot.bj.baidubce.com",       // MQTT broker ip
  1883,                                     // MQTT broker port
  "1v1r5ep/zhihu_iamliubo",                 // MQTT username
  "tNVKODyl2chbm5yp",                       // MQTT password
  "baiduIoT-esp8266-device1",               // Client name
  false,
  false
);
DHT dht(DHTPIN, DHTTYPE);

long lastMsg = 0;
char msg[23];


void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  dht.begin();
  digitalWrite(LED_BUILTIN, HIGH);
}

void LED_Control_Callback(const String & payload) {
  const char* p = payload.c_str();
  Serial.println("LED callback");
  Serial.println(p);
  if(strstr(p,"On")){
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on 
  }
  if(strstr(p,"Off")){
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off
  }
}
void Temp_Humd_Report() {
  float humd = dht.readHumidity();
  float temp = dht.readTemperature();
  sprintf (msg,"Temp:%.2f'C  Humd:%.0f% ",temp,humd);
  
  Serial.println("======Temp Humd Report======");
  Serial.println(msg);
  
  client.publish("/zhihu_esp8266/humd", String(humd));
  client.publish("/zhihu_esp8266/temp", String(temp));
}

void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial
  client.subscribe("/zhihu_esp8266/LEDcontrol", LED_Control_Callback);
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
