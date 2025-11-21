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
  display.setTextColor(WHITE); //Colore testo impostato a bianco
  display.setTextSize(1); // Dimensione del testo impostato a 1
  display.setCursor(0,0); // Inizzializzo il puntatore a codinate 0,0 ovvero l'angolo in alto a sinistra
  display.clearDisplay(); // Pulisco il display
  
  display.println("21-11-25"); // Stampo data odierna
  display.setTextSize(2); // Dimensione del testo impostato a 2
  display.println("21-11-25"); // Stampo data odierna
  display.setTextSize(3); // Dimensione del testo impostato a 3
  display.println("21-11-25"); // Stampo data odierna
  display.display();

}

void loop() {

}

