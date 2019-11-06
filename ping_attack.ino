#include <ESP8266WiFi.h>
#include <Pinger.h>

/*
 * MAC Address spoofing sketch for ESP8266
 * Author: Ethan Carter
 * Uses resources found in the Pinger library created by Alessio Leoncini
 */

//Change these to the ssid and password of your network
const char* ssid = "";
const char* password = "";
/* Change these to the ssid and password for use in WiFi AP mode
const char* ssidAP = "ssid";
const char* passwordAP = "";
*/

bool finished_setup;
Pinger pinger;

//Change this to desired MAC Address
uint8_t mac[6] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


extern "C" {
  #include <user_interface.h>
  #include <lwip/icmp.h>
}

void setup() {
  serialConfig(74880);
  initWiFi();
  pinger.OnReceive([](const PingerResponse& response)
  {
    if (response.ReceivedResponse)
    {
      Serial.printf(
        "Reply from %s: bytes=%d time=%lums TTL=%d\n",
        response.DestIPAddress.toString().c_str(),
        response.EchoMessageSize - sizeof(struct icmp_echo_hdr),
        response.ResponseTime,
        response.TimeToLive);
    }
    else
    {
      Serial.printf("Request timed out.\n");
    }

    // Return true to continue the ping sequence.
    // If current event returns false, the ping sequence is interrupted.
    return true;
  });

  pinger.OnEnd([](const PingerResponse& response)
  {
    // Evaluate lost packet percentage
    float loss = 100;
    if(response.TotalReceivedResponses > 0)
    {
      loss = (response.TotalSentRequests - response.TotalReceivedResponses) * 100 / response.TotalSentRequests;
    }

    // Print packet trip data
    Serial.printf(
      "Ping statistics for %s:\n",
      response.DestIPAddress.toString().c_str());
    Serial.printf(
      "    Packets: Sent = %lu, Received = %lu, Lost = %lu (%.2f%% loss),\n",
      response.TotalSentRequests,
      response.TotalReceivedResponses,
      response.TotalSentRequests - response.TotalReceivedResponses,
      loss);

    // Print time information
    if(response.TotalReceivedResponses > 0)
    {
      Serial.printf("Approximate round trip times in milli-seconds:\n");
      Serial.printf(
        "    Minimum = %lums, Maximum = %lums, Average = %.2fms\n",
        response.MinResponseTime,
        response.MaxResponseTime,
        response.AvgResponseTime);
    }

    // Print host data
    Serial.printf("Destination host data:\n");
    Serial.printf(
      "    IP address: %s\n",
      response.DestIPAddress.toString().c_str());
    if(response.DestMacAddress != nullptr)
    {
      Serial.printf(
        "    MAC address: " MACSTR "\n",
        MAC2STR(response.DestMacAddress->addr));
    }
    if(response.DestHostname != "")
    {
      Serial.printf(
        "    DNS name: %s\n",
        response.DestHostname.c_str());
    }

    return true;
  });

  /*
  // Ping default gateway
  Serial.printf(
    "\n\nPinging default gateway with IP %s\n",
    WiFi.gatewayIP().toString().c_str());
  if(pinger.Ping(WiFi.gatewayIP()) == false)
  {
    Serial.println("Error during last ping command.");
  }
  delay(10000);

  // Ping technologytourist.com
  Serial.printf("\n\nPinging technologytourist.com\n");
  if(pinger.Ping("technologytourist.com") == false)
  {
    Serial.println("Error during ping command.");
  }
  */

  finished_setup = true;
}

void loop() {
  // put your main code here, to run repeatedly:
  if(finished_setup){
    Serial.println("Beginning loop...");

    // Ping default gateway
    Serial.printf(
    "\n\nPinging default gateway with IP %s\n",
    WiFi.gatewayIP().toString().c_str());
    if(pinger.Ping(WiFi.gatewayIP()) == false){
      Serial.println("Error during last ping command.");
    }
    delay(10000);
    // Ping technologytourist.com
    Serial.printf("\n\nPinging technologytourist.com\n");
    if(pinger.Ping("technologytourist.com") == false){
      Serial.println("Error during ping command.");
    }
    delay(10000);
   }
}

void serialConfig(int baud){
  Serial.begin(baud);
  Serial.println();
}

void initWiFi(){
  Serial.println("ESP8266 MAC Address: " + WiFi.macAddress());
  delay(500);
  wifi_set_macaddr(0, const_cast<uint8*>(mac));
  Serial.println("New MAC Address: " + WiFi.macAddress());

  Serial.print("Attempting connection to network: ");
  Serial.println(ssid);
  WiFi.begin(ssid);
  //Show activity while connecting
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println();
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
