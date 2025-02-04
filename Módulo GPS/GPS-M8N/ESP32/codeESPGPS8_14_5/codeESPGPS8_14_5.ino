#include <TinyGPS++.h>
#include <HardwareSerial.h>

static const int RXPin = 15, TXPin = 4;  // Pinos ESP32 conectados ao GPS
static const uint32_t GPSBaud = 9600;  // Tente 9600, 38400 ou 115200 para o NEO-N8N

TinyGPSPlus gps;
HardwareSerial gpsSerial(1);

void setup() {
    Serial.begin(115200);
    gpsSerial.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin);  // Inicializa UART1 com a taxa configurada

    Serial.println("Iniciando comunicação com GPS NEO-N8N...");
}

void loop() {
    while (gpsSerial.available() > 0) {
        gps.encode(gpsSerial.read());  // Processa os dados recebidos do GPS

        if (gps.location.isUpdated()) {  // Nova posição disponível
            Serial.print("Latitude: ");
            Serial.print(gps.location.lat(), 6);
            Serial.print(" | Longitude: ");
            Serial.println(gps.location.lng(), 6);
        }

        if (gps.date.isUpdated()) {  // Atualização da data
            Serial.print("Data: ");
            Serial.print(gps.date.day());
            Serial.print("/");
            Serial.print(gps.date.month());
            Serial.print("/");
            Serial.println(gps.date.year());
        }

        if (gps.time.isUpdated()) {  // Atualização do horário
            Serial.print("Hora: ");
            Serial.print(gps.time.hour());
            Serial.print(":");
            Serial.print(gps.time.minute());
            Serial.print(":");
            Serial.println(gps.time.second());
        }

        if (gps.speed.isUpdated()) {  // Atualização da velocidade
            Serial.print("Velocidade (km/h): ");
            Serial.println(gps.speed.kmph());
        }
    }
}
