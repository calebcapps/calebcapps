#include <WiFi.h>
#include <esp_wpa2.h>

#include <ESPDash.h>

const char* ssid = "GentexCorporate";
#define username "caleb.capps"
#define password "Helen4102023!";
 
AsyncWebServer server(80);
ESPDash dashboard(&server); 
 
Card temperature(&dashboard, TEMPERATURE_CARD, "Temperature", "°C");
Card humidity(&dashboard, HUMIDITY_CARD, "Humidity", "%");
 
void setup() {
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
   
  Serial.println(WiFi.localIP());
   
  server.begin();
}
 
void loop() {
 
  int temp = random(10, 40);
  int hum = random(0, 100);
   
  temperature.update(temp);
  humidity.update(hum);
 
  dashboard.sendUpdates();
 
  delay(1000);
}
