#include <Arduino.h>
#include <WiFi.h>
#include <Adafruit_SSD1306.h>

#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RST -1
#define ROW 64
#define COL 128

#define OLED 0x3C

Adafruit_SSD1306 display= Adafruit_SSD1306(COL,ROW,&Wire,OLED_RST);

void setup() {
  Serial.begin(115200);
  WiFi.begin("TPSITquinteBis","quintaemme");
  while(WiFi.status() != WL_CONNECTED){
    display.clearDisplay();
    display.setCursor(0,0);
    Serial.print("Connessione....");
    display.print("Connessione....");
    display.display();
    delay(1000);
    WiFi.begin("TPSITquinteBis","quintaemme");
  }
 while(!display.begin(SSD1306_SWITCHCAPVCC,OLED)){
   printf("Display initialization falied"); //In caso qualcosa non andasse a buon fine
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
  display.print(WiFi.SSID());
  display.print(WiFi.localIP());
  Serial.print(WiFi.SSID());
  Serial.println(WiFi.localIP());
  display.display();
  display.clearDisplay();
  display.setCursor(0,0);
  delay(1000);
}

