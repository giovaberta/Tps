#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>

#define OLED_SDA 21 // pin SDA
#define OLED_SCL 22 // pin SCL
#define OLED_RST -1 // RESET OLED
#define ROW 64 // numero righe
#define COL 128 // numero colonne

Adafruit_SSD1306 display = Adafruit_SSD1306(COL, ROW, &Wire, OLED_RST);

int array[6];
int cont = 0;
float  temp=0, press = 0;
void setup() {
  Serial.begin(115200);
  Wire.begin(OLED_SDA,OLED_SCL);
  while (!display.begin(SSD1306_SWITCHCAPVCC,0x3C)){
    printf("display instalization failed");
    }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  }


void loop() {
  Wire.beginTransmission(0x77);
  Wire.write(0xF7);
  Wire.endTransmission(true);
  Wire.requestFrom(0x77, 6);
  while(Wire.available()) {
    array[cont]=Wire.read();
    cont+=1;
    }
  cont = 0;
  display.clearDisplay();
  press=(uint32_t) ((array[2]+(array[1]<<8)+(array[0]<<16))>>4)*800/1048575.0+300;
  temp=(uint32_t)((array[5]+(array[4]<<8)+(array[3]<<16))>>4)*125/1048575.0-40;


  Serial.printf("temperatura: %.2f°C\n", temp);
  Serial.printf("pressione: %.2fhpa\n", press);

  display.setCursor(0,0);
  display.printf("temperatura: %.2f C\n", temp);
  display.printf("pressione: %.2fhpa\n", press);
  display.display();  
  
  delay(1000);
}
