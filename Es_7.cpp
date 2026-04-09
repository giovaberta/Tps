#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#define Pulsante 0 // pin pulsante

#define OLED 0x3C // pin display

#define OLED_SDA 21 // pin SDA
#define OLED_SCL 22 // pin SCL
#define OLED_RST -1 // RESET OLED
#define ROW 64 // numero righe
#define COL 128 // numero colonne

// Dichiaro variabili interne
int minut = 0;
int sec = 0;
bool premuto = false;
hw_timer_t *timer0 = NULL;

// Dichiaro una macchina a stati con 3 stati
typedef enum Stato{
  S1,S2,S3
}State;
State ora = S1;

// Interrupt timer0
void IRAM_ATTR lampeggio0() {
  // Incremento i secondi ogni secondo
  sec++;
}

// Funzione di interrupt per il pulsante
void IRAM_ATTR isr() {
  // Se premo il pulsante setto la variabile in true
  premuto = true;
}

Adafruit_SSD1306 display= Adafruit_SSD1306(COL,ROW,&Wire,OLED_RST); // Oggetto che controlla il display

void setup() {
  Serial.begin(115200); // Inizializzo la seriale
  pinMode(Pulsante, INPUT); // Inizializzo il pulsante in input
  // Inizializzazione del display
  Wire.begin(OLED_SDA,OLED_SCL);
  while (!display.begin(SSD1306_SWITCHCAPVCC,OLED)){
    printf("display instalization failed");
    }
  display.setTextColor(WHITE); //Colore testo impostato a bianco
  display.setTextSize(1); // Dimensione del testo impostato a 1
  display.setCursor(0,0); // Inizializzo il puntatore a coordinate 0,0 ovvero l'angolo in alto a sinistra
  display.clearDisplay(); // Pulisco il display
 
  // Inizializzazione del timer0
  timer0 = timerBegin(0, 80, true);
  timerAttachInterrupt(timer0, &lampeggio0, true);
  timerAlarmWrite(timer0, 1000000, true); // Attiva interrupt ogni 1 secondo
  timerAlarmEnable(timer0);

  // Inizializzazione del Interrupt
  attachInterrupt(Pulsante, isr,FALLING);
  }

void loop() {
  // Macchina a stati
  switch (ora)
  {
  case S1: // S1 = stato azzerato
    minut = 0;
    sec = 0;
    display.setCursor(0,0);
    display.clearDisplay();
    display.printf("minuti: %d secondi: %d",minut,sec);
    display.display();
    if(premuto){
      ora = S2;
      sec = 0;
      minut = 0;
      premuto = false;
    }
  break;

  case S2: // S2 = stato conteggio
    // Se i secondi arrivano a 60 allora aggiungo un minuto e azzero i secondi
    if (sec == 60){
      sec = 0;
      minut++;
    }
    display.setCursor(0,0);
    display.clearDisplay();
    display.printf("minuti: %d secondi: %d",minut,sec);
    display.display();
    if(premuto){
      ora = S3;
      premuto = false;
    }
  break;

  case S3: // S3 = è uno stato che resta fermo fino a quando non si preme nuovamente il pulsante
    if(premuto){
      ora = S1;
      premuto = false;
    }
  break;
  }
}
