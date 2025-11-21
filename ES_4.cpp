#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#define OLED_SDA 21 // pin SDA
#define OLED_SCL 22 // pin SCL
#define OLED_RST -1 // RESET OLED
#define ROW 64 // numero righe
#define COL 128 // numero colonne

Adafruit_SSD1306 display = Adafruit_SSD1306(COL, ROW, &Wire, OLED_RST);

int array[6];
int cont = 0;
float raw_temp = 0;
float raw_press = 0;
uint8_t p_msb, p_lsb, p_xlsb;
uint8_t t_msb, t_lsb, t_xlsb;

void setup() {
  Serial.begin(115200); // Avvia la comunicazione seriale
  Wire.begin(OLED_SDA, OLED_SCL);
  
  // Inizializzazione display
  while (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Display initialization failed!");
    delay(1000); // Attendere prima di riprovare
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  
  // Configurazione sensore
  Wire.beginTransmission(0x77);  // Invia il comando per la lettura
  Wire.write(0xF4);
  Wire.write(0x27); // Avvia la lettura di temperatura e pressione
  Wire.endTransmission(true);
}

void loop() {
  Wire.beginTransmission(0x77);
  Wire.write(0xF7); // Indirizzo di lettura dei dati
  Wire.endTransmission(true);
  
  // Richiedi 6 byte di dati (3 per la temperatura, 3 per la pressione)
  Wire.requestFrom(0x77, 6);
  
  // Controlla che ci siano abbastanza byte
  if (Wire.available() == 6) {
    // Leggi i byte e assegnali all'array
    for (int i = 0; i < 6; i++) {
      array[i] = Wire.read();
    }
    
    // Assegna i valori letti ai rispettivi byte di pressione e temperatura
    p_msb = array[0];
    p_lsb = array[1];
    p_xlsb = array[2];
    t_msb = array[3];
    t_lsb = array[4];
    t_xlsb = array[5];
    
    // Calcola la pressione e la temperatura
    raw_press = ((uint32_t)p_msb << 12) | ((uint32_t)p_lsb << 4) | (p_xlsb >> 4);
    raw_temp = ((uint32_t)t_msb << 12) | ((uint32_t)t_lsb << 4) | (t_xlsb >> 4);
    
    // Stampa solo i dati richiesti sul seriale
    Serial.print("Temperatura: ");
    Serial.print(raw_temp);
    Serial.println(" C");

    Serial.print("Pressione: ");
    Serial.print(raw_press);
    Serial.println(" hPa");
  } else {
    // Se non ricevi 6 byte, stampa un errore
    Serial.println("Errore nella lettura dei dati!");
  }
  
  // Mostra i dati sul display OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.printf("Temperatura: %.2f C\n", raw_temp);
  display.printf("Pressione: %.2f hPa\n", raw_press);
  display.display();
  
  delay(1000); // Pausa di 1 secondo
}
