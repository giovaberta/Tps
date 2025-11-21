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
    printf("Display initialization falied");
  }
  display.setTextColor(WHITE); //Colore testo impostato a bianco
  display.setTextSize(1); // Dimensione del testo impostato a 1
  display.setCursor(0,0); // Inizzializzo il puntatore a codinate 0,0 ovvero l'angolo in alto a sinistra
  display.clearDisplay(); // Pulisco il display
     
}
  byte error, address;
  int nDevices = 0, ds = 0, bmp = 0,aht = 0;

void loop() {  
    for(address = 1; address < 127; address++ ) { 
      Wire.beginTransmission(address);  
      error = Wire.endTransmission();
      if (error == 0) {
        if (address<16) {
        }
        if(address == 60){
          if(ds == 0){
            display.print("oled 0x3C\n");
            ds++;
          }
        }
        if(address == 119){
          if(bmp == 0){
            display.print("bmp 0x77\n");
            bmp++;
          }
        }
        if(address == 56){
          if(aht == 0){
            display.print("aht 0x38\n");
            aht++;
          }
        }
        nDevices++;
      }  
    }
    if (nDevices == 0) {
      display.println("No I2C devices found");
    }
  display.display();
}

