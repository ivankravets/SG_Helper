#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <aJSON.h>
#include <SG_Helper.h>

const char* ssid = "...";
const char* password = "...";
IPAddress ip(192,168,30,161);  //Node static IP
IPAddress gateway(192,168,30,1);
IPAddress subnet(255,255,255,0);
ESP8266WebServer server(80);
WiFiClient client;
SG_Helper t("Test_Hostname");
int lastWebsocketTime = 0;

void setup() {
  Serial.begin(115200);
  t.setupWiFi(ssid, password, ip, gateway, subnet, client);
  // initializes OTA (over the air) update
  t.setupOTA();
  server.on("/", [](){
    server.send(200, "text/html", t.getHTMLresponse(server.method(), server.uri()));
  });
  server.onNotFound([](){
    server.send(200, "text/html", t.getHTMLresponse(server.method(), server.uri()));
  });
  server.on("/json", [](){
    server.send(200, "text/html", getJSON(server.method(), server.uri()));
  });
  server.begin();
  Serial.println("Web server running.");
}

void loop() {
  // waiting for new OTA
  ArduinoOTA.handle();

  // handle Webserver
  server.handleClient();

  if(lastWebsocketTime == 0 || (millis()-lastWebsocketTime)/1000 >= 3)
  {
    // put your main code here, to run repeatedly:
    HTTPClient http;

    //Now let is try a post request.
    http.begin("http://192.168.30.13:1880/test");
    aJsonObject *root;
    root=aJson.createObject();
    aJson.addStringToObject(root,"message", "Hello from ESP8266");
    char *payload=aJson.print(root);

    int httpCode = http.POST((uint8_t *)payload,strlen(payload));
    if(httpCode == HTTP_CODE_OK)
    {
        Serial.print("HTTP response code ");
        Serial.println(httpCode);
        String response = http.getString();
        Serial.print("HTTP response body ");
        Serial.println(response);
    }
    else
    {
      Serial.println("Error in HTTP request");
    }

    http.end();
    lastWebsocketTime = millis();
  }
}

String getJSON(HTTPMethod method, String uri){
  String root = "";
  root.concat("{\"method\":\"");
  root.concat(t.getMethod(method));
  root.concat("\",\"uri\":\"");
  root.concat(uri);
  root.concat("\"}");
  //String ret = new String(charpayload,charstrlen(payload))
  return root;
}
