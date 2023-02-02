#include "ESP8266WiFi.h"
#include "WiFiClient.h"
#include "ESP8266WebServer.h"
#include "ESP8266HTTPClient.h"


#define WIFI_SSID "connecting..."
#define WIFI_PASS "81790487"
#define API_ERRORS 10
#define WIFI_ERRORS 100

const String API_LINK = "http://qcs.pythonanywhere.com/api/station/?token=INSECURE-ynlht-js081-0z2b5-";

HTTPClient http;
WiFiClient client;


void setup() {
  // Initialising
  Serial.begin(115200);
  Serial.println();

  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);
  WiFi.mode(WIFI_STA);

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.print("Connecting to \"");
  Serial.print(WIFI_SSID);
  Serial.print("\" ");

  // Connecting to WiFi
  int k_wifi_errors = 0;
  while (WiFi.status() != WL_CONNECTED && k_wifi_errors < WIFI_ERRORS) {
    Serial.print(".");
    k_wifi_errors++;
    delay(100);
    digitalWrite(BUILTIN_LED, HIGH);
    delay(100);
    digitalWrite(BUILTIN_LED, LOW);
  }
  Serial.println();

  // Error with WiFI connection, need debug
  if (k_wifi_errors >= WIFI_ERRORS)
    criticalError("Error with WiFi connection! Connection time exceeded!");

  // Successfully connected
  digitalWrite(BUILTIN_LED, LOW);
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  // Ping request
  const String url = API_LINK + "&request=ping";
  Serial.println(url);
  Serial.print("Test Ping: ");

  http.begin(client, url);
  int http_code = http.GET();
  String http_payload = http.getString();
  http.end();

  Serial.print(http_code);
  Serial.print(", ");
  Serial.println(http_payload);
  
  if (http_code != 200) criticalError();
}


void loop() {
  http.begin(client, API_LINK + "&request=getStatus");
  
  int http_code = http.GET();
  if (http_code != 200) criticalError();
  String payload = http.getString();
  http.end();

  // Parse payload
  if (payload == "00") {
    Serial.println("Closing");
  } else if (payload == "10") {
    Serial.println("Opening");
  }

  delay(1000);
}

void criticalError() {
  return (criticalError("Unsigned error!"));
}

void criticalError(String error) {
  while (true) {
    Serial.println(error);
    for (int _i = 0; _i < 5; _i++) {
      digitalWrite(BUILTIN_LED, LOW);
      delay(100);
      digitalWrite(BUILTIN_LED, HIGH);
      delay(100);
    }
    digitalWrite(BUILTIN_LED, LOW);
    delay(3000);
    digitalWrite(BUILTIN_LED, HIGH);
    delay(1000);
  };
}
