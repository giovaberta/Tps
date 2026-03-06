#include<Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>  

#define BMP280 0x77 // Definisco il sensore bmp

const char* SSID     = "TPSITquinteBis";
const char* PASSWORD = "quintaemme";

const uint16_t PORT = 5000;
WiFiServer server(PORT);

Adafruit_BMP280 bmp;

void setup() {
  Serial.begin(115200);

  // Inizializza il sensore
  while (!bmp.begin()) {
    Serial.println("BMP280 non trovato!");
    delay(1000);
  }

  // Connessione WiFi
  Serial.print("Connessione WiFi");
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nIP ESP32: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.printf("Server TCP in ascolto sulla porta %d\n", PORT);
}

void loop() {
  // Attende fino a quando non si connette un client
  WiFiClient client = server.accept(); 

  if (client) { // Controllo che il client si sia connesso 
    Serial.println("Client connesso!");
    client.println("=== ESP32 Sensor Server ===");
    client.println("Comandi: p=pressione  t=temperatura q=quit");
    while (client.connected()) {
      if (client.available()) {
        char cmd = client.read(); // Leggo il messaggio inviato dal client
        
        // Scarta \r e \n (netcat li manda)
        if (cmd == '\r' || cmd == '\n') continue;

        Serial.printf("Comando ricevuto: '%c'\n", cmd);
        // Creo una macchina a stati per interpretare il comando
        switch (cmd) {
          // Per l'invio della pressione
          case 'p': {
            float pressure = bmp.readPressure();  // hPa
            char buf[64];
            snprintf(buf, sizeof(buf), "Pressione: %.2f hPa\n", pressure);
            client.print(buf); 
            break;
          }
          
          // Per l'invio della temperatura
          case 't': {
            float temp = bmp.readTemperature();
            char buf[64];
            snprintf(buf, sizeof(buf), "Temperatura: %.2f °C\n", temp);
            client.print(buf);
            break;
          }

          // Per il quit
          case 'q': {
            client.println("Disconnessione. Ciao!\n");
            client.stop();
            Serial.println("Client disconnesso.");
            break;
          }
        }
      }
    }
    client.stop();
    Serial.println("Client disconnesso.");
  }
}
