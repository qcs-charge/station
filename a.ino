#include <SoftwareSerial.h>

SoftwareSerial ESP8266(2, 3);

void setup()
{
  Serial.begin(9600);
  ESP8266.begin(9600);
}

void loop()
{
  if (ESP8266.available()) {
    char c = ESP8266.read();
    Serial.print(c);
  }

  if (Serial.available()) {
    char c = Serial.read();
    ESP8266.print(c);
  }
}
