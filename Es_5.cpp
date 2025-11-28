#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>

#define OLED_SDA 21 // pin SDA
#define OLED_SCL 22 // pin SCL
#define OLED_RST -1 // RESET OLED
#define ROW 64 // numero righe
#define COL 128 // numero colonne

Adafruit_SSD1306 display = Adafruit_SSD1306(COL, ROW, &Wire, OLED_RST);

// Dichiara i parametri di calibrazione del BMP280 (da leggere dal sensore)
uint16_t dig_T1, dig_T2, dig_T3;
uint16_t dig_P1, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;

int array[6];
int cont = 0;
float raw_temp = 0;
float raw_press = 0;
uint8_t p_msb, p_lsb, p_xlsb;
uint8_t t_msb, t_lsb, t_xlsb;

void setup() {
  Serial.begin(115200); // Avvia la comunicazione seriale
  Wire.begin(OLED_SDA, OLED_SCL);
  
  // Inizializzazione display
  while (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Display initialization failed!");
    delay(1000); // Attendere prima di riprovare
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  
  // Configurazione sensore
  Wire.beginTransmission(0x77);  // Invia il comando per la lettura
  Wire.write(0xF4);
  Wire.write(0x27); // Avvia la lettura di temperatura e pressione
  Wire.endTransmission(true);
}

// Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
// t_fine carries fine temperature as global value
uint32_t t_fine;
uint32_t bmp280_compensate_T_int32(uint32_t adc_T)
{
uint32_t var1, var2, T;
var1 = ((((adc_T>>3) – ((uint32_t)dig_T1<<1))) * ((uint32_t)dig_T2)) >> 11;
var2 = (((((adc_T>>4) – ((uint32_t)dig_T1)) * ((adc_T>>4) – ((uint32_t)dig_T1))) >> 12) *
((uint32_t)dig_T3)) >> 14;
t_fine = var1 + var2;
T = (t_fine * 5 + 128) >> 8;
return T;
}
// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
// Output value of “24674867” represents 24674867/256 = 96386.2 Pa = 963.862 hPa
uint32_t bmp280_compensate_P_int64(uint32_t adc_P)
{
  uint64_t var1, var2, p;
  var1 = ((uint64_t)t_fine) – 128000;
  var2 = var1 * var1 * (uint64_t)dig_P6;
  var2 = var2 + ((var1*(uint64_t)dig_P5)<<17);
  var2 = var2 + (((uint64_t)dig_P4)<<35);
  var1 = ((var1 * var1 * (uint64_t)dig_P3)>>8) + ((var1 * (uint64_t)dig_P2)<<12);
  var1 = (((((uint64_t)1)<<47)+var1))*((uint64_t)dig_P1)>>33;
  if (var1 == 0)
{
    return 0; // avoid exception caused by division by zero
  }
  p = 1048576-adc_P;
  p = (((p<<31)-var2)*3125)/var1;
  var1 = (((uint64_t)dig_P9) * (p>>13) * (p>>13)) >> 25;
  var2 = (((uint64_t)dig_P8) * p) >> 19;
  p = ((p + var1 + var2) >> 8) + (((uint64_t)dig_P7)<<4);
  return ((uint32_t)p);
}

void loop() {
  Wire.beginTransmission(0x77);
  Wire.write(0xF7); // Indirizzo di lettura dei dati
  Wire.endTransmission(true);
  
  // Richiedi 6 byte di dati (3 per la temperatura, 3 per la pressione)
  Wire.requestFrom(0x77, 6);
  
  // Controlla che ci siano abbastanza byte
  if (Wire.available() == 6) {
    // Leggi i byte e assegnali all'array
    for (int i = 0; i < 6; i++) {
      array[i] = Wire.read();
    }
    
    // Assegna i valori letti ai rispettivi byte di pressione e temperatura
    p_msb = array[0];
    p_lsb = array[1];
    p_xlsb = array[2];
    t_msb = array[3];
    t_lsb = array[4];
    t_xlsb = array[5];
    
    // Calcola la pressione e la temperatura
    raw_press = ((uint32_t)p_msb << 12) | ((uint32_t)p_lsb << 4) | (p_xlsb >> 4);
    raw_temp = ((uint32_t)t_msb << 12) | ((uint32_t)t_lsb << 4) | (t_xlsb >> 4);
    
    // Stampa solo i dati richiesti sul seriale
    Serial.print("Temperatura: ");
    Serial.print(raw_temp);
    Serial.println(" C");

    Serial.print("Pressione: ");
    Serial.print(raw_press);
    Serial.println(" hPa");
  } else {
    // Se non ricevi 6 byte, stampa un errore
    Serial.println("Errore nella lettura dei dati!");
  }
  
  // Mostra i dati sul display OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.printf("Temperatura: %.2f C\n", raw_temp);
  display.printf("Pressione: %.2f hPa\n", raw_press);
  display.display();
  
  delay(1000); // Pausa di 1 secondo
}
