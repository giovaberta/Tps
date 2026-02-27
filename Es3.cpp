#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Ping.h>

const char* ssid = "TPSITquinteBis";
const char* password = "quintaemme";
int nWifi ;

String CriptType(wifi_auth_mode_t encryptionType) {
  switch (encryptionType) {
    // Macchina a stati che riconosce la crittografia
    case WIFI_AUTH_OPEN:            return "OPEN (nessuna crittografia)";
    case WIFI_AUTH_WEP:             return "WEP";             //WPe
    case WIFI_AUTH_WPA_PSK:         return "WPA-PSK";         //WPA
    case WIFI_AUTH_WPA2_PSK:        return "WPA2-PSK";        //WPA2
    case WIFI_AUTH_WPA_WPA2_PSK:    return "WPA/WPA2-PSK";    //WPA/WPA2
    case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2-Enterprise"; //WPA2 Enterprise
    case WIFI_AUTH_WPA3_PSK:        return "WPA3-PSK";        //WPA3
    case WIFI_AUTH_MAX:             return "Auth_Max";        //MAX
    default:                        return "SCONOSCIUTA";     //Sconosciuta
  }
}

void setup() {
  Serial.begin(115200);
  // Eseguo la connessione al wifi
  WiFi.begin(ssid,password);
  Serial.print("Connessione in corso");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.print("\n");
}

void loop() {
  // Eseguo una scansione delle reti
  Serial.println("Scansione reti Wi-Fi...");
  int numReti = WiFi.scanNetworks();
  // In caso non trovo reti
  if (numReti == 0) {
    Serial.println("Nessuna rete trovata.");
  } else {
    // Stampo quante reti ho trovate
    Serial.printf("---------\nTrovate %d reti:\n---------\n", numReti);
    // Scorro le varie reti
    for (int i = 0; i < numReti; i++) {
      // recupero i dati necessari per esegguire la stampa in seguito
      String nomeSsid   = WiFi.SSID(i);
      int    rssi       = WiFi.RSSI(i);
      String encryption = CriptType(WiFi.encryptionType(i));
      // Stampo in seriale la rete con il some l'rssi e la tipologia di crittografia
      Serial.printf("[%2d] SSID: %-20s | RSSI: %d dBm \t| Crittografia: %s\n",i + 1,nomeSsid.c_str(),rssi,encryption.c_str());
      if (nomeSsid == String(ssid)) {
        nWifi = i;
      }
    }
    Serial.printf(">> Connesso a: %s\n", ssid);
    Serial.printf(">> Crittografia in uso: %s\n", CriptType(WiFi.encryptionType(nWifi)));
  }
  Serial.println("---------");
  delay(10000); // Attende 10 secondi prima del prossimo scan
}
