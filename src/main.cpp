#include <Arduino.h>
#include <iostream>
#include <string.h>
#include <M5EPD.h>
#include <WiFi.h> 
#include "FS.h"
#include "SPIFFS.h"
#include "tinyxml2.h"

M5EPD_Canvas canvas(&M5.EPD);
using namespace tinyxml2;
XMLDocument doc;

char * testDocument = "<root><element>NHKニュース</element><element>BBCニュース</element><element>もうやめたい</element></root>";

void setup() {
  
  M5.begin();
  M5.TP.SetRotation(90);
  M5.EPD.SetRotation(90);
  M5.EPD.Clear(true);

  canvas.loadFont("/ipaexg.ttf", SD);
  canvas.createCanvas(540, 960);
  canvas.createRender(42, 256);
  canvas.setTextSize(42);
  canvas.setTextColor(15);

  WiFi.begin("AirMac", "hira0698"); 
  while (WiFi.status() != WL_CONNECTED) { 
      delay(100); 
      Serial.print("."); 
  }

  Serial.println();
  Serial.print("WiFi connected: ");
  Serial.println(WiFi.localIP());

  // if(doc.LoadFile( "http://www.nhk.or.jp/rss/news/cat0.xml" ) == 0){
  //   Serial.print("LoadXML Completed.");
  // }else{
  //   Serial.print("LoadXML Error.");
  // }
  // if (!SPIFFS.begin(true)){
  //   log_e("SPIFFS Mount Failed");
  //   while(1);
  // }

}

void loop() {
  
  M5.EPD.Clear(true);

  // if(SD.exists("/ipaexg.ttf")) { 
  //   canvas.loadFont("/ipaexg", SD); 
  // }

  //canvas.drawString("NHKニュース", 45, 450);
  //canvas.pushCanvas(0,0,UPDATE_MODE_DU4);
  
  Serial.println(doc.LoadFile("/sd/cat0.xml"));
  // doc.LoadFile( "https://www.nhk.or.jp/rss/news/cat0.xml" );
  // doc.Print();

  if(doc.Parse(testDocument)!= XML_SUCCESS){
    Serial.println("Error parsing");  
    return;
  };

  //doc.LoadFile("/sd/cat0.xml");
  //doc.Print();

  XMLNode * channel = doc.FirstChild();
  XMLElement * title = channel->FirstChildElement("element");
  XMLElement * title2 = title->NextSiblingElement("element");
  //const char * top_title_text = title->GetText();
  canvas.drawString(title->GetText(), 10, 450);
  canvas.drawString(title2->GetText(), 10, 550);
  //canvas.drawString(title->GetText(), 10, 550, 45);
  canvas.pushCanvas(0,0,UPDATE_MODE_DU4);

  // XMLElement * title = channel->FirstChildElement("title");
  int pos_x = 50;
  int pos_y = 50;
 
  for(XMLElement * item = channel->FirstChildElement("title"); item!= NULL; item = item->NextSiblingElement("item")) {
    Serial.print(".");
    XMLElement * title = item->FirstChildElement("title");
    XMLElement * description = title->FirstChildElement("description");
    
    canvas.drawString(title->GetText(), pos_x, pos_y);  
    canvas.drawString(description->GetText(), pos_x, pos_y + 10);
    canvas.pushCanvas(0,0,UPDATE_MODE_DU4); 
    pos_y = pos_y + 20;

    //std::string midashi = title->GetText();
    //std::string naiyou = description->GetText();  
  }

  sleep(3600);

}