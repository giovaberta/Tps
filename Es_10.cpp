#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>

#define OLED 0x3C // pin display


#define OLED_SDA 21 // pin SDA
#define OLED_SCL 22 // pin SCL
#define OLED_RST -1 // RESET OLED
#define ROW 64 // numero righe
#define COL 128 // numero colonne

// Oggetto display
Adafruit_SSD1306 display = Adafruit_SSD1306(COL, ROW, &Wire, OLED_RST);

void setup() {
  Serial.begin(115200); // Inizializzo la seriale
  // Inizializzazione del dispaly
  Wire.begin(OLED_SDA,OLED_SCL);
  while (!display.begin(SSD1306_SWITCHCAPVCC,OLED)){
    printf("display instalization failed");
    }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.display();
  
}
void loop() {
    int rip = 40;
    for(int cont = 0;cont<40;cont+=4){
      rip = rip - 4;
      display.clearDisplay();
      display.drawCircle(64,32,cont,WHITE);
      display.drawCircle(64,32,rip,WHITE);
      display.display();
      delay(50);
    }
}
