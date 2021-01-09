//#include <Arduino.h>
#include <string.h>
#include <M5EPD.h>
#include <WiFi.h> 
#include "FS.h"
#include "SPIFFS.h"
#include "tinyxml2.h"

M5EPD_Canvas canvas(&M5.EPD);
using namespace tinyxml2;

void setup() {

  M5.begin();
  M5.TP.SetRotation(90);
  M5.EPD.SetRotation(90);
  M5.EPD.Clear(true);

  canvas.loadFont("/ipaexg.ttf", SD); 
  
  WiFi.begin("********", "********"); 
  while (WiFi.status() != WL_CONNECTED) { 
      delay(100); 
      Serial.print("."); 
  }

  Serial.println();
  Serial.print("WiFi connected: ");
  Serial.println(WiFi.localIP());

  // if (!SPIFFS.begin(true)){
  //   log_e("SPIFFS Mount Failed");
  //   while(1);
  // }

  //XMLNode * root = xmlDocument.FirstChild();
  //XMLElement * element = root->FirstChildElement("element");

  //int val;
  //element->QueryIntText(&val);

  //Serial.println(val);

}

void loop() {
  
  M5.begin();  
  M5.EPD.Clear(true);

  // if(SD.exists("/ipaexg.ttf")) { 
  //   canvas.loadFont("/ipaexg", SD); 
  // }

  canvas.createCanvas(540, 960);
  //canvas.setTextSize(32);
  canvas.createRender(42, 256);
  canvas.setTextSize(42);
  canvas.setTextColor(15);
  canvas.drawString("ブッシープレコの世界", 45, 450);
  canvas.pushCanvas(0,0,UPDATE_MODE_DU4);

//   XMLDocument doc;
//   doc.LoadFile( "https://www.nhk.or.jp/rss/news/cat0.xml" );
//   doc.Print();
// /*
//   if(doc.Parse(xmlData)!= XML_SUCCESS){
//     Serial.println("Error parsing");  
//     return;
//   };
// */
//   XMLNode * channel = doc.FirstChild();
//   XMLElement * top_title = channel->FirstChildElement("title");
//   std::string top_midashi = top_title->GetText();

//   //XMLElement * item = channel.FirstChildElement("item");
//   int pos_x = 50;
//   int pos_y = 50;
//   for(XMLElement * item = channel->FirstChildElement("item"); item!= NULL; item = item->NextSiblingElement("item")) {
    
//     XMLElement * title = item->FirstChildElement("title");
//     XMLElement * description = title->FirstChildElement("description");
    
//     canvas.drawString(title->GetText(), pos_x, pos_y);  
//     canvas.drawString(description->GetText(), pos_x, pos_y + 10);
//     canvas.pushCanvas(0,0,UPDATE_MODE_DU4); 
//     pos_y = pos_y + 20;

    //std::string midashi = title->GetText();
    //std::string naiyou = description->GetText();  
  // }

  sleep(600);

}