#include <Wire.h>              
#include <ctime>                
#include <Arduino.h>            
#include <Adafruit_SSD1306.h>  
#include <Adafruit_AHTX0.h>    
#include <Adafruit_BMP280.h>    

#define OLED 0x3C // pin display
#define OLED 0x77 // pin del bmp280

// Creazione oggetti sensori
Adafruit_BMP280 bmp; // Sensore BMP280 per pressione e temperatura
Adafruit_AHTX0 aht;  // Sensore AHTX0 per temperatura e umidità

// Definizione pin I2C per il display OLED
#define OLED_SDA 21 // pin SDA
#define OLED_SCL 22 // pin SCL
#define OLED_RST -1 // RESET OLED
#define ROW 64 // numero righe
#define COL 128 // numero colonne

// Creazione oggetto display OLED con dimensioni e gestione I2C
Adafruit_SSD1306 display(COL, ROW, &Wire, OLED_RST);

// Funzione per impostare manualmente data e ora del sistema
void setManualTime() {
    struct tm manualTime;

    manualTime.tm_year = 2025 - 1900;  // Anno
    manualTime.tm_mon  = 11; // Mese (0=gennaio, quindi 11=Dicembre)
    manualTime.tm_mday = 5; // Giorno del mese
    manualTime.tm_hour = 9; // Ora
    manualTime.tm_min  = 34; // Minuti
    manualTime.tm_sec  = 0; // Secondi
    manualTime.tm_isdst = -1; // Disabilita ora legale

    time_t t = mktime(&manualTime);

    // Struttura per impostazione dell’orologio di sistema
    struct timeval now = { .tv_sec = t };
    settimeofday(&now, nullptr); // Imposta l’orologio interno
}

void setup() {
    Serial.begin(115200);
    Wire.begin(OLED_SDA, OLED_SCL);

    setManualTime(); // Imposta ora manuale definita nella funzione

    // Inizializza sensore BMP280, riprova finché non funziona
    while (!bmp.begin()){
        printf("bmp inizialization failed\n");
    }

    // Inizializza sensore AHTX0, riprova finché non funziona
    while (!aht.begin()){
        printf("aht inizialization failed\n");
    }

    // Inizializza display OLED, riprova finché non funziona
    while (!display.begin(SSD1306_SWITCHCAPVCC,OLED)) {
        printf("display initialization failed\n");
    }

    // Pulisce il display e prepara impostazioni testo
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0,0);

    // Configurazione iniziale particolare per BMP280
    Wire.beginTransmission(BMP280);
    Wire.write(0xF4);
    Wire.write(0x27);
    Wire.endTransmission(true);
}

void loop() {
    // Ottiene tempo corrente in secondi
    time_t current_time_seconds = time(NULL);
    // Converte in ora locale (struttura tm)
    struct tm* local_time = localtime(&current_time_seconds);

    // Estraggo ora, minuti, secondi alla struttura tm
    int ora = local_time->tm_hour;
    int min = local_time->tm_min;
    int sec = local_time->tm_sec;

    // Lettura temperatura e umidità dal sensore AHT
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);

    // Mostra ora sul display
    display.clearDisplay();
    display.setCursor(0,0);
    display.printf("Ora: %02d:%02d:%02d", ora, min, sec);

    // Mostra temperatura e umidità sul display del aht
    display.printf("\nAHT\nTemperatura: %.2f\nUmidita: %.2f",temp.temperature, humidity.relative_humidity);

    display.display();
    delay(1000);        

    // Mostra ora sul display
    display.clearDisplay();
    display.setCursor(0,0);
    display.printf("Ora: %02d:%02d:%02d", ora, min, sec);

    // Mostra su display temperatura e pressione da bmp
    display.printf("\nBMP\nTemperatura: %.2f\nPressione: %.2f",
                   bmp.readTemperature(), bmp.readPressure());
    display.display();
    delay(1000);        
}
