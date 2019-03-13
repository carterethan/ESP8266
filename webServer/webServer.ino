#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/*
 * Web server sketch for ESP8266
 * Creates a web server over an existing WiFi network or a new AP.
 * Author: Ethan Carter
 */
 
//Change these to the ssid and password of your network
const char* ssid = "Housing-WiFi";
const char* password = "";
//Change these to the ssid and password for use in WiFi AP mode
const char* ssidAP = "ESP8266";
const char* passwordAP = "";

ESP8266WebServer server(80);

void setup() {
  serialConfig(74880);
  initWiFi();
  initServer();
}

void loop() {
  server.handleClient();
}

void serialConfig(int baud){
  Serial.begin(baud);
  Serial.println();
}

void initWiFi(){
  Serial.print("MAC Address: ");
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
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssidAP, passwordAP);
  Serial.println("WiFi AP Running");
  Serial.print("AP Name:");
  Serial.println(ssidAP);
}

void initServer(){
  server.on("/", handleRoot);
  server.begin();
}

void handleRoot() {
  String html = "<!DOCTYPE html> <html> <body>";
  html+= "<h1>The server is working.</h1>";
  html+= "</body> </html>";
  server.send(200, "text/html", html);
}
