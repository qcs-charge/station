#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include "WiFiClient.h"
#include "ESP8266WebServer.h"
#include "ESP8266HTTPClient.h"

#define PIN_OUTPUT BUILTIN_LED
#define PIN_INPUT A0

const String API_LINK = "http://qcs.pythonanywhere.com/api/station/?token=INSECURE-ynlht-js081-0z2b5-k02i6";

HTTPClient http;
WiFiClient client;

void setup() {
  Serial.begin(115200);

  pinMode(PIN_INPUT, INPUT_PULLUP);
  pinMode(PIN_OUTPUT, OUTPUT);
  digitalWrite(PIN_OUTPUT, LOW);

  WiFiManager wifiManager;
  wifiManager.setDebugOutput(false);
  wifiManager.autoConnect("qcs wifi", "12345678");

  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  http.begin(client, API_LINK + "&request=getStatus");

  int http_code = http.GET();
  String payload = http.getString();

  http.end();

  if (http_code == 200 && (payload == "00" || payload == "10")) {
    digitalWrite(PIN_OUTPUT, HIGH);

    while (analogRead(A0) < 300) {
      Serial.println(analogRead(A0));
      delay(1);
    }
    
    digitalWrite(PIN_OUTPUT, LOW);
    
    http_code = -1;
    String editPayload = "-1";
    while (http_code != 200 && editPayload != "1") {
      http.begin(client, API_LINK + "&request=setStatus&status=" + payload[0] + "1");
      http_code = http.GET();
      editPayload = http.getString();
      http.end();
    }
  }
}
