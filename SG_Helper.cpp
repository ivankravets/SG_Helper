#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
#include "SG_Helper.h"

SG_Helper::SG_Helper(String hostname)
{
  String _hostname = hostname;
}

void SG_Helper::setupWiFi(const char* ssid, const char* password, IPAddress ip, IPAddress gateway, IPAddress subnet, WiFiClient client)
{
  // Connecting to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  //WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);

  // wait till Wifi ok
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("WiFi connected with SSID: ");
  Serial.println(ssid);
  Serial.print("IP Address is: ");
  Serial.println(WiFi.localIP());
}

// ota setup
void SG_Helper::setupOTA() {
  Serial.println("Booting");
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
}

// sent standard HTTM response
String SG_Helper::getHTMLresponse(HTTPMethod method, String uri) {
  String output = "";
  output.concat("<h1>It is running:</h1>");
  output.concat("<h3> Basis Swissglider Webserver on Arduino ESP8266 Boards like D1 / NodeMCU etc.</h3>");
  output.concat("The Method is: ");
  output.concat(getMethod(method));
  output.concat("<br>");
  output.concat("The URI is: ");
  output.concat(uri);
  output.concat("<br>");
  return output;
}

String SG_Helper::getMethod(HTTPMethod method){
  switch(method) {
    case 0 : return "any";
    case 1 : return "get";
    case 2 : return "post";
    case 3 : return "put";
    case 4 : return "patch";
    case 5 : return "delete";
    case 6 : return "options";
    default : return "unknown";
  }
}
