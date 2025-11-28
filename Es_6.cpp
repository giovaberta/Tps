#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>

#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RST -1
#define ROW 64
#define COL 128

Adafruit_AHTX0 aht; // Crea un oggetto sensore AHTX0
Adafruit_SSD1306 display= Adafruit_SSD1306(COL,ROW,&Wire,OLED_RST); // Oggetto che controlla il display

void setup()
{
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

  while (!Serial) delay(10); // Attende la connessione della porta seriale
  Serial.println("Test Sensore AHT20");

  
  if (! aht.begin()) //Inizializza il sensore
  {
  Serial.println("Errore: Impossibile trovare il sensore AHT20! Controlla i collegamenti I2C.");
  while (1) delay(10); // Loop infinito in caso di errore
  }
  Serial.println("Sensore AHT20 trovato e inizializzato.");
}

void loop()
{
  // Struttura per memorizzare i dati dell'evento
  sensors_event_t miaUmidita, miaTemperatura;
  // Ottieni i nuovi dati dal sensore
  if (aht.getEvent(&miaUmidita, &miaTemperatura)){
    // Stampa i valori letti sulla seriale
    Serial.print("Temperatura: ");
    Serial.print(miaTemperatura.temperature);
    Serial.println(" °C");
    Serial.print("Umidità: ");
    Serial.print(miaUmidita.relative_humidity);
    Serial.println(" %");

    // Stampa i valori letti su oled
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("Temperatura: ");
    display.print(miaTemperatura.temperature);
    display.println(" *C");
    display.print("Umidità: ");
    display.print(miaUmidita.relative_humidity);
    display.println(" %");
    display.display();
  }
  else{
    Serial.println("Errore durante la lettura del sensore.");
  }
  delay(5000); // Lettura ogni 5 secondi
}
