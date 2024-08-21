#define LED1 4

#define RX 16
#define TX 17
#define M0 32
#define M1 33

HardwareSerial LoRaSerial(2);
 
void setup() {
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(LED1, OUTPUT);
  
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
  Serial.begin(9600);

  LoRaSerial.begin(9600, SERIAL_8N1, RX, TX);
}

void loop() {
  
  if(LoRaSerial.available() > 1){
    String input = LoRaSerial.readString();
    Serial.println(input);

    if(input == "00000,00000,00000,00000,00000,00000,00000,00000,00000,0000") {
      digitalWrite(LED1, HIGH);  
    } 
    if(input == "10000,00000,00000,00000,00000,00000,00000,00000,00000,0000") {
      digitalWrite(LED1, LOW);
    }
  }
  delay(20);
}