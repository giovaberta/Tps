#include <Arduino.h>

#define Led_Giallo 33 // Il led giallo è assegnato al pin 33
#define Led_Rosso 32 // Il led rosso è assegnato al pin 32

// Dichiaro le task
void taskLedGiallo(void *parameter);
void taskLedRosso(void *parameter);

// Dichiare le variabili che utilizzo nel task per invertire lo stato dei led
bool StatoG = false,StatoR = false;

void setup() {
  Serial.begin(115200);
  
  // Configura pin LED come output
  pinMode(Led_Rosso, OUTPUT);
  pinMode(Led_Giallo, OUTPUT);
  
  // Spengo i led
  digitalWrite(Led_Rosso, LOW);
  digitalWrite(Led_Giallo, LOW);
  
  // Crea task LED giallo (priorità 1, stack 2048)
  xTaskCreatePinnedToCore(
    taskLedGiallo,    // Funzione task
    "taskLedGiallo",  // Nome task
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
    0              // Core 0
  );
}

void loop() {} // Il Loop non serve dato che viene tutto gestito dai task

// Task 1: Stampa frase e accende LED giallo
void taskLedGiallo(void *parameter) {
  vTaskDelay(300);                                                  // 300ms di delay (NON bloccante) per alternare i led
  while(1) {                                                        // Ciclo infinito
    Serial.println("Task Led giallo attivo - Led giallo acceso!");  // Stampo in seriale
    StatoG = !StatoG;                                               // Inverto lo stato del led
    vTaskDelay(300);                                                // 300ms di delay (NON bloccante per il sistema)
    digitalWrite(Led_Giallo, StatoG);                               // Accende LED giallo
  }
}

// Task 2: Stampa frase e accende LED rosso
void taskLedRosso(void *parameter) {
  while(1) {                                                        // Ciclo infinito
    Serial.println("Task Led rosso attivo - Led rosso acceso!");    // Stampo in seriale
    StatoR = !StatoR;                                               // Inverto lo stato del led
    vTaskDelay(300);                                                // 300ms di delay (NON bloccante per il sistema)
    digitalWrite(Led_Rosso, StatoR);                                // Spegne LED rosso
  }
}
