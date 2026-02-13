#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_BMP280.h>
#include <BluetoothSerial.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RST -1
#define ROW 64
#define COL 128
Adafruit_SSD1306 display= Adafruit_SSD1306(COL,ROW,&Wire,OLED_RST);
Adafruit_BMP280 bmp;
BluetoothSerial serialBT;


void setup() {
 Serial.begin(115200);
 bmp.begin();
 serialBT.begin("Berta_Schiavo", false);
 while (!display.begin(SSD1306_SWITCHCAPVCC,0x3C)){
    printf("display instalization failed");
    }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
}
 // Stampa l'array risultante

void loop() {
  if (serialBT.available()) {
    char buffer[64]; // Definisce la dimensione massima del messaggio
    int i = 0;

    // Legge finché c'è roba o finché il buffer non è pieno
    while (serialBT.available() && i < 63) {
      char c = serialBT.read();
      buffer[i] = c;
      i++;
      delay(2);
    }
   
    buffer[i] = '\0';

    display.clearDisplay();
    display.println(buffer);
    display.setCursor(0,0);
    display.display();
  }
  delay(20);
}
