#include <Arduino.h>

// Dichiaro le task
void taskLedGiallo(void *parameter);
void taskLedRosso(void *parameter);

// Dichiaro i contatori delle due task
int cont1=0,cont2=0;

void setup() {
  Serial.begin(115200);
  
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
  
  // Crea task LED rosso (priorità 2, stack 2048)
  xTaskCreatePinnedToCore(
    taskLedRosso,  // Funzione task
    "TaskRosso",   // Nome task
    2048,          // Stack size assegnato al task
    NULL,          // Parametri
    2,             // Priorità 2
    NULL,          // Task handle
    1              // Core 1
  );
}

void loop() {} // Il Loop non serve dato che viene tutto gestito dai task

// Task 1: Stampa frase e accende LED giallo
void taskLedGiallo(void *parameter) {
  while(1) {                          // Ciclo infinito
    Serial.print("Task Led giallo");  // Stampo in seriale
    Serial.println(cont1);            // Stampo il contatore
    cont1++;                          // Aumento il contatore
  }
}

// Task 2: Stampa frase e accende LED rosso
void taskLedRosso(void *parameter) {
  while(1) {                      
    vTaskDelay(5);                        // Inserisco un delay di 5ms
    Serial.print("Task Led rosso");       // Stampo in seriale il nome del task
    Serial.println(cont2);                // Stampo il contatore
    cont2++;                              // Aumento il contatore
  }
}

/*
Considerazioni :
 Quello con priorità più alta viene comunque eseguito di meno dato che c'è un blocco di 5ms che permette
 al task 1 di essere eseguito molte più volte del task 2
*/
