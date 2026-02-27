#include <Arduino.h>
#include <WiFi.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Ping.h>

#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RST -1
#define ROW 64
#define COL 128

#define OLED 0x3C

IPAddress ip;
int BestId;
int BestRSSI = -999;

Adafruit_SSD1306 display= Adafruit_SSD1306(COL,ROW,&Wire,OLED_RST);

void setup() {
  Serial.begin(115200);
  while(!display.begin(SSD1306_SWITCHCAPVCC,OLED)){
   printf("Display initialization falied"); //In caso qualcosa non andasse a buon fine
 }
  WiFi.begin("TPSITquinteBis","quintaemme");
  while(WiFi.status() != WL_CONNECTED){
    display.clearDisplay();
    display.setCursor(0,0);
    Serial.print("Connessione....");
    display.print("Connessione....");
    display.display();
    delay(1000);
  }
  display.clearDisplay(); // Pulisco lo scermo preventivamente all'inizio del codice
 //Dichiarazioni per eventuali testi (in questo caso non utilizzati)
 display.setTextColor(WHITE); //Colore testo impostato a bianco
 display.setTextSize(1); // Dimensione del testo impostato a 1
 display.setCursor(0,0); // Inizzializzo il puntatore a codinate 0,0 ovvero l'angolo in alto a sinistra
 // Scrivo tre parole con dimensioni diverse una sotto l'altra
 display.display(); // Mostro sul dispaly quello che ho caricato in memoria
}  
                 

void loop() {
  display.clearDisplay();
  display.setCursor(0,0);
  bool ping = Ping.ping("192.168.114.106",3); // Eseguo tre ping all'ip assegnato al Pc della scuola
  ip = WiFi.localIP();                        // Salvo l'ip che il mikrotic mi ha assegnato 
  // Esegue le spampe in seriale e sul display
  Serial.print("Ip: ");
  Serial.println(ip);
  display.printf("Ip: ");
  display.println(ip);
  // Stampo due messaggi differenti in base all'esito del ping
  if(ping){
    Serial.println("Ping eseguito con successo all'ip : 192.168.114.106");
    display.println("Ping eseguito con successo all'ip : 192.168.114.106");
  }
  else{
    Serial.println("Ping non andato a buon fine");
    display.println("Ping non andato a buon fine");
  }
  display.display();
  int scan = WiFi.scanNetworks(); // Esegue uno scan per vedere quante reti ci sono
  Serial.printf("\n----\nReti trovate: %d\n----\n",scan);
  for(int i =0; i < scan; i++){
      int rssi = WiFi.RSSI(i); // Per ogni rete prendo l'rssi
      Serial.printf("[%d] SSID: %s  RSSI: %d dBm\n",i + 1, WiFi.SSID(i).c_str(), rssi); // Stampo la rete con ssid e rssi 
      if(rssi > BestRSSI){ // Verifico se l'rssi è migliore rispetto al precedente
        BestRSSI = rssi;
        BestId = i;
      }
  }
  // Stampo la migliore rete con rssi più alto
  Serial.printf("Rete migliore: %s, con RSSI: %d\n",WiFi.SSID(BestId).c_str(),BestRSSI);
  display.printf("Rete migliore: %s, con RSSI: %d",WiFi.SSID(BestId).c_str(),BestRSSI);
  display.display();
  delay(10000);
}
