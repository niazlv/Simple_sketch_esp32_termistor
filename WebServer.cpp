#include "HardwareSerial.h"
#include "freertos/portmacro.h"
#include "WebServer.hpp"
#include "FFat.h"


void TaskToWebServer(void *parameter) {
  Serial.println("web server handler started!");

  server.on("/all", HTTP_GET, []() {
    String json = "{";
    json += "\"heap\":" + String(ESP.getFreeHeap());
    json += ", \"analog\":" + String(analogRead(0));
    json += ", \"gpio\":" + String((uint32_t)(0));
    json += "}";
    server.send(200, "text/json", json);
    json = String();
  });

  server.on("/file",HTTP_GET,[](){
    if (!server.hasArg("file")) {
    server.send(500, "text/plain", "BAD ARGS");
    return;
    }

    String path = server.arg("file");
    Serial.println("file: " + path);


    File file = FFat.open(path);

    server.streamFile(file, "text/plain");
    file.close();
    //server.send(200,"text/plain", "test\n");
  });

  server.begin();
  while(1) {
    server.handleClient();
    vTaskDelay(10/portTICK_PERIOD_MS);
  }
}

uint8_t WebServerClass::begin() {
  
  xTaskCreate(TaskToWebServer,
              "TaskToWebServer",
              8*1024,
              NULL,
              2,
              NULL);
  return 0;
}