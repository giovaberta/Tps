#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_AHTX0.h>

#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RST -1
#define ROW 64
#define COL 128
#define BMP280_ADDR 0x77 // Definisco il sensore bmp

Adafruit_SSD1306 display= Adafruit_SSD1306(COL,ROW,&Wire,OLED_RST); // Oggetto che controlla il display
Adafruit_BMP280 bmp; // Oggetto che controlla il bmp

uint8_t readRegister(uint8_t reg) { // Funzione che legge un regisro I2C del BMP280
  Wire.beginTransmission(BMP280_ADDR); //Selezione il sensore bmp
  Wire.write(reg); // Legge il registro che è stato passato
  Wire.endTransmission(); // Finisce la tletture
  Wire.requestFrom(BMP280_ADDR, (uint8_t)1); // Richiede 1byte di risposta dal bmp
  return Wire.read(); // Ritorno il byte letto
}

void setup() {
  Serial.begin(115200);
  Wire.begin(OLED_SDA,OLED_SCL);
  // Se non rileva il display manda in errore
  while(!display.begin(SSD1306_SWITCHCAPVCC,0x3C)){ 
    Serial.printf("Display initialization falied");
  }
  display.setTextColor(WHITE); //Colore testo impostato a bianco
  display.setTextSize(1); // Dimensione del testo impostato a 1
  display.setCursor(0,0); // Inizzializzo il puntatore a codinate 0,0 ovvero l'angolo in alto a sinistra
  display.clearDisplay(); // Pulisco il display


  if (!bmp.begin(0x77)) { // Prova a comuniocare con il sensore
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
  Serial.println(id, HEX);
  // Stampo la risposta sul dispaly e la mostro
  display.setCursor(0, 16);
  display.print("ID: 0x");
  display.println(id, HEX);
  display.display();
}

// Funzione main inutilizzata
void loop() {  
}

