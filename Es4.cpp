#include <Arduino.h>
#include <BluetoothSerial.h>  
#include <Adafruit_BMP280.h>  
#include <Adafruit_SSD1306.h> 
#include <Wire.h>             

// Definizione dei pin I2C per l'ESP32 e parametri fisici del display
#define OLED_SDA 21           // Pin dati I2C
#define OLED_SCL 22           // Pin clock I2C
#define OLED_RST -1           // Reset del display (non usato, impostato a -1)
#define ROW 64                // Altezza del display in pixel
#define COL 128               // Larghezza del display in pixel

#define oled 0x3C             // Indirizzo I2C standard del display 

// Istanza dell'oggetto Bluetooth Classic
BluetoothSerial SerialBT;

// Inizializzazione dell'oggetto display con le dimensioni e i parametri definiti sopra
Adafruit_SSD1306 display = Adafruit_SSD1306(COL, ROW, &Wire, OLED_RST);

void setup() {
  Serial.begin(115200); // Avvio della comunicazione seriale per il debug
  
  // Inizializzazione del Bluetooth. Il nome "ESP32_Scanner" sarà visibile ad altri dispositivi
  if(!SerialBT.begin("ESP32_Scanner")){
    Serial.println("Errore nell'inizializzazione Bluetooth!");
    while(1); // Blocco infinito in caso di errore critico
  }

  // Avvio del bus I2C con i pin specificati
  Wire.begin(OLED_SDA, OLED_SCL);

  // Tentativo di connessione al display OLED. SWITCHCAPVCC genera internamente la tensione 3.3V
  while(!display.begin(SSD1306_SWITCHCAPVCC, oled)){
    Serial.println("Display initialization failed");
    // Ciclo infinito o attesa finché il display non risponde
  }

  // Configurazione estetica iniziale del display
  display.clearDisplay();           // Pulisce il buffer interno (schermo vuoto)
  display.setTextColor(WHITE);      // Imposta il colore (monocromatico: bianco su nero)
  display.setTextSize(1);           // Scala del testo minima (circa 6x8 pixel per carattere)
  display.setCursor(0,0);           // Posiziona il cursore nell'angolo superiore sinistro
  display.display();                // Invia i dati fisicamente al display

  Serial.println("Inizio scansione Bluetooth Classic...");
}

// Funzione che stampa sul display il numero di dispositivi connessi 
void stampaSuDisplay(int n) {
   display.clearDisplay();          // Cancella la vecchia schermata
   display.setCursor(0,0);          // Torna all'inizio
   display.printf("Dispositivi trovati: %d", n); // Stampa formattata (come in C)
   display.display();               // Applica le modifiche allo schermo
}

void loop() {
  Serial.println("Scansione in corso...");
  
  // Ottiene il puntatore ai risultati della scansione Bluetooth
  BTScanResults *pResults = SerialBT.getScanResults();
  
  // Avvia la scoperta fisica dei dispositivi per una durata di 5 secondi (5000ms)
  // Questa funzione è bloccante per il tempo indicato
  if (SerialBT.discover(5000)) {
    int count = pResults->getCount(); // Conta quanti dispositivi sono stati rilevati
    Serial.printf("Trovati %d dispositivi:\n", count);

    // Ciclo attraverso la lista dei dispositivi scoperti
    for (int i = 0; i < count; i++) {
      BTAdvertisedDevice *device = pResults->getDevice(i); // Estrae il singolo dispositivo
      
      int num = i + 1; // Numero progressivo per l'elenco
      
      // Recupera il nome. Se vuoto (molti dispositivi lo nascondono), assegna un segnaposto
      String nome = device->getName().c_str();
      if (nome == "") nome = "<Sconosciuto>";
      
      // Recupera l'indirizzo MAC univoco del dispositivo
      String mac = device->getAddress().toString().c_str();
      
      // Recupera l'RSSI 
      int rssi = device->getRSSI();

      // Output formattato sulla Serial Monitor per il debug
      Serial.printf("%d) Nome: %s | MAC: %s | RSSI: %d\n", num, nome.c_str(), mac.c_str(), rssi);
      
      // Aggiorna il display OLED con il conteggio corrente
      stampaSuDisplay(num);
    }
  } else {
    // Se discover restituisce false, non ci sono dispositivi o la radio è occupata
    Serial.println("Nessun dispositivo trovato o errore.");
  }
  // Pausa di 5 secondi 
  delay(5000); 
}
