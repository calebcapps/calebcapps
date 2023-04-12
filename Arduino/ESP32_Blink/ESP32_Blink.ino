<<<<<<< Updated upstream
=======
#include <WiFi.h>
//#include <MUIU8g2.h>
#include <U8g2lib.h>
#include <U8x8lib.h>



U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, 15, 4, 16);

>>>>>>> Stashed changes
/*
  ESP32 Blink
  esp32-blink.ino
  Rewrite of classic Blink sketch for ESP32
  Use LED on GPIO2
   
  DroneBot Workshop 2020
  https://dronebotworkshop.com
*/
 
<<<<<<< Updated upstream
// LED on GPIO2 is the builtin LED
int ledPin = 2;
=======
// LED on GPIO2
int ledPin = 26;
>>>>>>> Stashed changes
 
void setup()
{
    // Set LED as output
    pinMode(ledPin, OUTPUT);
    
    // Serial monitor setup
    Serial.begin(115200);

    u8g2.begin();
    u8g2.setFont(u8g2_font_6x12_mf); // fairly small font
    u8g2.setFontRefHeightExtendedText();
    u8g2.setDrawColor(1); // normal, not inverted
    u8g2.setFontPosTop(); // x,y is at top of font
    u8g2.setFontDirection(0); // not rotated
    u8g2.drawStr(0, 0, "It Works!");
    u8g2.sendBuffer();
}
 
void loop()
{
//    Serial.print("Hello");
//    digitalWrite(ledPin, HIGH);
//    
//    delay(500);
//    
//    Serial.println(" world!");
//    digitalWrite(ledPin, LOW);
//    
//    delay(500);
//    
//    Serial.print("Upload");
//    digitalWrite(ledPin, HIGH);
//    
//    delay(1000);
//    
//    Serial.println(" complete!");
//    digitalWrite(ledPin, LOW);
//    
//    delay(1000);
}
