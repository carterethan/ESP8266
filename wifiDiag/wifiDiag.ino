#include <ESP8266WiFi.h>
/*
 * WiFi diagnostic sketch for ESP8266
 * Author: Ethan Carter
 */
 
//Change these to the ssid and password of your network
const char* ssid = "ssid";
const char* password = "";
//Change these to the ssid and password for use in WiFi AP mode
const char* ssidAP = "ssid";
const char* passwordAP = "";

void setup() {
  serialConfig(74880);
  initWiFi();
}

void loop() {
  // put your main code here, to run repeatedly:
}

void serialConfig(int baud){
  Serial.begin(baud);
  Serial.println();
}

void initWiFi(){
  Serial.print("ESP8266 MAC Address: ");
  Serial.println(WiFi.macAddress());
  Serial.print("Attempting connection to network: ");
  Serial.println(ssid);
  WiFi.begin(ssid);
  //Show activity while connecting
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void initAP(){
  WiFiServer server(80);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  server.begin();
  Serial.println(WiFi.softAPIP());
}
