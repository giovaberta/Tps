#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

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
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  //Dichiarazione righe
  display.drawLine(8,4,125,4,WHITE); // Riga Orzz superiore
  display.drawLine(8,4,8,56,WHITE); // Riga Vert sinistra
  display.drawLine(8,56,125,56,WHITE); // Riga Orzz inferiore
  display.drawLine(125,56,125,4,WHITE); // Riga Vert destra
  display.drawLine(8,4,125,56,WHITE); // Diagonale supSx,infDx
  display.drawLine(8,56,125,4,WHITE); // Diagonale infSx,supDx
  //Mostra sul display
  display.display();
}
void loop(){  }
