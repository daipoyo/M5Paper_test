//#include <Arduino.h>
//#include <M5EPD.h>
#include "tinyxml2.h"

using namespace tinyxml2;

char * testDocument = "<root><element>7</element></root>";

void setup() {
  
  Serial.begin(115200);

  XMLDocument xmlDocument;

  if(xmlDocument.Parse(testDocument)!= XML_SUCCESS){
      Serial.println("Error parsing");  
      return;
  };

  XMLNode * root = xmlDocument.FirstChild();
  XMLElement * element = root->FirstChildElement("element");

  int val;
  element->QueryIntText(&val);

  Serial.println(val);

}

void loop() {
}