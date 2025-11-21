#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>

#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RST -1
#define ROW 64
#define COL 128

Adafruit_SSD1306 display= Adafruit_SSD1306(COL,ROW,&Wire,OLED_RST);


void setup() {
  Wire.begin(OLED_SDA,OLED_SCL);
  while(!display.begin(SSD1306_SWITCHCAPVCC,0x3C)){ 
    printf("Display initialization falied");
  }
  display.clearDisplay(); //Pulisco lo schermo
  display.drawCircle(64,32,29,WHITE); // Disegno un cerchio di 0,5cm convertiti in px di diametro
  display.display(); // Carico la memoria sul display
}

void loop() {

}

