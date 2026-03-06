#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>

// Pin I2C, dimensioni OLED e indirizzi I2C dei dispositivi
#define OLED_SDA  21
#define OLED_SCL  22
#define OLED_RST  -1
#define SCREEN_W  128
#define SCREEN_H  64
#define OLED_ADDR 0x3C
#define BMP_ADDR  0x77

Adafruit_SSD1306 display(SCREEN_W, SCREEN_H, &Wire, OLED_RST);
Adafruit_BMP280  bmp;

// Credenziali WiFi e broker MQTT
const char* SSID        = "TPSITquinteBis";
const char* PASSWORD    = "quintaemme";
const char* MQTT_SERVER = "test.mosquitto.org";
const int   MQTT_PORT   = 1883;
const char* CLIENT_ID   = "esp32_publisher_01";  // ID univoco sul broker

// Topic su cui vengono pubblicati i dati del sensore
const char* TOPIC_TEMP = "scuola/fasto/temperatura";
const char* TOPIC_PRES = "scuola/fasto/pressione";

#define PUBLISH_INTERVAL_MS 3000UL  // pubblica ogni 3 secondi

WiFiClient   espClient;
PubSubClient client(espClient);  // client MQTT che usa la connessione WiFi

// Mostra una o due righe di testo sull'OLED
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

// Mostra temperatura e pressione formattate sull'OLED
void mostraDati(float temp, float pres) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);  display.println("-- BMP280 MQTT --");
  display.setCursor(0, 20); display.print("Temp: "); display.print(temp, 1); display.println(" C");
  display.setCursor(0, 36); display.print("Pres: "); display.print(pres / 100.0, 1); display.println(" hPa");
  display.display();
}

void connetti_WiFi() {
  mostraMessaggio("Connessione WiFi...");
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {  // attende finché non è connesso
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi: " + WiFi.localIP().toString());
  mostraMessaggio("WiFi connesso!", WiFi.localIP().toString());
  delay(1500);
}

void connetti_MQTT() {
  mostraMessaggio("Connessione MQTT...");
  while (!client.connected()) {  // riprova finché non si connette
    Serial.print("Tentativo MQTT...");
    if (client.connect(CLIENT_ID)) {
      Serial.println("connesso al broker!");
      mostraMessaggio("Connesso con", "broker!");
      delay(1500);
    } else {
      Serial.print("Errore: ");
      Serial.println(client.state());  // codice errore PubSubClient
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
    while (true);  // blocca se il display non risponde
  }
  display.clearDisplay();
  display.display();

  if (!bmp.begin(BMP_ADDR)) {
    Serial.println("BMP280 non trovato!");
    mostraMessaggio("BMP280", "non trovato!");
    while (true);  // blocca se il sensore non risponde
  }

  connetti_WiFi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
  connetti_MQTT();
}

unsigned long lastPublish = 0;  // timestamp dell'ultima pubblicazione

void loop() {
  if (!client.connected()) {
    connetti_MQTT();  // riconnessione automatica se il broker cade
  }
  client.loop();  // mantiene viva la connessione e processa i messaggi in arrivo

  unsigned long now = millis();
  if (now - lastPublish >= PUBLISH_INTERVAL_MS) {  // controllo intervallo senza usare delay()
    lastPublish = now;

    float temp = bmp.readTemperature();  // °C
    float pres = bmp.readPressure();     // Pa → convertita in hPa alla pubblicazione
    // Converte da float a stringa con 5/7 di minimo con 2 cifre decimali e li salva sul buf
    char bufTemp[10], bufPres[10];
    dtostrf(temp,       5, 2, bufTemp);  // float → stringa per publish MQTT
    dtostrf(pres/100.0, 7, 2, bufPres);

    client.publish(TOPIC_TEMP, bufTemp);
    client.publish(TOPIC_PRES, bufPres);

    Serial.printf("Pubblicato → Temp: %s °C | Pres: %s hPa\n", bufTemp, bufPres);
    mostraDati(temp, pres);
  }
}
