#include <Arduino.h>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <M5EPD.h>
#include <WiFi.h> 
#include <HTTPClient.h>
#include <vector>
#include <regex>
#include "FS.h"
#include "SPIFFS.h"
#include "tinyxml2.h"

M5EPD_Canvas canvas(&M5.EPD);
using namespace tinyxml2;
XMLDocument doc;
rtc_time_t RTCtime;
rtc_date_t RTCDate;

char temStr[10];
char humStr[10];
char timeStrbuff[64];

float tem;
float hum;

//Parser Orig
std::vector<int> find_all(const std::string str, const std::string subStr) {
    std::vector<int> result;
 
    int subStrSize = subStr.size();
    int pos = str.find(subStr);
 
    while (pos != std::string::npos) {
        result.push_back(pos);
        pos = str.find(subStr, pos + subStrSize);
    }
 
    return result;
}

//RTC1
void flushTime(){
    M5.RTC.getTime(&RTCtime);
    M5.RTC.getDate(&RTCDate);

    // sprintf(timeStrbuff,"%d/%02d/%02d %02d:%02d:%02d",
    sprintf(timeStrbuff,"%d/%02d/%02d %02d:%02d",
                        RTCDate.year,RTCDate.mon,RTCDate.day,
                        RTCtime.hour,RTCtime.min);
    
    canvas.drawString(timeStrbuff, 10, 900);
    // canvas.pushCanvas(0,0,UPDATE_MODE_A2);
}

// Battery
void flushBattery(){

  char buf[20];
  
  uint32_t vol = M5.getBatteryVoltage();
  if(vol < 3300){
      vol = 3300;
  }else if(vol > 4350){
      vol = 4350;
  }

  float battery = (float)(vol - 3300) / (float)(4350 - 3300);
  
  if(battery <= 0.01){
      battery = 0.01;
  }
  if(battery > 1){
      battery = 1;
  }

  // uint8_t px = battery * 25;
  sprintf(buf, "%d%%", (int)(battery * 100));
  canvas.drawString(buf, 400, 900, 27);
  // canvas.fillRect(498 + 3, 8 + 10, px, 13, 15);
  // _bar->pushImage(498, 8, 32, 32, 2, ImageResource_status_bar_battery_charging_32x32);
}

//Set Time
int8_t global_timezone = 9;

bool SyncNTPTime(void){
    const char *ntpServer = "time.cloudflare.com";
    configTime(global_timezone * 3600, 0, ntpServer);

    struct tm timeInfo;
    if (getLocalTime(&timeInfo)){
        rtc_time_t time_struct;
        time_struct.hour = timeInfo.tm_hour;
        time_struct.min = timeInfo.tm_min;
        time_struct.sec = timeInfo.tm_sec;
        M5.RTC.setTime(&time_struct);
        rtc_date_t date_struct;
        date_struct.week = timeInfo.tm_wday;
        date_struct.mon = timeInfo.tm_mon + 1;
        date_struct.day = timeInfo.tm_mday;
        date_struct.year = timeInfo.tm_year + 1900;
        M5.RTC.setDate(&date_struct);
        return 1;
    }
    log_d("Time Sync failed");
    return 0;
}

void setup() {

  setlocale(LC_CTYPE, "");
  
  M5.begin();
  M5.RTC.begin();
  M5.SHT30.Begin();
  //M5.TP.SetRotation(90);
  M5.EPD.SetRotation(90);
  M5.EPD.Clear(true);

  canvas.loadFont("/ipaexg.ttf", SD);
  canvas.createCanvas(540, 960);
  canvas.createRender(34, 256);
  canvas.setTextSize(34);
  canvas.setTextColor(15);

  WiFi.begin("*ssid*", "*passwd*"); 
  while (WiFi.status() != WL_CONNECTED) { 
      delay(100); 
      Serial.print("."); 
  }

  Serial.println();
  Serial.print("WiFi connected: ");
  Serial.println(WiFi.localIP());

  SyncNTPTime();

}

