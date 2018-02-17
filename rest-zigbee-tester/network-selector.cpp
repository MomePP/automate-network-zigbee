#include "Arduino.h"
#include "network-selector.h"
#include <WiFi.h>
#include <HTTPClient.h>

NetworkSelector::NetworkSelector(char *ssid, char *pwd)
{
    _ssid = ssid;
    _pwd = pwd;
}

int NetworkSelector::status()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return 0;
    }
    else
    {
        // WiFi.begin(_ssid, _pwd);
        WiFi.reconnect();
        return 1;
    }
}

// NetworkSelector::get(char* url)
// {
// }

int NetworkSelector::post(char *url, char *payload)
{
    int responseCode = 0;

    if (WiFi.status() == WL_CONNECTED)
    {

        HTTPClient http;

        http.begin(url);                                                     //Specify destination for HTTP request
        http.addHeader("Content-Type", "application/x-www-form-urlencoded"); //Specify content-type header

        int httpResponseCode = http.POST(payload); //Send the actual POST request

        if (httpResponseCode > 0)
        {
            _res_payload = http.getString();
            _isNewPayload = 1;
        }
        else
        {
            // network error handle
        }

        responseCode = httpResponseCode;

        http.end(); //Free resources
    }
    else
    {
        // ZigBee API
        Serial.println("ZigBee API ...");
        WiFi.reconnect();
    }

    return responseCode;
}

String NetworkSelector::getResponseBody(void)
{
    if (_isNewPayload)
    {
        _isNewPayload = 0;
        return _res_payload;
    } else {
        return "";
    }
}