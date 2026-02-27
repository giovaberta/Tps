#include <Arduino.h>
#include <WiFi.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Ping.h>
#include <WiFi.h>

const char* ssid = "TPSITquinteBis";
const char* password = "quintaemme";
const char* serverIP = "192.168.114.106"; 
const int   serverPort = 2000;

WiFiClient client;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connessione al Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connesso! IP ESP32: " + WiFi.localIP().toString());
  Serial.printf("\nServer in ascolto su %s, alla porta %d\n",serverIP,serverPort);
}

void loop() {
  if (client.connect(serverIP, serverPort)) {
    client.println("prova di connessione"); // invia il messaggio a netcat
    Serial.println("connessione attiva");
    client.stop();
  } else {
    Serial.println("Connessione al server fallita, riprovo...");
  }

  delay(5000);
}
