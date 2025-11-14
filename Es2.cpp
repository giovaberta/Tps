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
  cerchio_1,
  cerchio_2,
  cerchio_3
};
Stato currentState = cerchio_1; // Stato iniziale
unsigned long start = millis();

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
}
void loop(){
  switch (currentState)
  {
  // Caso 1, il primo cerchio è riempito mentre gli altri due sono vuoti
  case cerchio_1:
    display.clearDisplay(); // Pulisco il display da tutto quello che c'era prima
    display.fillCircle(22,32,20,WHITE); // Cerchio 1 pieno 
    display.drawCircle(64, 32, 20, WHITE); // Cerchio 2 vuoto 
    display.drawCircle(106, 32, 20, WHITE); // Cerchio 3 pieno
    display.display(); // Mostro sul dispaly quello che ho caricato in memoria
    // Dopo 2 secondi passo allo stato successivo inizializzando la variabile start 
    if(millis()-start >= 2000){ 
      currentState = cerchio_2; // Stato succesivo
      start = millis();
    }
    break;
    // Caso 2, il secondo cerchio è riempito mentre gli altri due sono vuoti
    case cerchio_2:
      display.clearDisplay(); // Pulisco il display da tutto quello che c'era prima
      display.drawCircle(22, 32, 20, WHITE); // Cerchio 1 vuoto 
      display.fillCircle(64,32,20,WHITE); // Cerchio 2 pieno
      display.drawCircle(106, 32, 20, WHITE); // Cerchio 3 vuoto
      display.display(); // Mostro sul dispaly quello che ho caricato in memoria
      // Dopo 2 secondi passo allo stato successivo inizializzando la variabile start 
      if(millis()-start >= 2000){
        currentState = cerchio_3; // Stato succesivo
        start = millis();
      }
    break;
    // Caso 3, il terzo cerchio è riempito mentre gli altri due sono vuoti
    case cerchio_3:
      display.clearDisplay(); // Pulisco il display da tutto quello che c'era prima
      display.drawCircle(22, 32, 20, WHITE); // Cerchio 1 vuoto
      display.drawCircle(64,32,20,WHITE); // Cerchio 2 vuoto
      display.fillCircle(106, 32, 20, WHITE); // Cerchio 3 pieno
      display.display(); // Mostro sul dispaly quello che ho caricato in memoria
      // Dopo 2 secondi passo allo stato successivo inizializzando la variabile start 
      if(millis()-start >= 2000){
        currentState = cerchio_1; // Stato succesivo
        start = millis(); 
      }
    break;
  }
}



