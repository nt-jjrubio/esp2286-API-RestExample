#include <stdio.h>
#include <ESP8266WebServer.h>

// Copy credentias.example.h to credentials.h and set wifi credentials
#include "credentials.h"

#define HTTP_REST_PORT 80
#define WIFI_RETRY_DELAY 600
#define MAX_WIFI_INIT_RETRY 50

IPAddress ip;

ESP8266WebServer http_rest_server(HTTP_REST_PORT);

int init_wifi() {
  Serial.printf("Connecting to WiFi %s...\n", ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(WIFI_RETRY_DELAY);
    Serial.print("#");
  }
  return WiFi.status();
}

void init_endpoints() {
  http_rest_server.on("/", HTTP_GET, []() {
    http_rest_server.send(200, "text/html", "<h1>Hello World</h1>");
  });
}



void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println("Booting...");

  if(init_wifi() == WL_CONNECTED){
    Serial.println("Connected OK");

    ip = WiFi.localIP();
    Serial.print("Connect at ");
    Serial.print(ip);
    init_endpoints();
  } else {
    Serial.printf("Error connecting to %s", ssid);
  }
}

void loop() {
  http_rest_server.handleClient();
}
