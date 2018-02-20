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
uint8_t firstBytePkt;
uint8_t secondBytePkt;

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
    if (Serial.available()) // incoming serial detect
    {
        firstBytePkt = Serial.read();
        secondBytePkt = Serial.read();
        if (firstBytePkt == (int)0x54 && secondBytePkt == (int)0xfe) // check first byte of packet
        {
            // Serial.println("header packet byte detect !!");
            headerPkt[counterPkt] == firstBytePkt; // put first byte to registers
            headerPkt[counterPkt] == secondBytePkt; // put second byte to registers
            counterPkt = 2; // set counter to skip first two byte already received

            while (Serial.available() > 0) // loop to receive all the packet
            {
                if (isHeaderPkt == true)
                {
                    bytePkt = Serial.read();
                    headerPkt[counterPkt] = bytePkt;
                    // Serial.print(counterPkt);
                    // Serial.print(" : ");
                    // Serial.println(bytePkt, HEX);

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
                    // Serial.print(counterPkt);
                    // Serial.print(" : ");
                    // Serial.println(deviceRegisters[counterPkt], HEX);
                    counterPkt++;
                }
            }
            // reset all flags
            isNewPkt = true;
            counterPkt = 0;
            isHeaderPkt = true;

            ReportPkt_Process();
        }
        else // flush all the serial buffer
        {
            while (Serial.available())
            {
                Serial.read();
            }
            // Serial.end();
            // Serial.begin(115200);
        }
    }
}

void ReportPkt_Process()
{
    if (isNewPkt)
    {
        if (deviceRegisters[1] == (int)0x02) // post method
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

void restful_post(char *body)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;

        http.begin("https://data.learninginventions.org/update?key=83JY45SPBM64U4TA"); //Specify destination for HTTP request
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");           //Specify content-type header

        int httpResponseCode = http.POST(body); //Send the actual POST request

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
