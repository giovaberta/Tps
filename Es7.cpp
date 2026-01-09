#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define DisplayPin 0x3C

#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RST -1
#define ROW 64
#define COL 128

// Dichiaro le task
void taskProd1(void *parameter);
void taskProd2(void *parameter);
void taskCons(void *parameter);

Adafruit_SSD1306 display= Adafruit_SSD1306(COL,ROW,&Wire,OLED_RST);

// Dichiaro variabile di tipo coda  
QueueHandle_t q1;

void setup() {
  Serial.begin(115200);

  // Creazione della coda
  q1 = xQueueCreate(5,sizeof(int));

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

  // Crea task Produttore 1 (priorità 1, stack 2048)
  xTaskCreatePinnedToCore(
    taskProd1,    // Funzione task
    "taskProd1",  // Nome task
    2048,             // Stack size assegnato al task
    NULL,             // Parametri
    1,                // Priorità 1
    NULL,             // Task handle
    1                 // Core 1
  );
  
  // Crea task Produttore 2 (priorità 1, stack 2048)
  xTaskCreatePinnedToCore(
    taskProd2,  // Funzione task
    "TaskProd2",   // Nome task
    2048,          // Stack size assegnato al task
    NULL,          // Parametri
    1,             // Priorità 1
    NULL,          // Task handle
    1              // Core 1
  );

  // Crea task Consumatori (priorità 2, stack 2048)
  xTaskCreatePinnedToCore(
    taskCons,  // Funzione task
    "TaskCons",   // Nome task
    2048,          // Stack size assegnato al task
    NULL,          // Parametri
    2,             // Priorità 1
    NULL,          // Task handle
    1              // Core 1
  );
}

void loop() {} // Il Loop non serve dato che viene tutto gestito dai task

// Task 1: Accende Led giallo
void taskProd1(void *parameter) {
  int numPar = 0;
  while(1) {  // Ciclo infinito                      
    xQueueSend(q1,&numPar,0); // Inserisco il valore nella coda 1
    vTaskDelay(1000); // Attendo 1000ms
    numPar += 2; // Icremento di 2
  }
}

// Task 2: Accende il led rosso
void taskProd2(void *parameter) {
  int numDisp = 1;
  vTaskDelay(50); // Leggero delay per farli altrenati 
  while(1) {     // Ciclo infinito                 
    xQueueSend(q1,&numDisp,0); // Inserisco il valore nella coda 1
    vTaskDelay(1000); // Attendo 1000ms
    numDisp += 2; // Icremento di 2
  }
}

// Task 3 : Accende entrambi i led
void taskCons(void *parameter) {
  while(1) { // Ciclo infinito        
     int num;   
    xQueueReceive(q1,&num,portMAX_DELAY); // Prendo un elemento dalla cosa
    Serial.printf("Numero: %d\n",num); // Stampo sulla seriale
    display.printf("Numero: %d\n",num); // Stampo sul display
    display.setCursor(0,0); // Setto il lcursore del display a cordinate 0,0
    display.display();
    vTaskDelay(500); // Attendo 500ms
    display.clearDisplay(); // Pulisco il dipslay 
  }
}
