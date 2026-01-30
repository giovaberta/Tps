#include <Arduino.h>
#include <BluetoothSerial.h>
#include <Adafruit_BMP280.h>

// Dichiaro l'oggetto bluetooth
BluetoothSerial SerialBT;
// Dichiaro le varie variabili per l'esecuzione del programma
char ris;
float temp,press;
// Oggetto bmp
Adafruit_BMP280 bmp;

void setup() {
  Serial.begin(115200);                             // Inizializzo la seriale
  SerialBT.begin("Esp32TestBerta");                 // Do il nome al blueTooth
  Serial.println("Inizio trasmissione");            // Stampo in seriale un messaggio di verifica
  while(!bmp.begin()){                              // Controllo che il bmp sia collegato correttamente 
    printf("Errore nel bmp");                       // Se viene rilevato un problema con il bmp stampo un messaggio di errore
  }
}
void loop() {
  if (SerialBT.available()) {                       // Se il c'è un dispositivo collegato
    ris = SerialBT.read();                          // Leggo se è arrivato un messaggio
    if(ris=='t' or ris=='T'){                       // Se il messagio è una "T" o "t" passa
      temp = bmp.readTemperature();                 // Leggo il valore della temperatura
      SerialBT.printf("Temperatura: %.2f",temp);    // Stampo sulla seriale del dispositivo BlueTooth collegato il dato della temperatura
    }
    if(ris=='p' or ris=='P'){                       // Se il mesaggio è una "P" o "p" passa
      press = bmp.readPressure();                   // Leggo il vaore della pressione sul bmp
      SerialBT.printf("Pressione: %.2f",press);     // Stampo sulla seriale del dispositivo BlueTooth collegato il dato della pressione 
    }   
  }
}
