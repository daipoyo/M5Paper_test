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
#include "esp_sleep.h"

const char* ca= \
"-----BEGIN CERTIFICATE-----\n" \
"MIIL3TCCCsWgAwIBAgIMb5wBeBwhgDIlSnMrMA0GCSqGSIb3DQEBCwUAMGIxCzAJ\n" \
"BgNVBAYTAkJFMRkwFwYDVQQKExBHbG9iYWxTaWduIG52LXNhMTgwNgYDVQQDEy9H\n" \
"bG9iYWxTaWduIEV4dGVuZGVkIFZhbGlkYXRpb24gQ0EgLSBTSEEyNTYgLSBHMzAe\n" \
"Fw0yMDA5MDIxNTM4MDZaFw0yMTEwMDQxNTM4MDZaMIIBBjEdMBsGA1UEDwwUUHJp\n" \
"dmF0ZSBPcmdhbml6YXRpb24xDzANBgNVBAUTBjU3ODYxMTETMBEGCysGAQQBgjc8\n" \
"AgEDEwJVUzEeMBwGCysGAQQBgjc8AgECEw1OZXcgSGFtcHNoaXJlMQswCQYDVQQG\n" \
"EwJVUzEWMBQGA1UECBMNTmV3IEhhbXBzaGlyZTETMBEGA1UEBxMKUG9ydHNtb3V0\n" \
"aDEpMCcGA1UECRMgMiBJbnRlcm5hdGlvbmFsIERyaXZlLCBTdWl0ZSAxNTAxHTAb\n" \
"BgNVBAoTFEdNTyBHbG9iYWxTaWduLCBJbmMuMRswGQYDVQQDExJ3d3cuZ2xvYmFs\n" \
"c2lnbi5jb20wggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDhvTs7gJj+\n" \
"myK4eLHq5Kt95hHCNG2BfLBnMnBZtkJNIX0RNuBUT9aJQTtJk1B4jgLI0x7Jr3N6\n" \
"wRcD1lnTjgb++lxBdJHD2lpenfqU39NrC13URyuo2nyas3c+o195YZs6RVexoU+5\n" \
"K/YKkqiJ6A4CNzLNcR3x3R0uN5lWsFHhHeVXjvVSH53F8FCFu2h6++7YDIYzt3MB\n" \
"IiwFu8B5MQfPxGTDFv/Joa8wPy4fjfqnvxa96jnWoUHALVAgi2QLSldrt9WcypX1\n" \
"uylwGTrgltV0JeBa+s8UVr001NoJ760kenTpnfGV172xOlIx04MnZ3iqYT1GBlGF\n" \
"utGnu8EApjyXAgMBAAGjggfrMIIH5zAOBgNVHQ8BAf8EBAMCBaAwgZYGCCsGAQUF\n" \
"BwEBBIGJMIGGMEcGCCsGAQUFBzAChjtodHRwOi8vc2VjdXJlLmdsb2JhbHNpZ24u\n" \
"Y29tL2NhY2VydC9nc2V4dGVuZHZhbHNoYTJnM3IzLmNydDA7BggrBgEFBQcwAYYv\n" \
"aHR0cDovL29jc3AyLmdsb2JhbHNpZ24uY29tL2dzZXh0ZW5kdmFsc2hhMmczcjMw\n" \
"VQYDVR0gBE4wTDBBBgkrBgEEAaAyAQEwNDAyBggrBgEFBQcCARYmaHR0cHM6Ly93\n" \
"d3cuZ2xvYmFsc2lnbi5jb20vcmVwb3NpdG9yeS8wBwYFZ4EMAQEwCQYDVR0TBAIw\n" \
"ADBFBgNVHR8EPjA8MDqgOKA2hjRodHRwOi8vY3JsLmdsb2JhbHNpZ24uY29tL2dz\n" \
"L2dzZXh0ZW5kdmFsc2hhMmczcjMuY3JsMIIFKQYDVR0RBIIFIDCCBRyCEnd3dy5n\n" \
"bG9iYWxzaWduLmNvbYINZ2xvYmFsc2lnbi5iZYINZ2xvYmFsc2lnbi5jaIIQZ2xv\n" \
"YmFsc2lnbi5jby51a4IRZ2xvYmFsc2lnbi5jb20uYXWCEWdsb2JhbHNpZ24uY29t\n" \
"LmhrghFnbG9iYWxzaWduLmNvbS5zZ4INZ2xvYmFsc2lnbi5lc4INZ2xvYmFsc2ln\n" \
"bi5ldYINZ2xvYmFsc2lnbi5mcoIOZ2xvYmFsc2lnbi5uZXSCDWdsb2JhbHNpZ24u\n" \
"bmyCFXNlY3VyZS5nbG9iYWxzaWduLm5ldIIRd3d3Lmdsb2JhbHNpZ24uYmWCEXd3\n" \
"dy5nbG9iYWxzaWduLmNoghR3d3cuZ2xvYmFsc2lnbi5jby51a4IVd3d3Lmdsb2Jh\n" \
"bHNpZ24uY29tLmF1ghV3d3cuZ2xvYmFsc2lnbi5jb20uaGuCFXd3dy5nbG9iYWxz\n" \
"aWduLmNvbS5zZ4IRd3d3Lmdsb2JhbHNpZ24uZXOCEXd3dy5nbG9iYWxzaWduLmV1\n" \
"ghF3d3cuZ2xvYmFsc2lnbi5mcoISd3d3Lmdsb2JhbHNpZ24ubmV0ghF3d3cuZ2xv\n" \
"YmFsc2lnbi5ubIIiY2VydGlmaWVkLXRpbWVzdGFtcC5nbG9iYWxzaWduLmNvbYIV\n" \
"Y2xpZW50Lmdsb2JhbHNpZ24uY29tghFjbi5nbG9iYWxzaWduLmNvbYIbY3RsMS5l\n" \
"cGtpcHJvLmdsb2JhbHNpZ24uY29tghdjdGwxLmhjcy5nbG9iYWxzaWduLmNvbYIa\n" \
"Y3RsMS5zeXN0ZW0uZ2xvYmFsc2lnbi5jb22CF2N0bDIuaGNzLmdsb2JhbHNpZ24u\n" \
"Y29tghpjdGwyLnN5c3RlbS5nbG9iYWxzaWduLmNvbYIYZG93bmxvYWRzLmdsb2Jh\n" \
"bHNpZ24uY29tghVlLXNpZ24uZ2xvYmFsc2lnbi5jb22CEmVkaS5nbG9iYWxzaWdu\n" \
"LmNvbYIWZXBraXByby5nbG9iYWxzaWduLmNvbYISaGNzLmdsb2JhbHNpZ24uY29t\n" \
"ghFoay5nbG9iYWxzaWduLmNvbYITaW5mby5nbG9iYWxzaWduLmNvbYIRanAuZ2xv\n" \
"YmFsc2lnbi5jb22CFG9jbmdzLmdsb2JhbHNpZ24uY29tghhvcGVyYXRpb24uZ2xv\n" \
"YmFsc2lnbi5jb22CFnBhcnRuZXIuZ2xvYmFsc2lnbi5jb22CFnByb2ZpbGUuZ2xv\n" \
"YmFsc2lnbi5jb22CFXJlZ2lzdC5nbG9iYWxzaWduLmNvbYIgcmZjMzE2MS10aW1l\n" \
"c3RhbXAuZ2xvYmFsc2lnbi5jb22CH3JmYzMxNjF0aW1lc3RhbXAuZ2xvYmFsc2ln\n" \
"bi5jb22CE3NlYWwuZ2xvYmFsc2lnbi5jb22CFXNlY3VyZS5nbG9iYWxzaWduLmNv\n" \
"bYITc2hvcC5nbG9iYWxzaWduLmNvbYIVc29jaWFsLmdsb2JhbHNpZ24uY29tghRz\n" \
"c2lmMS5nbG9iYWxzaWduLmNvbYIXc3NsY2hlY2suZ2xvYmFsc2lnbi5jb22CFnN0\n" \
"YXRpYzEuZ2xvYmFsc2lnbi5jb22CFnN0YXRpYzIuZ2xvYmFsc2lnbi5jb22CFXN0\n" \
"YXR1cy5nbG9iYWxzaWduLmNvbYIWc3VwcG9ydC5nbG9iYWxzaWduLmNvbYIVc3lz\n" \
"dGVtLmdsb2JhbHNpZ24uY29tghF0aC5nbG9iYWxzaWduLmNvbYIOZ2xvYmFsc2ln\n" \
"bi5jb20wHQYDVR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMB8GA1UdIwQYMBaA\n" \
"FN2z522oLujFTm7PdOZ1PJQVzugdMB0GA1UdDgQWBBSgOHv0jCH3JdmJRT1GPHUm\n" \
"/YbYmjCCAQUGCisGAQQB1nkCBAIEgfYEgfMA8QB3AFzcQ5L+5qtFRLFemtRW5hA3\n" \
"+9X6R9yhc5SyXub2xw7KAAABdE934ncAAAQDAEgwRgIhAJdVkn+p4Vrdj3D9hIOJ\n" \
"PDpXuxHUTzpYc/BxjLqNcKpxAiEAjlsIW2P9AFvcBqfP1dBr786hV/CiRdSyKMZC\n" \
"SBNcjk8AdgD2XJQv0XcwIhRUGAgwlFaO400TGTO/3wwvIAvMTvFk4wAAAXRPd+Bk\n" \
"AAAEAwBHMEUCIQD9NYOUtCekrFwmGCVGNYzgxWWWRAy2QKsOfHdEjeKr8wIgM7po\n" \
"kojeuRXEMP8bnm1A7S57j1/2Wcov1U871fX4wUowDQYJKoZIhvcNAQELBQADggEB\n" \
"AENL058pRO/HflqUeXSX4aZvqf9mx6mQi/ff5WA1BRvUg6BFNr2hXc7XpAz6vV7U\n" \
"DYcxZFw6C6lvOV0tQfdMWGG+j+jhT7MWQigHqtXTBw6ddTEuzu/6p3mY3kZLX1KZ\n" \
"hTYatwBg/mcSlRzHGaU3c/qn4HCKa9na2QaFOWEffGh51jUlm2/8uRcFUuLS+kcX\n" \
"qPCnVC2eHELqtjLEZCW+qNNkxT/8LRLYmgYDTGYAGWaILIfX25m3CEtABfzvcSF1\n" \
"dYjAOknKCA552jYfC0FCIUZjx1K52SEXQCviQpZ/tJToBCwTxzUhiN3rum+FtKuL\n" \
"uhB087agCDUo8J7k5jRtrNM=\n" \
"-----END CERTIFICATE-----\n";

