#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>


#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RST -1
#define ROW 64
#define COL 128

Adafruit_SSD1306 display= Adafruit_SSD1306(COL,ROW,&Wire,OLED_RST);

// Dichiarazione dei 3 stati per le macchina a stati
enum Stato{
 reverseColor_1,
 reverseDisplay_1,
 reverseColor_2,
 reverseDisplay_2
};
Stato currentState; // Stato iniziale nessuno
unsigned long start = millis();

void stampa(){
  display.println("Ciao Mondo!");
  display.fillRect(20, 20, 50, 30, WHITE);
}

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
 stampa();
 display.display(); // Mostro sul dispaly quello che ho caricato in memoria
}
void loop(){
 switch (currentState)
 {
 case reverseColor_1:
   display.invertDisplay(true);
   if(millis()-start >= 2000){
     currentState = reverseDisplay_1;
     start = millis();
   }
 break;
 case reverseDisplay_1:
   display.setRotation(2);
   display.clearDisplay();
   stampa();
   display.display(); // Mostro sul dispaly quello che ho caricato in memoria
   if(millis()-start >= 2000){
     currentState = reverseColor_2;
     start = millis();
   }
 break;
 case reverseColor_2:
   display.invertDisplay(false);
   display.display(); // Mostro sul dispaly quello che ho caricato in memoria
   if(millis()-start >= 2000){
     currentState = reverseDisplay_2;
     start = millis();
   }
 break;
 case reverseDisplay_2:
   display.setRotation(0);
   display.clearDisplay();
   stampa();
   display.display(); // Mostro sul dispaly quello che ho caricato in memoria
   if(millis()-start >= 2000){
     currentState = reverseColor_1;
     start = millis();
   }
 default:
   if(millis()-start >= 2000){
     currentState = reverseColor_1;
     start = millis();
   }
   break;
 }
}
