#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

// Pin I2C e parametri display OLED
#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RST -1
#define SCREEN_W 128
#define SCREEN_H 64
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_W, SCREEN_H, &Wire, OLED_RST);

// Credenziali WiFi e parametri broker MQTT
const char* SSID        = "TPSITquinteBis";
const char* PASSWORD    = "quintaemme";
const char* MQTT_SERVER = "test.mosquitto.org";
const int   MQTT_PORT   = 1883;
const char* CLIENT_ID   = "esp32_client_01";  // ID univoco sul broker

WiFiClient   espClient;
PubSubClient client(espClient);  // client MQTT che usa la connessione WiFi

// Scrive una o due righe sull'OLED
void mostraMessaggio(String riga1, String riga2 = "") {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(riga1);
  if (riga2 != "") {
    display.setCursor(0, 20);
    display.println(riga2);
  }
  display.display();
}

void connetti_WiFi() {
  mostraMessaggio("Connessione WiFi...");
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {  // attende finché non è connesso
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connesso: " + WiFi.localIP().toString());
  mostraMessaggio("WiFi connesso!", WiFi.localIP().toString());
  delay(1500);
}

void connetti_MQTT() {
  mostraMessaggio("Connessione MQTT...");
  while (!client.connected()) {  // riprova finché non si connette al broker
    Serial.print("Tentativo MQTT...");
    if (client.connect(CLIENT_ID)) {
      Serial.println("connesso!");
      mostraMessaggio("Connesso con", "broker!");
    } else {
      Serial.print("Errore stato: ");
      Serial.println(client.state());  // stampa codice errore PubSubClient
      mostraMessaggio("MQTT fallito", "riprovo...");
      delay(3000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  Wire.begin(OLED_SDA, OLED_SCL);  // inizializza bus I2C
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED non trovato!");
    while (1);  // blocca se il display non risponde
  }
  display.clearDisplay();
  display.display();

  connetti_WiFi();

  client.setServer(MQTT_SERVER, MQTT_PORT);  // configura broker
  connetti_MQTT();
}

void loop() {
  if (!client.connected()) {
    connetti_MQTT();   // riconnessione automatica se il broker cade
  }
  client.loop();  // mantiene viva la connessione MQTT e processa i messaggi
}
