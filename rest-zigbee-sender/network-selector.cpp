#include "Arduino.h"
#include "network-selector.h"
#include <WiFi.h>
#include <HTTPClient.h>

extern uint8_t deviceRegisters[32];

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
    bool zigbeeFlag = 0;
    uint8_t pkt_size = (uint8_t)sizeof(deviceRegisters);
    uint8_t pkt_header[3] = {0x54, 0xfe, pkt_size};

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
        // Serial.println("ZigBee API ...");

        zigbeeFlag = 1;

        deviceRegisters[0] = 0x0c;
        deviceRegisters[1] = 0x02;
        deviceRegisters[2] = (uint8_t)strlen(payload);

        for (int i = 0; i < strlen(payload); i++)
        {
            deviceRegisters[i + 3] = (uint8_t)payload[i];
            // Serial.print(payload[i]);
            // Serial.print((uint8_t)test[i]);
        }

        // uint8_t report_pkt[sizeof(pkt_header) + sizeof(deviceRegisters)];
        // int pkt_counter = 0;

        // for (int i = 0; i < sizeof(pkt_header); i++) {
        //     report_pkt[i] = pkt_header[i];
        //     pkt_counter++;
        // }
        // for (int i = 0; i < sizeof(deviceRegisters); i++) {
        //     report_pkt[i+pkt_counter] = deviceRegisters[i];
        // }

        // Serial.write(report_pkt, sizeof(report_pkt));
        Serial.write(pkt_header, sizeof(pkt_header));
        Serial.write(deviceRegisters, sizeof(deviceRegisters));

        // if (zigbeeFlag)
        // {
        //     uint8_t emptyRegisters[32];

        //     Serial.write(pkt_header, sizeof(pkt_header));
        //     Serial.write(emptyRegisters, sizeof(emptyRegisters));

        //     zigbeeFlag = 0;
        // }

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
    }
    else
    {
        return "";
    }
}