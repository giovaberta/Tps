#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>

#define OLED 0x3C // pin del display
#define BMP280 0x77 // pin del bmp280

#define OLED_SDA 21 // pin SDA
#define OLED_SCL 22 // pin SCL
#define OLED_RST -1 // RESET OLED
#define ROW 64 // numero righe
#define COL 128 // numero colonne

Adafruit_SSD1306 display= Adafruit_SSD1306(COL,ROW,&Wire,OLED_RST); // Oggetto che controlla il display

int array[6]; // Dichiaro un arrey di dimensione 6 per contenere i 6 bite
float temp = 0, press = 0; // Variabili per la memorizzazione dei dati grezzi
int cont = 0; // Variabile di appoggio

void setup() {
  Serial.begin(115200); // Avvia la comunicazione seriale
  Wire.begin(OLED_SDA, OLED_SCL);
  // Inizializzazione display
  while (!display.begin(SSD1306_SWITCHCAPVCC, OLED)) {
    Serial.println("Display initialization failed!");
  }
  display.setTextColor(WHITE); //Colore testo impostato a bianco
  display.setTextSize(1); // Dimensione del testo impostato a 1
  display.setCursor(0,0); // Inizializzo il puntatore a coordinate 0,0 ovvero l'angolo in alto a sinistra
  display.clearDisplay(); // Pulisco il display
  }

void loop() {
  // Trasmissione dei dati
  Wire.beginTransmission(BMP280); // Inizio la trasmissione con il bmp280
  Wire.write(0xF7); // Registro da dove comincio a leggere i dati
  Wire.endTransmission(true);
  // Richiedi 6 byte di dati (3 per la temperatura, 3 per la pressione)
  Wire.requestFrom(BMP280, 6);
  // Assegno in un array tutti i 6 registri che ho letto
  while(Wire.available()) {
    array[cont]=Wire.read();
    cont+=1;
    }
  cont = 0;
  display.clearDisplay();
  // Eseguo una proporzione per ricavarmi i dati compensati da quelli grezzi
  press=(uint32_t) ((array[2]+(array[1]<<8)+(array[0]<<16))>>4)*800/1048575.0+300; // Unità di misure hpa (ettopascal)
  temp=(uint32_t)((array[5]+(array[4]<<8)+(array[3]<<16))>>4)*125/1048575.0-40; // Unità di misura °C

  // Stampo i  valori sulla seriale (il .2f e per far stampare solo due cifre dopo la virgola)
  Serial.printf("temperatura: %.2f°C\n", temp);
  Serial.printf("pressione: %.2fhpa\n", press);

  // Stampo i valori sul display (il .2f e per far stampare solo due cifre dopo la virgola)
  display.setCursor(0,0);
  display.printf("temperatura: %.2f C\n", temp);
  display.printf("pressione: %.2fhpa\n", press);
  display.display();  
 
  delay(1000); // Attendo 1 secondo
}
