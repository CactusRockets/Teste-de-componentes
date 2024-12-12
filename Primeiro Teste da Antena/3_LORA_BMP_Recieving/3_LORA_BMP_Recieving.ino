#include <WiFi.h>
#include <ESPmDNS.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>

#define LED1 5
#define RX 16
#define TX 17
#define M0 32
#define M1 33

HardwareSerial LoRaSerial(2);

const char *ssid = "BrenoNet";
const char *password = "breno123";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

float counter = 0;
float tempo = 0;

void onWsEvent(
  AsyncWebSocket *server,
  AsyncWebSocketClient *client,
  AwsEventType type,
  void *arg,
  uint8_t *data,
  size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.println("Cliente conectado");
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.println("Cliente desconectado");
  } else if (type == WS_EVT_DATA) {
    Serial.print("Dados recebidos: ");
    Serial.write(data, len);
    Serial.println();
  }
}

void setupServer() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (!MDNS.begin("esp32")) {
    Serial.println("Erro ao configurar mDNS");
    return;
  }
  Serial.println("mDNS configurado como esp32.local");

  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  // Adicione uma rota básica para verificar a conexão HTTP
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Servidor ESP32 funcionando");
  });

  server.begin();
}

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
  Serial.begin(9600);

  LoRaSerial.begin(9600, SERIAL_8N1, RX, TX);

  setupServer();

  bool messageRecieved = false;
}

void debugWS() {
  Serial.print("Número de Conexões: ");
  Serial.println(ws.count());

  if (ws.count() > 0) {
    StaticJsonDocument<200> doc;

    doc["maximumAltitude"] = 1;
    doc["altitude"] = 0.5 * counter + 5;
    doc["maximumVelocity"] = 1;
    doc["velocity"] = 1;
    doc["maximumAcceleration"] = 0;
    doc["acceleration"] = 0;
    doc["velocityX"] = counter;
    doc["velocityY"] = 10 * std::sin(counter * 3.14 / 8);
    doc["velocityZ"] = 8 * std::cos(counter * 3.14 / 8);
    doc["accelerationX"] = pow(counter, 2);
    doc["accelerationY"] = 10 * std::sin(counter * 3.14 / 8) + 10 * std::cos(counter * 3.14 / 8);
    doc["accelerationZ"] = 0.1 * pow(counter, 3) - 0.1 * pow(counter, 2) + 100;
    doc["latitude"] = -9.381354 + tempo;
    doc["longitude"] = -40.543419 + tempo;
    doc["skib1"] = true;
    doc["skib2"] = true;

    counter++;
    tempo = tempo + 0.01;
    if (counter > 31) counter = 0;

    String json;
    serializeJson(doc, json);
    ws.textAll(json);
    Serial.print("Enviando dados: ");
    Serial.println(json);
  }
}

String *dividirString(String inputString, char delimitador) {
  // Contar o número de segmentos
  int contador = 1;
  for (int i = 0; i < inputString.length(); i++) {
    if (inputString[i] == delimitador) {
      contador++;
    }
  }

  // Alocar memória para o vetor de segmentos
  String *resultado = new String[contador];

  // Dividir a string e armazenar nos segmentos
  int indice = 0;
  int posicaoAnterior = 0;
  int posicaoAtual = inputString.indexOf(delimitador);

  while (posicaoAtual != -1) {
    resultado[indice++] = inputString.substring(posicaoAnterior, posicaoAtual);
    posicaoAnterior = posicaoAtual + 1;
    posicaoAtual = inputString.indexOf(delimitador, posicaoAnterior);
  }

  // Armazenar o último segmento
  resultado[indice] = inputString.substring(posicaoAnterior);

  return resultado;
}

void loop() {
  Serial.println(LoRaSerial.available());
  if (LoRaSerial.available() > 1) {
    String input = LoRaSerial.readString();
    Serial.println(input);

    // String* inputSegmented = dividirString(input, ',');

    // if(input == "00000,00000,00000,00000,00000,00000,00000,00000,00000,0000") {
    //   digitalWrite(LED1, HIGH);
    // }
    // if(input == "10000,00000,00000,00000,00000,00000,00000,00000,00000,0000") {
    //   digitalWrite(LED1, LOW);
    // }

    if (ws.count() > 0) {
      StaticJsonDocument<200> doc;

      // doc["maximumAltitude"] = 1;
      // doc["altitude"] = input;
      // doc["maximumVelocity"] = 1;
      // doc["velocity"] = 1;
      // doc["maximumAcceleration"] = 0;
      // doc["acceleration"] = 0;
      // doc["velocityX"] = counter;
      // doc["velocityY"] = 10 * std::sin(counter * 3.14 / 8);
      // doc["velocityZ"] = 8 * std::cos(counter * 3.14 / 8);
      // doc["accelerationX"] = pow(counter, 2);
      // doc["accelerationY"] = 10 * std::sin(counter * 3.14 / 8) + 10 * std::cos(counter * 3.14 / 8);
      // doc["accelerationZ"] = 0.1 * pow(counter, 3) - 0.1 * pow(counter, 2) + 100;
      // doc["latitude"] = -9.381354 + tempo;
      // doc["longitude"] = -40.543419 + tempo;
      // doc["skib1"] = true;
      // doc["skib2"] = true;

      doc["maximumAltitude"] = 1;
      doc["altitude"] = input;
      doc["maximumVelocity"] = 1;
      doc["velocity"] = 1;
      doc["maximumAcceleration"] = 0;
      doc["acceleration"] = 0;
      doc["velocityX"] = 0;
      doc["velocityY"] = 10;
      doc["velocityZ"] = 8;
      doc["accelerationX"] = 0;
      doc["accelerationY"] = 10;
      doc["accelerationZ"] = 0.1;
      doc["latitude"] = -9.381354;
      doc["longitude"] = -40.543419;
      doc["skib1"] = true;
      doc["skib2"] = true;

      counter++;
      tempo = tempo + 0.01;
      if (counter > 31) counter = 0;

      String json;
      serializeJson(doc, json);
      ws.textAll(json);
      Serial.print("Enviando dados: ");
      Serial.println(json);
    }
  }
  delay(50);
}