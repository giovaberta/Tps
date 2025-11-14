#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

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
display.clearDisplay();
display.setTextColor(WHITE);
display.setTextSize(1);
display.setCursor(0,0);
/*byte error, address;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      if(address,HEX == 1110111,HEX){
        display.printf("oled 0x");
        display.println(address,HEX);
      }
      else{
        display.printf("I2C device found at address 0x");
        display.println(address,HEX);
      }
    }  
  }*/
  display.drawLine(8,4,125,4,WHITE); // Riga Orzz superiore (x0=8,y0=4,x1=125,y1=4)
  display.drawLine(8,4,8,56,WHITE);
  display.drawLine(8,56,125,56,WHITE);
  display.drawLine(125,56,125,4,WHITE);
  display.drawLine(8,4,125,56,WHITE);
  display.drawLine(8,56,125,4,WHITE);


  display.display();
}

void loop(){

}