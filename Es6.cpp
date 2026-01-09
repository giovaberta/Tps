#include <Arduino.h>

#define Led_Verde 26 // Il led giallo è assegnato al pin 26
#define Led_Rosso 25 // Il led rosso è assegnato al pin 25
#define Led_Azzurro 33 // Il led azzurro è assegnato al pin 33

// Dichiaro le task
void taskLedVerde(void *parameter);
void taskLedRosso(void *parameter);
void taskLedAzzurro(void *parameter);

// Dichiare tre variabile di tipo coda  
QueueHandle_t q1;
QueueHandle_t q2;
QueueHandle_t q3;

bool segnale = true;

void setup() {
  
  // Creazione delle tre code
  q1 = xQueueCreate(1,sizeof(bool));
  q2 = xQueueCreate(1,sizeof(bool));
  q3 = xQueueCreate(1,sizeof(bool));

  // Dichiaro il primo segnale per la prima coda a true

  xQueueSend(q1, &segnale, 0); // Invio il segnale alla coda 1 (Coda, Puntatore della variabile signal, Tempo di time out)

  // Configura pin LED come output
  pinMode(Led_Rosso, OUTPUT);
  pinMode(Led_Verde, OUTPUT);
  pinMode(Led_Azzurro, OUTPUT);
  // Spengo i led
  digitalWrite(Led_Rosso, LOW);
  digitalWrite(Led_Verde, LOW);
  digitalWrite(Led_Azzurro, LOW);

  // Crea task LED Verde (priorità 1, stack 2048)
  xTaskCreatePinnedToCore(
    taskLedVerde,    // Funzione task
    "taskLedVerde",  // Nome task
    2048,             // Stack size assegnato al task
    NULL,             // Parametri
    1,                // Priorità 1
    NULL,             // Task handle
    1                 // Core 1
  );
  
  // Crea task LED rosso (priorità 1, stack 2048)
  xTaskCreatePinnedToCore(
    taskLedRosso,  // Funzione task
    "TaskRosso",   // Nome task
    2048,          // Stack size assegnato al task
    NULL,          // Parametri
    1,             // Priorità 1
    NULL,          // Task handle
    1              // Core 1
  );

  // Crea task LED azzurro (priorità 1, stack 2048)
  xTaskCreatePinnedToCore(
    taskLedAzzurro,  // Funzione task
    "TaskLedAzzurro",   // Nome task
    2048,          // Stack size assegnato al task
    NULL,          // Parametri
    1,             // Priorità 1
    NULL,          // Task handle
    1              // Core 1
  );
}

void loop() {} // Il Loop non serve dato che viene tutto gestito dai task

// Task 1: Accende Led giallo
void taskLedVerde(void *parameter) {
  while(1) {  // Ciclo infinito                      
    xQueueReceive(q1, &segnale,portMAX_DELAY); // Attende di ricevere qualcosa dalla coda all'infinito (grazie a portMAX_DELAY)
    digitalWrite(Led_Verde, HIGH); // Accendo il led
    vTaskDelay(1000); // Attendo 1000ms
    digitalWrite(Led_Verde,LOW); // Spengo il led
    xQueueSend(q2, &segnale,0); // Aggiungo la variabile segnale alla coda 2
  }
}

// Task 2: Accende il led rosso
void taskLedRosso(void *parameter) {
  while(1) {     // Ciclo infinito                 
    xQueueReceive(q2, &segnale,portMAX_DELAY); // Attende di ricevere qualcosa dalla coda all'infinito (grazie a portMAX_DELAY)
    digitalWrite(Led_Rosso, HIGH); // Accendo il led
    vTaskDelay(1000); // Attendo 1000ms
    digitalWrite(Led_Rosso,LOW); // Spengo il led
    xQueueSend(q3, &segnale,0); // Aggiungo la variabile segnale alla coda 3
  }
}

// Task 3 : Accende entrambi i led
void taskLedAzzurro(void *parameter) {
  while(1) { // Ciclo infinito           
    xQueueReceive(q3, &segnale,portMAX_DELAY); // Attende di ricevere qualcosa dalla coda all'infinito (grazie a portMAX_DELAY)
    digitalWrite(Led_Azzurro, HIGH); // Accendo il led
    vTaskDelay(1000); // Attendo 1000ms
    digitalWrite(Led_Azzurro,LOW); // Spengo il led
    xQueueSend(q1, &segnale,0); // Aggiungo la variabile segnale alla coda 1
  }
}
