#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#define OLED 0x3C // pin display

#define OLED_SDA 21 // pin SDA
#define OLED_SCL 22 // pin SCL
#define OLED_RST -1 // RESET OLED
#define ROW 64 // numero righe
#define COL 128 // numero colonne

Adafruit_SSD1306 display= Adafruit_SSD1306(COL,ROW,&Wire,OLED_RST); // Oggetto che controlla il display

void setup() {
  // Inizializzazione del display
  Wire.begin(OLED_SDA,OLED_SCL);
  // Se non rileva il display manda in errore
  while (!display.begin(SSD1306_SWITCHCAPVCC,OLED)){
    printf("display instalization failed");
    }
  display.setTextColor(WHITE); //Colore testo impostato a bianco
  display.setTextSize(1); // Dimensione del testo impostato a 1
  display.setCursor(0,0); // Inizializzo il puntatore a coordinate 0,0 ovvero l'angolo in alto a sinistra
  display.clearDisplay(); // Pulisco il display
 
}
void loop() {
    int rip = 40; // Contatore che comincia a 40 e diminuisce 4 alla volta
    for(int cont = 0;cont<40;cont+=4){ // Contatore che comincia a 0 e aumenta 4 alla volta
      rip = rip - 4;
      display.clearDisplay(); // Ripulisco il display
      display.drawCircle(64,32,cont,WHITE); // Disegno il cerchio che si sta allargando
      display.drawCircle(64,32,rip,WHITE); // Disegno il cerchio che si sta rimpicciolendo
      display.display();
      delay(50); // Attendo 50ms
    }
}
