/*
    This sketch sends data via HTTP GET requests to data.sparkfun.com service.

    You need to get streamId and privateKey at data.sparkfun.com and paste them
    below. Or just customize this script to talk to other HTTP servers.

*/

#include <WiFi.h>
// #include <HTTPClient.h>

const char *ssid = "momeLaptop";
const char *password = "Memoriedz3280";
uint8_t deviceRegisters[32];

#include "network-selector.h"

NetworkSelector networkMome("momeLaptop", "Memoriedz3280");

void setup()
{
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  // Serial.println();
  // Serial.println();
  // Serial.print("Connecting to ");
  // Serial.println(ssid);

  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();

  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   // Serial.print(".");
  // }

  // Serial.println("");
  // Serial.println("WiFi connected");
  // Serial.println("IP address: ");
  // Serial.println(WiFi.localIP());

  // char *test = "field1=100";

  // Serial.println(strlen(test));

  // for (int i=0; i<strlen(test); i++) {
  //   Serial.print(test[i]);
  //   Serial.print((uint8_t)test[i]);
  // }
}

void loop()
{
  // Serial.println(networkMome.status());

  int resCode = networkMome.post("https://data.learninginventions.org/update?key=83JY45SPBM64U4TA", "field1=100");

  // Serial.println(resCode);
  // Serial.println(networkMome.getResponseBody());

  delay(10000); //Send a request every 10 seconds
}
