#define RX 16
#define TX 17
#define M0 32
#define M1 33

String turnOn = "00000,00000,00000,00000,00000,00000,00000,00000,00000,0000";
String turnOff = "10000,00000,00000,00000,00000,00000,00000,00000,00000,0000";

HardwareSerial loraSerial(2);

void setup() {

  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);

  Serial.begin(9600);
  loraSerial.begin(9600, SERIAL_8N1, RX, TX);
}
 
void loop() {
  loraSerial.print(turnOn);
  Serial.println(turnOn);
  delay(1250);

  loraSerial.print(turnOff);
  Serial.println(turnOff);
  delay(1250);
}