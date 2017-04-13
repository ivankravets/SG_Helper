#ifndef SG_Helper_h
#define SG_Helper_h

#include "Arduino.h"

class SG_Helper
{
  public:
    SG_Helper(String hostname);
    void setupWiFi(const char* ssid, const char* password, IPAddress ip, IPAddress gateway, IPAddress subnet, WiFiClient client);
    void setupOTA();
    String getHTMLresponse(HTTPMethod method, String uri);
    String getMethod(HTTPMethod method);
  private:
    String _hostname;
};

#endif
