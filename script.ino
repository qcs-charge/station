#include "ESP8266WiFi.h"
#include "WiFiClient.h"
#include "ESP8266WebServer.h"
#include "ESP8266HTTPClient.h"


const String WIFI_SSID = "connecting...";
const String WIFI_PASS = "81790487";
const String API_LINK = "http://qcs.pythonanywhere.com/api/station/?token=INSECURE-ynlht-js081-0z2b5-";
const int API_ERRORS = 10;
const int WIFI_ERRORS = 100;
const auto SIGNAL_LED = BUILTIN_LED;


HTTPClient http;
WiFiClient client;


void setup() {
  // Initialising
  Serial.begin(115200);
  Serial.println();

  pinMode(SIGNAL_LED, OUTPUT);
  digitalWrite(SIGNAL_LED, LOW);
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
    digitalWrite(SIGNAL_LED, HIGH);
    delay(100);
    digitalWrite(SIGNAL_LED, LOW);
  }
  Serial.println();

  // Error with WiFI connection, need debug
  if (k_wifi_errors >= WIFI_ERRORS)
    criticalError("Error with WiFi connection! Connection time exceeded!");

  // Successfully connected
  digitalWrite(SIGNAL_LED, LOW);
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());


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
  
  if (http_code != 200) {
    criticalError();
  }
}


void loop() {
  // Start HTTP session, fetch API
  http.begin(client, API_LINK);
  
  int http_code = http.GET();
  String http_payload = http.getString();

  Serial.println(http_code);
  http.end();

  if (http_code != 200) {
    criticalError();
  }
  // Process payload

  int code = parseApiPayload(http_payload);
  Serial.println(code);
  delay(10000);
}


int parseApiPayload(String payload) {
  return (0);
}


void criticalError() {
  return (criticalError("Unsigned error!"));
}


void criticalError(String error) {
  // Terminate main loop, activate Signal LED blinking
  while (true) {
    Serial.println(error);
    for (int _i = 0; _i < 5; _i++) {
      digitalWrite(SIGNAL_LED, LOW);
      delay(100);
      digitalWrite(SIGNAL_LED, HIGH);
      delay(100);
    }
    digitalWrite(SIGNAL_LED, LOW);
    delay(3000);
    digitalWrite(SIGNAL_LED, HIGH);
    delay(1000);
  };
}
