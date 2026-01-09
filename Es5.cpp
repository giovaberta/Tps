#include <Arduino.h>

/*
Obbiettivo:
  Stampare in seriale il numero di volte che il pulsante viene premuto, con l'utilizzo di un semaforo
*/

#define DefaultBut 0 // Definisco il pin 0
int z = 0; // Variabile globale contatore

SemaphoreHandle_t sembin; // Handle per la gestione del semaforo binario

bool ButValue = HIGH;      // Stato attuale del pulsante
bool ButPrevValue = HIGH;  // Stato precedente del pulsante

// Creazione delle task
void taskOne(void *par); 
void taskTwo(void *par); 

void setup() {
  Serial.begin(9600); // Inizializzazione della seriale
  pinMode(DefaultBut, INPUT); // Configurazione del pin del pulsante come ingresso

  sembin = xSemaphoreCreateBinary(); // Creazione del semaforo binario

  // Creazione della prima task:
  xTaskCreate(taskOne, "taskOne", 10000, NULL, 1, NULL);

  // Creazione della seconda task:
  xTaskCreate(taskTwo, "taskTwo", 10000, NULL, 1, NULL);
}

void loop() {} // La funzione loop rimane vuota, la logica è gestita dai task RTOS

void taskOne(void *par) {
  while (1) {
    // Blocca l'esecuzione della task finché non ottiene il semaforo.
    xSemaphoreTake(sembin, portMAX_DELAY);

    Serial.printf("premuto %d volte \n", z); // Stampa il valore corrente del contatore

    vTaskDelay(50); // Breve ritardo 
  }
}

void taskTwo(void *par) {
  while (1) {
    // Loop infinito per monitorare il pulsante
    ButValue = digitalRead(DefaultBut); // Legge lo stato attuale del pulsante

    // Rileva sel il pulsante passa da non premuto a premuto (fronte di discesa)
    if ((ButValue == LOW) && (ButPrevValue == HIGH)) {
      z++; // Incrementa il contatore
      xSemaphoreGive(sembin); // Rilascia il semaforo, sbloccando taskOne
    }
    ButPrevValue = ButValue; // Aggiorna lo stato precedente per il prossimo ciclo

    vTaskDelay(50); // Breve ritardo 
  }
}
