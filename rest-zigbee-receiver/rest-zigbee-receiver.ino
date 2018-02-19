#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "itim";
const char *password = "ilovecpe";

uint8_t headerPkt[11];
uint8_t deviceRegisters[32];

bool isNewPkt = false;
bool isHeaderPkt = true;
int counterPkt = 0;
int pkt_size = 32;
uint8_t bytePkt;

void setup()
{
    Serial.begin(115200);
    delay(10);

    WiFi.begin(ssid, password);
    WiFi.waitForConnectResult();
}

void loop()
{
    receiveReportPkt();
}

void receiveReportPkt()
{
    while (Serial.available() > 0)
    {
        if (isHeaderPkt == true) // ** need to check start byte to read packet
        {
            bytePkt = Serial.read();
            headerPkt[counterPkt] = bytePkt;
            // Serial.println(bytePkt);

            if (counterPkt >= 10)
            {
                isHeaderPkt = false;
                counterPkt = 0;
            }
            else
            {
                counterPkt++;
            }
        }
        else
        {
            // ** static size need to be solve when develop processes
            deviceRegisters[counterPkt] = Serial.read();
            counterPkt++;
        }
        isNewPkt = true;
    }
    ReportPkt_Process();
    counterPkt = 0;
    isHeaderPkt = true;
}

void ReportPkt_Process()
{
    if (isNewPkt)
    {
        if ((uint)deviceRegisters[1] == 2) // post method
        {
            char payload[(int)deviceRegisters[2]];

            for (int i = 0; i < (int)deviceRegisters[2]; i++)
            {
                payload[i] = deviceRegisters[i + 3];
                // Serial.println(payload[i]);
            }
            // Serial.println(payload);
            restful_post(payload);
        }
        isNewPkt = false;
        // Serial.print(isNewPkt);
    }
}

void restful_post(char *payload)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;

        http.begin("https://data.learninginventions.org/update?key=83JY45SPBM64U4TA"); //Specify destination for HTTP request
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");           //Specify content-type header

        int httpResponseCode = http.POST(payload); //Send the actual POST request

        if (httpResponseCode > 0)
        {
            // network success handle
        }
        else
        {
            // network error handle
        }
        http.end(); //Free resources
    }
}
