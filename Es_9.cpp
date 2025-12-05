#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>

#define OLED 0x3C // pin del display

#define OLED_SDA 21 // pin SDA
#define OLED_SCL 22 // pin SCL
#define OLED_RST -1 // RESET OLED
#define ROW 64 // numero righe
#define COL 128 // numero colonne

Adafruit_SSD1306 display= Adafruit_SSD1306(COL,ROW,&Wire,OLED_RST); // Oggetto che controlla il display

void setup() {
  Wire.begin(OLED_SDA,OLED_SCL);
  // Se non rileva il display manda in errore
  while(!display.begin(SSD1306_SWITCHCAPVCC,0x3C)){ 
    printf("Display initialization falied");
  }
  display.clearDisplay(); //Pulisco lo schermo
  display.drawCircle(64,32,29,WHITE); // Disegno un cerchio di 0,5cm convertiti in px di diametro
  display.display(); // Carico la memoria sul display
}

// Il void non è utilizzato
void loop() {}