void loop() {
  
  String payload;
  M5.EPD.Clear(true);

  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
 
    HTTPClient http;

    // http.begin( "http://www.nhk.or.jp/rss/news/cat0.xml" ); //Specify the URL and certificate
    http.begin( "http://www.japantimes.co.jp/feed" ); //Specify the URL and certificate
    int httpCode = http.GET(); //Make the request
 
    if (httpCode == HTTP_CODE_OK) { //Check for the returning code
      
      payload = http.getString();
      // Serial.println(httpCode);
      // Serial.println(payload);                
    }
    
    Serial.println("you have finished downloading");
    http.end(); //Free the resources
  }

  const char * testDocument = payload.c_str();
  std::string test = testDocument;
  std::string title_s = "<title>";
  std::string title_e = "</title>";
  std::string desc_s = "<description>";
  std::string desc_e = "</description>";

  std::vector<int> findVec1 = find_all(test, title_s);
  std::vector<int> findVec2 = find_all(test, title_e);
  std::vector<int> findVec3 = find_all(test, desc_s);
  std::vector<int> findVec4 = find_all(test, desc_e);

  std::size_t size = findVec1.size();

  for(int i = 0; i < size; i=i+1){

    M5.SHT30.UpdateData();
    tem = M5.SHT30.GetTemperature();
    hum = M5.SHT30.GetRelHumidity();
    Serial.printf("Temperatura: %2.2f*C  Humedad: %0.2f%%\r\n", tem, hum);
    dtostrf(tem, 2, 2 , temStr);
    dtostrf(hum, 2, 2 , humStr);
    
    //Serial.println(i);
    
    canvas.fillCanvas(0);

    std::string title = test.substr(findVec1[i]+7, findVec2[i]-findVec1[i]-7);
    std::string description = test.substr(findVec3[i]+13, findVec4[i]-findVec3[i]-13); 

    // std::size_t title_size = title.size();
    // std::size_t desc_size = description.size();
    std::size_t title_size = title.length();
    std::size_t desc_size = description.length();
    Serial.println(title_size);
    Serial.println(desc_size);

    unsigned int title_row = title_size / 3 / 14; //3byte文字換算
    unsigned int desc_row = desc_size / 3 / 13; //3byte文字換算

    Serial.println(title_row);
    Serial.println(desc_row);

    // canvas.drawString("NHK総合ニュース", 5, 10);
    canvas.drawString("JapanTimes TopNews", 5, 10);

    unsigned int position = 0;
    title_row = title_row + 1;
    desc_row = desc_row + 1;

    if(title_row>1){
      for(int i=0; i<title_row; i=i+1){
      position = 50*i+100;
      std::string title_sub = title.substr(3*14*i, 3*14);
      canvas.drawString(title_sub.c_str(), 5, position);
      }
    }else{
      position = 100;
      canvas.drawString(title.c_str(), 5, position);
    }
    // canvas.drawString(title.c_str(), 15, 100);
    if(desc_row>1){
      for(int i=0; i<desc_row; i=i+1){
      std::string desc_sub = description.substr(3*13*i, 3*13);
      canvas.drawString(desc_sub.c_str(), 25, position+50+50*i);
      }
    }else{
      canvas.drawString(description.c_str(), 25, position+50);
    }
    // canvas.drawString(description.c_str(), 15, 150);
    
    canvas.drawString("温度:" + String(temStr) + "℃", 10, 800);
    canvas.drawString("湿度:" + String(humStr) + "%", 10, 850);

    flushTime();
    flushBattery();
    canvas.pushCanvas(0,0,UPDATE_MODE_A2);

    M5.disableEPDPower();
    delay(500);
    M5.disableMainPower();

    sleep(900); //interval:15min
    M5.enableMainPower();
    delay(500);
    M5.enableEPDPower();
  }

  for (const auto &pos:findVec4) {
    Serial.println(pos);
  }

  // if(doc.Parse(testDocument)!= XML_SUCCESS){
  //   Serial.println("Error parsing");  
  //   return;
  // }else{
  //   Serial.println("Success parsing");
  // }

  // unsigned int num1 = test.find("<title>");
  // unsigned int num2 = test.find("</title>");
  //Serial.println(num1);
  //Serial.println(num2);
  
  // std::string top_title = test.substr(num1+7, num2-num1-7);
  // canvas.drawString(top_title.c_str(), 10, 550, 45);
  // canvas.pushCanvas(0,0,UPDATE_MODE_DU4);

  // sleep(3600);

}