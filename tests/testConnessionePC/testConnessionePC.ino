#include <WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "";
const char *password = "";

WiFiUDP Udp;

void setup() {
  Serial.begin(115200);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Start the server
  Serial.println("Server started");
  // Print the IP address on serial monitor
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");  //URL IP to be typed in mobile/desktop browser
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop() {
  // Leggi i dati dal tuo sensore o da altre sorgenti
  float temperatura = 2;

  // Invia i dati al tuo computer
  inviaDati(temperatura);
  Serial.println("x");

  delay(1000); // Puoi regolare la frequenza di invio dei dati
}

void inviaDati(float dato) {
  Udp.beginPacket("192.168.1.3", 2025);
  Udp.print(dato);
  Udp.endPacket();
}
