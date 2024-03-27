/*
Os pinos M0 e M1 do módulo LoRa foram conectados diretamente no GND
*/

#include <SoftwareSerial.h>
#define RX_PIN 2
#define TX_PIN 3

#define BUTTON 5

SoftwareSerial LoRaSerial(RX_PIN, TX_PIN);
float initialTime = 0;
float previousTime = 0;
float nextTime = 0;

bool initiated = false;
int counter = 0;
 
void setup() {
  Serial.begin(9600);

  pinMode(BUTTON, INPUT_PULLUP);
  LoRaSerial.begin(9600);
}

void loop() {
  if(digitalRead(BUTTON) == LOW) {
    initialTime = millis();
    Serial.print("Tempo inicial: ");
    Serial.println(initialTime);

    initiated = true;
  }

  if(initiated) {
    previousTime = millis();

    if(LoRaSerial.available() > 1){
      String input = LoRaSerial.readString();

      nextTime = millis();
      counter++;

      Serial.println(String(previousTime) + "," + String(nextTime) + "," + String(counter));
    }
  }
}