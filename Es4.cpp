#include <Arduino.h>

#define Led_Giallo 26 // Il led giallo è assegnato al pin 33
#define Led_Rosso 25 // Il led rosso è assegnato al pin 32

// Dichiaro le task
void taskLedGiallo(void *parameter);
void taskLedRosso(void *parameter);
void taskLedAccesi(void *parameter);


void setup() {
  Serial.begin(115200);
  
  // Configura pin LED come output
  pinMode(Led_Rosso, OUTPUT);
  pinMode(Led_Giallo, OUTPUT);
  // Spengo i led
  digitalWrite(Led_Rosso, LOW);
  digitalWrite(Led_Giallo, LOW);

  // Crea task LED giallo (priorità 10, stack 2048)
  xTaskCreatePinnedToCore(
    taskLedGiallo,    // Funzione task
    "taskLedGiallo",  // Nome task
    2048,             // Stack size assegnato al task
    NULL,             // Parametri
    10,                // Priorità 10
    NULL,             // Task handle
    1                 // Core 1
  );
  
  // Crea task LED rosso (priorità 5, stack 2048)
  xTaskCreatePinnedToCore(
    taskLedRosso,  // Funzione task
    "TaskRosso",   // Nome task
    2048,          // Stack size assegnato al task
    NULL,          // Parametri
    5,             // Priorità 5
    NULL,          // Task handle
    1              // Core 1
  );

  // Crea task LED Accesi che accende entrambi i led (priorità 1, stack 2048)
  xTaskCreatePinnedToCore(
    taskLedAccesi,  // Funzione task
    "TaskLedAccesi",   // Nome task
    2048,          // Stack size assegnato al task
    NULL,          // Parametri
    1,             // Priorità 1
    NULL,          // Task handle
    1              // Core 1
  );
}

void loop() {} // Il Loop non serve dato che viene tutto gestito dai task

// Task 1: Accende Led giallo
void taskLedGiallo(void *parameter) {
  while(1) {  // Ciclo infinito                      
    digitalWrite(Led_Rosso ,LOW); 
    digitalWrite(Led_Giallo, HIGH);
    vTaskDelay(1000);
    digitalWrite(Led_Giallo ,LOW);
    vTaskDelay(2001);
  }
}

// Task 2: Accende il led rosso
void taskLedRosso(void *parameter) {
  while(1) {     // Ciclo infinito                 
    vTaskDelay(1001);
    digitalWrite(Led_Giallo ,LOW);
    digitalWrite(Led_Rosso, HIGH);
    vTaskDelay(1000);
    digitalWrite(Led_Rosso ,LOW);
    vTaskDelay(1000);
  }
}

// Task 3 : Accende entrambi i led
void taskLedAccesi(void *parameter) {
  while(1) { // Ciclo infinito           
    vTaskDelay(2001);
    digitalWrite(Led_Giallo, HIGH);
    digitalWrite(Led_Rosso, HIGH);
    vTaskDelay(1000);
    digitalWrite(Led_Rosso ,LOW);

  }
}
