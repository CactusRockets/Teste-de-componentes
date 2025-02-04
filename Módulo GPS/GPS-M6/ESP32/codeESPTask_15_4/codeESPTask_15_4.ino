/*
  Pinagem:
  3V3:VCC
  GNG:GND
  GPIO15:TX
  GPIO4:RX
*/


#include <TinyGPS++.h>
#include <HardwareSerial.h>

#define RXPin 15
#define TXPin 4
#define GPSBaud 9600

TinyGPSPlus gps;
HardwareSerial gpsSerial(1);
TaskHandle_t TaskGPS; // Handle da task

void processGPS(void *pvParameters) {
    while (1) { 
        if (gpsSerial.available()) { 
            while (gpsSerial.available()) {
                gps.encode(gpsSerial.read());
            }

            // Se uma nova posição foi atualizada, imprime os dados
            if (gps.location.isUpdated()) {
                Serial.print("Latitude: ");
                Serial.print(gps.location.lat(), 6);
                Serial.print(" | Longitude: ");
                Serial.println(gps.location.lng(), 6);
            }

            if (gps.date.isUpdated()) {
                Serial.print("Data: ");
                Serial.print(gps.date.day());
                Serial.print("/");
                Serial.print(gps.date.month());
                Serial.print("/");
                Serial.println(gps.date.year());
            }

            if (gps.time.isUpdated()) {
                Serial.print("Hora: ");
                Serial.print(gps.time.hour());
                Serial.print(":");
                Serial.print(gps.time.minute());
                Serial.print(":");
                Serial.println(gps.time.second());
            }

            if (gps.speed.isUpdated()) {
                Serial.print("Velocidade (km/h): ");
                Serial.println(gps.speed.kmph());
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10)); // Aguarda 100ms antes de verificar novamente
    }
}

void setup() {
    Serial.begin(115200);
    gpsSerial.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin);
    
    Serial.println("Iniciando comunicação com GPS...");

    // Criando a Task para processar os dados do GPS
    xTaskCreatePinnedToCore(
        processGPS,    // Função da Task
        "TaskGPS",     // Nome da Task
        4096,          // Tamanho da stack
        NULL,          // Parâmetro passado
        1,             // Prioridade da Task
        &TaskGPS,      // Handle da Task
        0              // Core 0 para execução
    );
}

void loop() {
    // O loop principal pode executar outras funções sem interferir no GPS
    Serial.println("Main Loop rodando...\n");
    delay(2000);
}
