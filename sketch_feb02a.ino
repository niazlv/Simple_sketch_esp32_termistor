#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include "Termistor.hpp"
#include "FileSystem.hpp" //default FFat 4MB

//dht settings
#define DHTPIN 23       // пин на котором сидит датчик
#define DHTTYPE DHT11   // его тип: DHT11, DHT22(AM2302, AM2321), DHT21(AM2301)



DHT_Unified dht(DHTPIN, DHTTYPE);
Termistor *tr[2];
FileSystem fs_ex;


void printTemps() {
  float temp = tr[0]->getTemp();
  sensors_event_t event;
  
  dht.temperature().getEvent(&event);
  float t = event.temperature;
  
  dht.humidity().getEvent(&event);
  float h = event.relative_humidity;
  
  float hic = (DHT(0,DHT11).computeHeatIndex(t, h, false));
  float hictemp = (DHT(0,DHT11).computeHeatIndex(temp, h, false));
  Serial.printf("DHT: t: %.2f°C, h: %.0f%%, Heat index: %0.2f°C\n",t,h,hic);
  Serial.printf("Termistor: %.2f, heat index: %.2f\n", temp,hictemp);
  Serial.println();
}

uint8_t writeTempsToFile() {
  float temp = tr[0]->getTemp();
  sensors_event_t event;
  
  dht.temperature().getEvent(&event);
  float t = event.temperature;
  
  dht.humidity().getEvent(&event);
  float h = event.relative_humidity;
  
  // float hic = (DHT(0,DHT11).computeHeatIndex(t, h, false));
  // float hictemp = (DHT(0,DHT11).computeHeatIndex(temp, h, false));

  String msg;
  // msg = "DHT_1: t:%.2f°C, h:%.0f%%, HI:%0.2f°C\n";
  // msg += "T_1: t:%.2f, HI:%.2f\n";
  
  // msg = "temp; humidity; /*HEAT INDEX;*/ NAME\n";
  msg  =  String(t)     + ";" + (int)h  +   /*";" + hic     + */";D1\n";
  msg +=  String(temp)  + ";" +             /*";" + hictemp + */";T1\n";
  File dir = FFat.open("/temperature");
  if(!dir) {
    fs_ex.mkdir("/temperature");
  }
  dir.close();
  File f = FFat.open("/temperature/tempdata.txt",FILE_READ);
  if(!f) {
    f.close();
    fs_ex.write("/temperature/tempdata.txt", "temp; humidity; NAME\n");
  }
  else {
    Serial.println("File \"/temperature/tempdata.txt\" reading...");
    //Serial.println(f.readString());
    //Serial.println();
    f.close();
  }
  fs_ex.appendFile("/temperature/tempdata.txt", msg.c_str());
  return 0;
}

void setup() {
  //pinMode(12,INPUT);
  Serial.begin(115200);
  tr[0] = new Termistor(12,3950);
  
  //termal sensors
  dht.begin();
  tr[0]->begin();

  // fs
  // FFat.format();
  fs_ex.begin();

  File f = FFat.open("/AIndex.FSList",FILE_READ);
  if(!f){
    Serial.println("file index not found! Create him!");
    f.close();
    fs_ex.write("/AIndex.FSList", "\n");
  }
  else {
    Serial.println("Reading AIndex.FSList file....");
    //Serial.println(f.readString());
    f.close();
  }

  f = FFat.open("/LOG.txt",FILE_READ);
  if(!f){
    Serial.println("file logs not found! Create him!");
    f.close();
    fs_ex.write("/LOG.txt", "logs:");
  }
  else {
    f.close();
  }
  
  File root = FFat.open("/");
  Serial.println(root.getNextFileName());
  if(root.getNextFileName() == String("/?")) {
    f = FFat.open("/LOG.txt",FILE_READ);
    String st = f.readString();
    Serial.println(st);
    f.close();
    FFat.format();
    //FFat.remove("/LOG.txt");
    //fs_ex.write("/LOG.txt", st.c_str());
  }
  //fs_ex.write("/lool.txt", "loooooooooool\n");
}

void loop() {
  //printTemps();
  fs_ex.listDir("/",2);
  writeTempsToFile();
  delay(10000);
  //fs_ex.listDir("/",2);
  //String str = "start: ";
  //str += random(100,10000);
  //str += "\n";
  //fs_ex.appendFile("/LOG.txt", str.c_str());
  //delay(2000);
  //Serial.println("reboot..");
  //delay(1000);
  //while(1){delay(100);}
  //ESP.restart();
}