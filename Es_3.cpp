#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>

#define BMP280 0x77 // Definisco il sensore bmp
#define OLED 0x3C // pin del display

#define OLED_SDA 21 // pin SDA
#define OLED_SCL 22 // pin SCL
#define OLED_RST -1 // RESET OLED
#define ROW 64 // numero righe
#define COL 128 // numero colonne

Adafruit_SSD1306 display= Adafruit_SSD1306(COL,ROW,&Wire,OLED_RST); // Oggetto che controlla il display
Adafruit_BMP280 bmp; // Oggetto che controlla il bmp

uint8_t readRegister(uint8_t reg) { // Funzione che legge un registro I2C del BMP280
  Wire.beginTransmission(BMP280); //Selezione il sensore bmp
  Wire.write(reg); // Legge il registro che è stato passato
  Wire.endTransmission(); // Finisce la letture
  Wire.requestFrom(BMP280, (uint8_t)1); // Richiede 1byte di risposta dal bmp
  return Wire.read(); // Ritorno il byte letto
}

void setup() {
  Serial.begin(115200);
  Wire.begin(OLED_SDA,OLED_SCL);
  // Se non rileva il display manda in errore
  while(!display.begin(SSD1306_SWITCHCAPVCC,OLED)){
    Serial.printf("Display initialization falied");
  }
  display.setTextColor(WHITE); //Colore testo impostato a bianco
  display.setTextSize(1); // Dimensione del testo impostato a 1
  display.setCursor(0,0); // Inizializzo il puntatore a coordinate 0,0 ovvero l'angolo in alto a sinistra
  display.clearDisplay(); // Pulisco il display

  if (!bmp.begin(0x77)) { // Prova a comunicare con il sensore
    // Se non trova il sensore da errore e ferma il codice
    Serial.println("BMP280 non trovato!");
    display.println("BMP280 ERR");
    display.display();
    while (1);
  }
  else{
    // Il sensore è stato rilevato e lo comunica al display e lo comunica alla seriale
    Serial.println("BMP280 trovato");
    display.println("BMP280 OK");
    display.display();
  }
  uint8_t id = readRegister(0xD0); // Leggo il registro 0XD0 contiene il Chip Id
  // Stampo la risposta sulla seriale
  Serial.print("BMP ID: 0x");
  Serial.println(id, HEX); // Trasformo l'id in esadecimale
  // Stampo la risposta sul display e la mostro
  display.setCursor(0, 16);
  display.print("ID: 0x");
  display.println(id, HEX); // Trasformo l'id in esadecimale
  display.display();
}

// Funzione main inutilizzata
void loop() {  
}
