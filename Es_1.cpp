#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>

#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RST -1
#define ROW 64
#define COL 128

Adafruit_SSD1306 display= Adafruit_SSD1306(COL,ROW,&Wire,OLED_RST);


void setup() {
  Wire.begin(OLED_SDA,OLED_SCL);
  while(!display.begin(SSD1306_SWITCHCAPVCC,0x3C)){
    printf("Display initialization failed");
  }
  display.setTextColor(WHITE); //Colore testo impostato a bianco
  display.setTextSize(1); // Dimensione del testo impostato a 1
  display.setCursor(0,0); // Inizzializzo il puntatore a codinate 0,0 ovvero l'angolo in alto a sinistra
  display.clearDisplay(); // Pulisco il display
     
}
  byte error, address; 
  int nDevices = 0;
  int ds = 0, bmp = 0,aht = 0; // Variabili per far stampare una sola valota 

void loop() {  
  // Eseguo una scansione dei dispositivi I2C collegati
    for(address = 1; address < 127; address++ ) { 
      Wire.beginTransmission(address);  
      error = Wire.endTransmission();
      if (error == 0) {
        // I primi 16 address sono riservati
        if (address<16) {
        }
        // Se il dispositivo rilevato ha address == 60
        if(address == 60){
          // Corrisponde al display Oled
          if(ds == 0){
            display.print("oled 0x3C\n");
            ds++;
          }
        }
        // Se il dispositivo rilevato ha address == 119
        if(address == 119){
          // Corrisponde al sensore bmp
          if(bmp == 0){
            display.print("bmp 0x77\n");
            bmp++;
          }
        }
        // Se il dispositivo rilevato ha address == 56
        if(address == 56){
          // Corrisponde al sensore aht
          if(aht == 0){
            display.print("aht 0x38\n");
            aht++;
          }
        }
        nDevices++;
      }  
    }
  // Se non ci sono dispositivi collegati do l'errore
    if (nDevices == 0) {
      display.println("No I2C devices found");
    }
  display.display();
}

