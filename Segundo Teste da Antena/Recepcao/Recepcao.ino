#define LED1 4

#define RX 16
#define TX 17
#define M0 32
#define M1 33

HardwareSerial LoRaSerial(2);
 
int counter = 0;

void setup() {
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(LED1, OUTPUT);
  
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
  Serial.begin(9600);

  LoRaSerial.begin(9600, SERIAL_8N1, RX, TX);

  // Habilitar RSSI
  sendCommand("C0 04 01 20"); // Habilita RSSI ambiente
  sendCommand("C0 06 01 80"); // Habilita RSSI para dados recebidos
}

void loop() {
  if (LoRaSerial.available() > 1) {
    String input = LoRaSerial.readString();
    Serial.print(counter++);
    Serial.print(" : ");
    Serial.println(input);

    // Obter RSSI dos dados recebidos
    String rssi = readRSSI();
    Serial.print("RSSI (dBm): ");
    Serial.println(rssi);

    if (input == "00000,00000,00000,00000,00000,00000,00000,00000,00000,0000") {
      digitalWrite(LED1, HIGH);  
    } 
    if (input == "10000,00000,00000,00000,00000,00000,00000,00000,00000,0000") {
      digitalWrite(LED1, LOW);
    }
  }
  delay(20);
}

void sendCommand(String cmd) {
  LoRaSerial.println(cmd);
  delay(50);
}

String readRSSI() {
  sendCommand("C1 00 01"); // Comando para obter RSSI do canal
  delay(50);

  String response = "";
  while (LoRaSerial.available()) {
    response += String(LoRaSerial.read(), HEX);
  }
  
  int rssi = response.toInt();
  return String(-rssi / 2) + " dBm";
}
