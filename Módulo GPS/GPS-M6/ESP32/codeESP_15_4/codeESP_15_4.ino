/*
  Pinagem:
  3V3:VCC
  GNG:GND
  GPIO15:TX
  GPIO4:RX
*/

#include <TinyGPS++.h>
#include <HardwareSerial.h>

static const int RXPin = 15, TXPin = 4;  // Pinos do ESP32 conectados ao módulo GPS
static const uint32_t GPSBaud = 9600;    // Taxa de transmissão do GPS NEO-6M
int gps_available = 0;

TinyGPSPlus gps;
HardwareSerial gpsSerial(1);  // Usa UART1 do ESP32

void setup() {
    Serial.begin(115200);  // Inicia a comunicação serial com o monitor serial
    gpsSerial.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin);  // Inicializa UART1 nos pinos 15 e 4

    Serial.println("Iniciando comunicação com GPS...");
}

void loop() {
    while(true) {
        gps_available = gpsSerial.available();
        // Serial.println(gps_available);

        if(gps_available <= 0) {
          break;
        }
        gps.encode(gpsSerial.read());  // Processa os dados recebidos do GPS

        if (gps.location.isUpdated()) {  // Se houver uma nova atualização de posição
            Serial.print("Latitude: ");
            Serial.print(gps.location.lat(), 6);
            Serial.print(" | Longitude: ");
            Serial.println(gps.location.lng(), 6);
        }

        if (gps.date.isUpdated()) {  // Se houver atualização da data
            Serial.print("Data: ");
            Serial.print(gps.date.day());
            Serial.print("/");
            Serial.print(gps.date.month());
            Serial.print("/");
            Serial.println(gps.date.year());
        }

        if (gps.time.isUpdated()) {  // Se houver atualização do horário
            Serial.print("Hora: ");
            Serial.print(gps.time.hour());
            Serial.print(":");
            Serial.print(gps.time.minute());
            Serial.print(":");
            Serial.println(gps.time.second());
        }

        if (gps.speed.isUpdated()) {  // Se houver atualização da velocidade
            Serial.print("Velocidade (km/h): ");
            Serial.println(gps.speed.kmph());
        }
    }
    delay(500);
}
