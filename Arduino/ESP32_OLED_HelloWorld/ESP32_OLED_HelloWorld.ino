#include "heltec.h"

void setup() {
  pinMode(LED, OUTPUT);

  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/, true /*Serial Enable*/);
  Heltec.display -> clear();
  Heltec.display -> drawString(0, 0, "Hello World!");
  Heltec.display -> drawString(0, 8, "Hello World!");
  Heltec.display -> display();
}

void loop() {
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(500);
}
