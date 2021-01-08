//#include <Arduino.h>
#include <string.h>
#include <M5EPD.h>
#include <WiFi.h> 
#include "tinyxml2.h"

M5EPD_Canvas canvas(&M5.EPD);
using namespace tinyxml2;

void setup() {
  
  Serial.begin(115200);
  M5.begin();
  
  WiFi.begin("WIFI-SSID", "WIFI-PASSWORD"); 
  while (WiFi.status() != WL_CONNECTED) { 
      delay(500); 
      Serial.print("."); 
  }

  //XMLNode * root = xmlDocument.FirstChild();
  //XMLElement * element = root->FirstChildElement("element");

  //int val;
  //element->QueryIntText(&val);

  //Serial.println(val);

}

void loop() {
  
  M5.EPD.Clear(true);
  canvas.createCanvas(540, 960);
  canvas.setTextSize(3);

  XMLDocument doc;
  doc.LoadFile( "https://www.nhk.or.jp/rss/news/cat0.xml" );
  doc.Print();
/*
  if(doc.Parse(xmlData)!= XML_SUCCESS){
    Serial.println("Error parsing");  
    return;
  };
*/
  XMLNode * channel = doc.FirstChild();
  XMLElement * top_title = channel->FirstChildElement("title");
  std::string top_midashi = top_title->GetText();

  //XMLElement * item = channel.FirstChildElement("item");
  int pos_x = 50;
  int pos_y = 50;
  for(XMLElement * item = channel->FirstChildElement("item"); item!= NULL; item = item->NextSiblingElement("item")) {
    
    XMLElement * title = item->FirstChildElement("title");
    XMLElement * description = title->FirstChildElement("description");
    
    canvas.drawString(title->GetText(), pos_x, pos_y);  
    canvas.drawString(description->GetText(), pos_x, pos_y + 10);  
    pos_y = pos_y + 20;

    //std::string midashi = title->GetText();
    //std::string naiyou = description->GetText();  
  }

  sleep(3600);

}