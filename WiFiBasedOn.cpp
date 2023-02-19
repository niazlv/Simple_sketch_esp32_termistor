#include "esp_wifi_types.h"
#include <sys/_stdint.h>
#include "HardwareSerial.h"
#include "freertos/portmacro.h"
#include "WiFiBasedOn.hpp"

void TaskLoop(void *parameter){
  Serial.println("Start TaskLoopWiFi task!");
  WiFiBasedOn _wifi;
  uint8_t probes = 0;
  while(true) {
    if(!WiFi.isConnected()) {
      Serial.println("Disconnected!");
      Serial.printf("Probe to connect to \"%s\"... Try %d\n",_wifi.client_ssid,probes);
      uint8_t counter = 0;
      if(probes >= 5) {
        probes = 0;
        _wifi.softAP();
      }
      while (WiFi.status() != WL_CONNECTED && counter < 20 && probes < 5) {
        delay(500);
        counter++;
      }
      if(counter >=20) {
        probes++;
      }
      counter = 0;
    }
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
}

uint8_t WiFiBasedOn::softAP(const char *ssid, const char *password) {
  ap_ssid = (char *)ssid;
  ap_password = (char *)password;
  WiFi.mode(WIFI_MODE_AP);
  WiFi.softAP(ssid, password);

  return 0;
}

uint8_t WiFiBasedOn::begin(const char *ssid, const char *password) {
  client_ssid = (char *)ssid;
  client_password = (char *)password;
  WiFi.setHostname("smart Garden esp32");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);

  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  xTaskCreate(TaskLoop,
              "TaskLoopWiFi",
              2*1024,
              NULL,
              2,
              NULL);
  return 0;
}