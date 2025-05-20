// Definições de pinos
const int PIN_13 = 8;
const int PIN_12 = 7;
const int BUZZER_PIN = 6;

// Temporizadores
unsigned long startTime;
bool pin13On = false;
bool pin12On = false;

void setup() {
  Serial.begin(115200);

  pinMode(PIN_13, OUTPUT);
  pinMode(PIN_12, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(PIN_13, LOW);
  digitalWrite(PIN_12, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // Armazena o tempo de início do programa
  startTime = millis();
}

void loop() {
  unsigned long currentTime = millis();
  unsigned long elapsed = currentTime - startTime;

  Serial.println(elapsed);

  // Ativa o pino 13 após 20ms por 2 segundos
  if (elapsed >= 10000 && elapsed < 12000) {
    digitalWrite(PIN_13, HIGH);
    pin13On = true;
    Serial.println("Skib 1 ativado");
  } else {
    digitalWrite(PIN_13, LOW);
    // Serial.println("Skib 1 desativado");
    pin13On = false;
  }

  // Ativa o pino 12 após 30ms por 2 segundos
  if (elapsed >= 20000 && elapsed < 22000) {
    digitalWrite(PIN_12, HIGH);
    Serial.println("Skib 2 ativado");
    pin12On = true;
  } else {
    digitalWrite(PIN_12, LOW);
    // Serial.println("Skib 2 desativado");
    pin12On = false;
  }

  // Buzzer ligado se qualquer um dos dois pinos estiver HIGH
  if (pin13On || pin12On) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(200);
}
