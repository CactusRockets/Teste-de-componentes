#define RX 16
#define TX 17
#define M0 21
#define M1 22

String turnOn = "10000,00000,00000,00000,00000,00000,00000,00000,00000,0000";
String turnOff = "00000,00000,00000,00000,00000,00000,00000,00000,00000,0000";

HardwareSerial loraSerial(2);

void setup() {
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);

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