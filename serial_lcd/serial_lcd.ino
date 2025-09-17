#include <WiFi.h>
#include "DHT.h"
#include <HTTPClient.h>

// DHT11 setup
#define DHTPIN 21
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// WiFi credentials
const char* ssid = "Redmi Note 9 Pro";     
const char* password = "soumya#06";        

// ThingSpeak settings
String apiKey = "396SUSJQZB63245W";   //  Write API key
const char* server = "http://api.thingspeak.com/update";

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
}

void loop() {
  delay(15000); // ThingSpeak requires 15 sec between updates

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temp: ");
  Serial.print(t);
  Serial.print(" °C | Hum: ");
  Serial.print(h);
  Serial.println(" %");

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Create the full URL with your API key and fields
    String url = String(server) + "?api_key=" + apiKey + "&field1=" + String(t) + "&field2=" + String(h);

    http.begin(url);  
    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.printf("Data sent! HTTP code: %d\n", httpCode);
      String payload = http.getString();
      Serial.println("Response: " + payload);
    } else {
      Serial.printf("Error sending data. HTTP code: %d\n", httpCode);
    }

    http.end();
  }
}
