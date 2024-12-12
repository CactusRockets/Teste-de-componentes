#include <WiFi.h>
#include <ESPmDNS.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>

const char* ssid = "BrenoNet";
const char* password = "breno123";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

int counter = 0;
float tempo = 0;

void onWsEvent(
  AsyncWebSocket *server, 
  AsyncWebSocketClient *client, 
  AwsEventType type, 
  void *arg, 
  uint8_t *data, 
  size_t len
) {
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

float periodicFunction1(float time, float delay) {
  return std::sin(time*3.14 + delay); // Ajuste a amplitude e período conforme necessário
}

float periodicFunction2(float time, float delay) {
  return std::cos(time*3.14 + delay); // Ajuste a amplitude e período conforme necessário
}

void setupServer(){
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
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Servidor ESP32 funcionando");
  });
}

void setup() {
  Serial.begin(115200);
  setupServer();
  server.begin();
}

void loop() {
  Serial.print("Número de Conexões: ");
  Serial.println(ws.count());

  if (ws.count() > 0) {
    StaticJsonDocument<200> doc;
    
    doc["maximumAltitude"] = 1;
    doc["altitude"] = 0.5*counter + 5;
    doc["maximumVelocity"] = 1;
    doc["velocity"] = 1;
    doc["maximumAcceleration"] = 0;
    doc["acceleration"] = 0;
    doc["velocityX"] = counter;
    doc["velocityY"] = 10*std::sin(counter*3.14/8);
    doc["velocityZ"] = 8*std::cos(counter*3.14/8);
    doc["accelerationX"] = pow(counter, 2);
    doc["accelerationY"] = 10*std::sin(counter*3.14/8) + 10*std::cos(counter*3.14/8);
    doc["accelerationZ"] = 0.1*pow(counter, 3) - 0.1*pow(counter, 2) + 100;
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
  delay(500);
}
