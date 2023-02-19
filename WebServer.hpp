#ifndef _WEBSERVER_HPP_
# define _WEBSERVER_HPP_

# include <WebServer.h>

static WebServer server(80);

class WebServerClass {
  public:
    uint8_t begin();
};

#endif