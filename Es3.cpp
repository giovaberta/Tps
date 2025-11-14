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
  //Inizzializzazione dello scermo
  Wire.begin(OLED_SDA,OLED_SCL);
  while(!display.begin(SSD1306_SWITCHCAPVCC,0x3C)){
    printf("Display initialization falied"); //In caso qualcosa non andasse a buon fine
  }
  display.clearDisplay(); // Pulisco lo scermo preventivamente all'inizio del codice
  //Dichiarazioni per eventuali testi (in questo caso non utilizzati)
  display.setTextColor(WHITE); //Colore testo impostato a bianco
  display.setTextSize(1); // Dimensione del testo impostato a 1
  display.setCursor(0,0); // Inizzializzo il puntatore a codinate 0,0 ovvero l'angolo in alto a sinistra
  // Scrivo tre parole con dimensioni diverse una sotto l'altra
  display.println("Piccolo");
  display.setTextSize(2); // Imposto la dimensione del testo a 2
  display.println("Medio");
  display.setTextSize(3); // Imposto la dimensione del testo a 3
  display.println("Grande");
  display.display(); // Mostro sul dispaly quello che ho caricato in memoria
}
void loop(){  }
