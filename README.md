# ESP8266

I personally use a Wemos D1 Mini, but generic ESP8266 modules should be compatible with these sketches.

## wifiDiag
This sketch is meant to help get your ESP8266 connected to the internet.

### initWiFi
Function within wifiDiag that creates a WiFi connection to your desired ssid and displays useful information such as local IP and MAC address.

### initAP
Function within wifiDiag that creates a WiFi access point set to your desired ssid and password.

## webServer
This sketch includes the methods from wifiDiag with the addition of a new method which starts a web server on local WiFi or the ESP8266 as an access point.
Note: you must use either initWiFi or initAP to join or create a network before running initServer.

### initServer
This function creates a web server on a previously joined or created network.

## ping_attack
This sketch allows a user to spoof a MAC address and continually create network traffic with that MAC address. This can be used to cause packet loss, interrupting the connection of the device to which the MAC address belongs to.
