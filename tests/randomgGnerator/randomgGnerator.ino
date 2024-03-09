#include "esp_random.h"
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  //Serial.println(esp_random()%2);
  //Serial.println((float)(esp_random()%100)/100);
  Serial.println(esp_random()%4);
  delay(500);

}
