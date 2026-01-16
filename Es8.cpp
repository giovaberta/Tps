#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>

#define DisplayPin 0x3C
#define BmpPin 0x77

#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RST -1
#define ROW 64
#define COL 128

// Dichiaro le task
void taskProd(void *parameter);
void taskCons(void *parameter);

// Dichiare i due controller per il display e per il bmp
Adafruit_SSD1306 display= Adafruit_SSD1306(COL,ROW,&Wire,OLED_RST);
Adafruit_BMP280 bmp;

// Dichiaro due semafori
SemaphoreHandle_t semBin;

// Dichiaro variabile globale per lo scambio di dati
float temp,press;

void setup() {
  Serial.begin(115200);

  // Creo una istanza dei due semafori binari
  semBin = xSemaphoreCreateBinary();

  // In caso il sensore bmp non sia correttamente collegato
  while(!bmp.begin()){
    printf("bmp inizialization failed\n");
  }

  Wire.begin(OLED_SDA,OLED_SCL);
  while(!display.begin(SSD1306_SWITCHCAPVCC,DisplayPin)){
   printf("Display initialization falied"); //In caso qualcosa non andasse a buon fine
  }

  display.clearDisplay(); // Pulisco lo scermo preventivamente all'inizio del codice
  //Dichiarazioni per eventuali testi (in questo caso non utilizzati)
  display.setTextColor(WHITE); //Colore testo impostato a bianco
  display.setTextSize(1); // Dimensione del testo impostato a 1
  display.setCursor(0,0); // Inizzializzo il puntatore a codinate 0,0 ovvero l'angolo in alto a sinistra
  display.display(); // Mostro sul dispaly quello che ho caricato in memoria

  // Crea task Produttore (priorità 1, stack 2048)
  xTaskCreatePinnedToCore(
    taskProd,    // Funzione task
    "taskProd",  // Nome task
    2048,             // Stack size assegnato al task
    NULL,             // Parametri
    1,                // Priorità 1
    NULL,             // Task handle
    1                 // Core 1
  );

  // Crea task Consumatori (priorità 1, stack 2048)
  xTaskCreatePinnedToCore(
    taskCons,  // Funzione task
    "TaskCons",   // Nome task
    2048,          // Stack size assegnato al task
    NULL,          // Parametri
    1,             // Priorità 1
    NULL,          // Task handle
    1              // Core 1
  );
}

void loop() {} // Il Loop non serve dato che viene tutto gestito dai task

// Task 1:
void taskProd(void *parameter) {
  while(1) {  // Ciclo infinito
    // Leggo i dati dal sensore bmp280 e li salvo sulle variabili                      
    press = bmp.readPressure();
    temp = bmp.readTemperature();
    xSemaphoreGive(semBin); // Da il semaforo al task 2
    vTaskDelay(1000); // Attende 1 secondo
  }
}

// Task Consumatore: Stampa i valori sulla seriale e sul dysplay
void taskCons(void *parameter) {
  while(1) { // Ciclo infinito        
    xSemaphoreTake(semBin,portMAX_DELAY); // Attendo che il task 1 dia il semaforo
    // Stampo i valori sulla seriale e sul display
    display.setCursor(0,0);
    Serial.printf("Temperatura: %.2f, Pressione: %.2f\n",temp,press);
    display.printf("Temperatura: %.2f\nPressione: %.2f",temp,press);
    display.display();
    display.clearDisplay();
  }
}
