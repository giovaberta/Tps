#include <Arduino.h>
#include <BluetoothSerial.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RST -1
#define ROW 64
#define COL 128

#define pulsante 0
#define oled 0x3C
 
Adafruit_SSD1306 display= Adafruit_SSD1306(COL,ROW,&Wire,OLED_RST);

// Dichiaro l'oggetto bluetooth
BluetoothSerial SerialBT;
// Dichiaro le varie variabili per l'esecuzione del programma
int pres = 0;
float temp,press;
// Oggetto bmp
Adafruit_BMP280 bmp;
String name_slave = "Piva_Schiavo";

void setup() {
  Serial.begin(115200);
  // Inizializzazione del display
  Wire.begin(OLED_SDA,OLED_SCL);
  while(!display.begin(SSD1306_SWITCHCAPVCC,oled)){
    printf("Display initialization falied");
  }
  //Dichiarazioni per eventuali testi
  display.clearDisplay();
  display.setTextColor(WHITE);                      //Colore testo impostato a bianco
  display.setTextSize(1);                           // Dimensione del testo impostato a 1
  display.setCursor(0,0);                           // Inizzializzo il puntatore a codinate 0,0 ovvero l'angolo in alto a sinistra
  pinMode(pulsante,INPUT);                           
  SerialBT.begin("Esp32Berta",true);                // Do il nome al blueTooth
  Serial.println("Inizio trasmissione");            // Stampo in seriale un messaggio di verifica
  while(!bmp.begin()){                              // Controllo che il bmp sia collegato correttamente 
    printf("Errore nel bmp");                       // Se viene rilevato un problema con il bmp stampo un messaggio di errore
  }
  while(!SerialBT.connect(name_slave)){             // Mi connetto al'Esp32 di Piva 
    printf("Errore nella connesione");
    delay(200);
  }
}
void loop() {
  if(SerialBT.connected()){                         // Controllo che sia connesso
    if(digitalRead(pulsante) == 0){                 // Quando è premuto il pulsante 
      display.clearDisplay();
      display.setCursor(0,0);
      temp = bmp.readTemperature(); 
      press = bmp.readPressure();
      SerialBT.printf("Temperatura: %.2f C\n",temp);// Trasmetto i dati della temperatura al bluetooth
      SerialBT.printf("Pressione: %.2f hPa",press); // Trasmetto i dati della pressione al bluetooth
      display.printf("Temperatura: %.2f C\n",temp); // Stampo i dati sul display della temperatura
      display.printf("Pressione: %.2f hPa",press);  // Stampo i dati sul display della pressione
      display.display();
      delay(1000);                                  // Attendo 1s 
    }
  else{                                             // In caso di disconnette si tenta la riconnesione
    while(!SerialBT.connect(name_slave)){
    printf("Errore nella connesione");
    delay(200);
    }
  }
  }
  
}
