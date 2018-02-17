#ifndef NetworkSelector_h
#define NetworkSelector_h

#include "Arduino.h"

class NetworkSelector
{
  public:
    NetworkSelector(char *ssid, char *pwd);
    int status();
    // get(char* url);
    int post(char *url, char *payload);
    String getResponseBody();
    // String post(char *url, char *body);
    // post(char* url, char* header[], char* body);
  private:
    char *_ssid;
    char *_pwd;
    String _res_payload;
    bool _isNewPayload = 0;
};

#endif