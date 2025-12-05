#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define OLED 0x3C // pin del dispaly

#define OLED_SDA 21 // pin SDA
#define OLED_SCL 22 // pin SCL
#define OLED_RST -1 // RESET OLED
#define ROW 64 // numero righe
#define COL 128 // numero colonne

Adafruit_SSD1306 display = Adafruit_SSD1306(COL,ROW,&Wire,OLED_RST); // Oggetto che controlla il display

void setup() {
  Wire.begin(OLED_SDA,OLED_SCL);
  while(!display.begin(SSD1306_SWITCHCAPVCC,OLED)){ // Controllo che il display sia collegato
    printf("Display initialization failed");
  }
  display.setTextColor(WHITE); //Colore testo impostato a bianco
  display.setCursor(0,0); // Inizzializzo il puntatore a codinate 0,0 ovvero l'angolo in alto a sinistra
  display.clearDisplay(); // Pulisco il display
  for(int i = 1;i<4;i++){ // Eseguo il codice 3 volte
    display.setTextSize(i); // Dimensione del testo impostato a i con viene aumentato da un ciclo for
    display.println("21-11-25"); // Stampo data odierna
  }
  display.display(); // Carico sul displai i comandi in memoria
}
// Il void non è utilizzato
void loop(){}
