#include "WString.h"
#ifndef _WIFIBASEDON_HPP_
# define _WIFIBASEDON_HPP_

# include <WiFi.h>
# include "Settings.hpp"

class WiFiBasedOn {
  public:
    uint8_t begin(){return this->begin(settings.ssid,settings.password);};
    uint8_t begin(const char *ssid, const char *password);
    uint8_t softAP(){return softAP(settings.host_ssid,settings.host_password);};
    uint8_t softAP(const char *ssid, const char *password);
    char *client_ssid;
    char *client_password;
    char *ap_ssid;
    char *ap_password;
  private:
    Settings settings;
};

#endif