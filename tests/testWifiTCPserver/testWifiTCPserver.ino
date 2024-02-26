/*#include "WiFi.h"
void setup() {
   //set the baud rate
   Serial.begin(115200);
   //set wifi to the station mode
   WiFi.mode(WIFI_STA);
   //disconnect to start again
   WiFi.disconnect();
   delay(100);
}
void loop() {
   Serial.println("starting to scan now");
   // finding the networks available nearby
   int netCount = WiFi.scanNetworks();
   Serial.println("scan completed");
   if (netCount == 0) {
      Serial.println("no network detected");
   } else {
      Serial.print(netCount);
      Serial.println(" networks found");
      for (int nn = 0; nn < netCount; ++nn) {
         //The number should start from 1 and not 0
         Serial.print(nn + 1);
         Serial.print(" ---- ");
         // Print SSID(Service Set Identifier)
         Serial.print(WiFi.SSID(nn));
         Serial.print(" ---- ");
         //Print RSSI(Received Signal Strength Indicator)
         Serial.print(WiFi.RSSI(nn));
         // print the unit
         Serial.print(" dB ---- ");
         String enType="";
         wifi_auth_mode_t encryp_type=WiFi.encryptionType(nn);
         if (encryp_type == WIFI_AUTH_WPA_WPA2_PSK){
            enType="WPA_WPA2_PSK";
         }
         else if (encryp_type == WIFI_AUTH_WPA2_ENTERPRISE){
            enType="WPA2_ENTERPRISE";
         }
         else if (encryp_type == WIFI_AUTH_OPEN){
            enType="Open";
         }
         else if (encryp_type == WIFI_AUTH_WPA_PSK){
            enType="WPA_PSK";
         }
         else if (encryp_type == WIFI_AUTH_WPA2_PSK){
            enType="WPA2_PSK";
         }
         else if (encryp_type == WIFI_AUTH_WEP){
            enType="WEP";
         }
         //display encryption type
         Serial.println(enType);
         delay(50);
      }
   }
   Serial.println("");
   // Wait and scan again
   delay(5000);
}*/


#include <WiFi.h>
#include <WebServer.h>

const char *ssid = "";
const char *password = "";

WebServer server(80);

void handleRoot() {
  server.send(200, "text/plain", "Hello, world!");
}

void setup() {
  Serial.begin(115200);

  // Connessione Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connessione al WiFi...");
  }
  Serial.println("Connesso al WiFi");

  // Stampa l'indirizzo IP assegnato all'ESP32
  Serial.print("Indirizzo IP: ");
  Serial.println(WiFi.localIP());

  // Definizione della route HTTP
  server.on("/", HTTP_GET, handleRoot);

  // Inizializzazione del server
  server.begin();
}

void loop() {
  // Gestione delle richieste HTTP
  server.handleClient();
}