M5EPD_Canvas canvas(&M5.EPD);
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
    // const char *ntpServer = "time.cloudflare.com";
    const char *ntpServer = "ntp.nict.jp";
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
  canvas.createRender(32, 32);
  canvas.setTextSize(32);
  canvas.setTextColor(15);

  WiFi.begin("dphone", "b555b555"); 
  while (WiFi.status() != WL_CONNECTED) { 
      delay(100); 
      Serial.print("."); 
  }

  Serial.println();
  Serial.print("WiFi connected: ");
  Serial.println(WiFi.localIP());

  SyncNTPTime();
}

//Global
uint8_t kiji = 0;
String payload;

void loop() {
  
  M5.EPD.Clear(true);

  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
    HTTPClient http;
    http.begin( "http://www.sankeibiz.jp/rss/news/flash.xml" ); //Specify the URL and certificate
    // http.begin( "https://www.sankeibiz.jp/rss/news/compliance.xml", ca ); //Specify the URL and certificate
    int httpCode = http.GET(); //Make the request
 
    if (httpCode == HTTP_CODE_OK) { //Check for the returning code
      payload = http.getString();
      // Serial.println(httpCode);
      // Serial.println(payload);                
    }
    
    Serial.println("you have finished downloading");
    // Serial.println(payload);
    http.end(); //Free the resources
    WiFi.mode(WIFI_OFF);
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

  std::size_t size = findVec3.size(); //descriptionのあるところまで
  Serial.printf("size = %02d",size);

  if(kiji > 0){
    M5.enableEPDPower();
  }

  M5.SHT30.UpdateData();
  tem = M5.SHT30.GetTemperature();
  hum = M5.SHT30.GetRelHumidity();
  // Serial.printf("Temperatura: %2.2f*C  Humedad: %0.2f%%\r\n", tem, hum);
  dtostrf(tem, 2, 2 , temStr);
  dtostrf(hum, 2, 2 , humStr);
  
  //Serial.println(i);
  
  canvas.fillCanvas(0);
  std::string title = test.substr(findVec1[kiji]+7, findVec2[kiji]-findVec1[kiji]-7);
  std::string description = test.substr(findVec3[kiji]+13, findVec4[kiji]-findVec3[kiji]-13); 
  std::size_t title_size = title.length();
  std::size_t desc_size = description.length();
  // Serial.println(title_size);
  // Serial.println(desc_size);
  unsigned int title_row = title_size / 3 / 16; //3byte文字換算
  unsigned int desc_row = desc_size / 3 / 15; //3byte文字換算
  // Serial.println(title_row);
  // Serial.println(desc_row);
  canvas.drawString("SankeiBiz ニュース速報", 5, 10);
  unsigned int position = 0;
  title_row = title_row + 1;
  desc_row = desc_row + 1;

  if(desc_row > 12){
    if(title_row < 3){
      desc_row = 12;
    }else{
      desc_row = 11;
    }
  }

  if(title_row>1){
    for(int count=0; count < title_row; count=count+1){
      position = 50*count+100;
      std::string title_sub = title.substr(3*16*count, 3*16);
      canvas.drawString(title_sub.c_str(), 5, position);
    }
  }else{
    position = 100;
    canvas.drawString(title.c_str(), 5, position);
  }
  // canvas.drawString(title.c_str(), 15, 100);
  if(desc_row>1){
    for(int count=0; count < desc_row; count=count+1){
      std::string desc_sub = description.substr(3*15*count, 3*15);
      canvas.drawString(desc_sub.c_str(), 25, position+50+50*count);
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
  delay(1000);
  // M5.disableEPDPower();
  // delay(500);
  // M5.disableMainPower();
  if(kiji == 0){
    sleep(10);
    kiji = kiji + 1;
  }else if(0 < kiji && kiji <= size){
    esp_sleep_enable_timer_wakeup(600 * 1000 * 1000); //600 seconds
    int err = esp_light_sleep_start();
    Serial.println(err);
    kiji = kiji + 1;
  }else{
    esp_sleep_enable_timer_wakeup(600 * 1000 * 1000); //600 seconds
    esp_light_sleep_start();
    esp_restart();
  }
  // M5.enableMainPower();
  // delay(500);
  // M5.enableEPDPower();
}