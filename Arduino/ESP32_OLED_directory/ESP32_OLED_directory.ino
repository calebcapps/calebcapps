#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>
#include "heltec.h"
#include "oled/OLEDDisplayUi.h"
#include "images.h"

/* You only need to format LittleFS the first time you run a
   test or else use the LITTLEFS plugin to create a partition
   https://github.com/lorol/arduino-esp32littlefs-plugin
   
   If you test two partitions, you need to use a custom
   partition.csv file, see in the sketch folder */

#define FORMAT_LITTLEFS_IF_FAILED true

// How long should the demo take?
#define DEMO_DURATION 3000
typedef void (*Demo)(void);
#define PIXELS_PER_LINE 10
#define PIXELS_PER_COL 1
extern Heltec_ESP32 Heltec;
OLEDDisplayUi ui( Heltec.display );

void msOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(128, 0, String(millis()));
}

void drawFrame1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  //get the dir string
  oled_draw_dir(LittleFS, "/", 3, display, x, y);
}
void oled_draw_dir(fs::FS &fs, String dirname, uint8_t levels, OLEDDisplay *display, int16_t x, int16_t y){
    String msg = "Listing directory: "+dirname;
    display->drawString(x, y, msg);
    x += 0; // move the cursor over
    y += PIXELS_PER_LINE;
    

    File root = fs.open(dirname);
    if(!root){
        msg = "- failed to open directory";
        display->drawString(x, y, msg);
        x = 0; // move the cursor over
        y += 1;   // new line
        return;
    }
    if(!root.isDirectory()){
        msg = " - not a directory";
        display->drawString(x, y, msg);
        x = 0; // move the cursor over
        y += 1;
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            String msg4 = "  DIR : ";
            display->drawString(x, y, msg4);
            Serial.print("Printing ");
            Serial.print(msg4);
            Serial.print(" on row ");
            Serial.print(y);
            Serial.print(" column ");
            Serial.println(x); 
            x +=msg4.length()*PIXELS_PER_COL; // move the cursor over
            String msg5 = file.name();
            display->drawString(x, y, msg5);
            Serial.print("Printing ");
            Serial.print(msg5);
            Serial.print(" on row ");
            Serial.print(y);
            Serial.print(" column ");
            Serial.println(x); 
            x = 0; // move the cursor over
            y += PIXELS_PER_LINE;   // new line
            Serial.println("NEWLINE SHOULD HAVE HAPPENED");
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        }
        else {
            msg = "  FILE: " + String(file.name());
            display->drawString(x, y, msg);
            x += 0; // move the cursor over
            y += PIXELS_PER_LINE;
            msg = "  SIZE: " + String(file.size());
            display->drawString(x, y, msg);
            x = 0; // move the cursor over
            y += PIXELS_PER_LINE;   // new line
        }
        file = root.openNextFile();
    }
}


void drawFrame2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->drawXbm(x, y, BT_width, BT_height, BT_bits);
  display->drawXbm(x + 12 + 1, y, WIFI_width, WIFI_height, WIFI_bits);
  display->drawXbm(x + 108, y, BAT_width, BAT_height, BAT_bits);
  display->drawXbm(x + 34, y + 12, LoRa_Logo_width, LoRa_Logo_height, LoRa_Logo_bits);  
}

void drawFrame3(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->drawXbm(x, y + 5, HelTec_LOGO_width, HelTec_LOGO_height, HelTec_LOGO_bits);
}

void drawFrame4(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);
  display->drawString(x, y, "HelTec");
  display->setFont(ArialMT_Plain_10);
  display->drawString(x, y + 25, "HelTec AutoMation");
  display->drawString(x, y + 35, "www.heltec.cn");
}

FrameCallback frames[] = { drawFrame1, drawFrame2, drawFrame3, drawFrame4 };

int frameCount = 4;

//LittleFS Commands
void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

String listDir2str(fs::FS &fs, const char * dirname, uint8_t levels){
    String output;
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return "N/A";
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return "N/A";
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            output += "  DIR : ";
            output += String(file.name());
            output += "\n";
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            output += "  FILE: ";
            output += String(file.name());
            output += "\tSIZE: ";
            output += String(file.size()) + '\n';
        }
        file = root.openNextFile();
    }
    return output;
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return;
    }

    Serial.println("- read from file:");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
    Serial.println();
}

String readFile2str (fs::FS &fs, const char * path){
    String output;
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return "N/A";
    }

    Serial.println("- read from file:");
    while(file.available()){
        output += char(file.read());
    }
    file.close();
    return output;
}

void setup(){
    Serial.begin(115200);

    String myFile;
    String myDir;
    if(!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)){
        Serial.println("LittleFS Mount Failed");
        return;
    }
    
    myDir = listDir2str(LittleFS, "/", /*levels to search*/3);
    Serial.println(myDir);

    myFile = readFile2str(LittleFS, "/HelloWorld.txt");
    //Serial.print("String saved as:");
    //Serial.println(myFile);
    //Serial.println( "Read complete" );

    Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);

  ui.setTargetFPS(30);

  // Customize the active and inactive symbol
  ui.setActiveSymbol(activeSymbol);
  ui.setInactiveSymbol(inactiveSymbol);

  // You can change this to
  // TOP, LEFT, BOTTOM, RIGHT
  ui.setIndicatorPosition(BOTTOM);

  // Defines where the first frame is located in the bar.
  ui.setIndicatorDirection(LEFT_RIGHT);

  // You can change the transition that is used
  // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_UP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);

  // Add frames
  ui.setFrames(frames, frameCount);

  // Initialising the UI will init the display too.
  ui.init();

  Heltec.display->flipScreenVertically();
}

void loop() {
  int remainingTimeBudget = ui.update();

  if (remainingTimeBudget > 0) {
    // You can do some work here
    // Don't do stuff if you are below your
    // time budget.
    delay(remainingTimeBudget);
  }
 }
