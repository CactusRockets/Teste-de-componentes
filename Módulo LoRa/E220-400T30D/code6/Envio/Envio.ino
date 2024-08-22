#define RX 16
#define TX 17
#define M0 18
#define M1 19

String turnOn = "10000,00000,00000,00000,00000,00000,00000,00000,00000,0000";
String turnOff = "00000,00000,00000,00000,00000,00000,00000,00000,00000,0000";

HardwareSerial loraSerial(2);

#define BMP_ADRESS 0x76

Adafruit_BMP280 bmp;

void setupBMP() {
  if(!bmp.begin(BMP_ADRESS)) {
    Serial.println("Failed to find BMP280 chip");
    while(1);
  }
  Serial.println("BMP conectado!");

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X4,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X8,     /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_1);   /* Standby time. */
}

void readBMP() {
  altitude = bmp.readAltitude(DEFAULT_PRESSURE);
  variationAltitude = bmp.readAltitude(DEFAULT_PRESSURE) - initialAltitude;

  if(altitude > maximumAltitudeValue) {
    maximumAltitudeValue = altitude;
  }
}

String getAltitudeBMP(){
  return String(bmp.readAltitude(DEFAULT_PRESSURE));
}

String getTemperatureBMP(){
  return String(bmp.readTemperature());
}

String getPressureBMP(){
  return String(bmp.readPressure());
}

void setup() {
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);

  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);

  Serial.begin(9600);
  loraSerial.begin(9600, SERIAL_8N1, RX, TX);
  setupBMP();

  initialAltitude = bmp.readAltitude(DEFAULT_PRESSURE);
  activeBuzzerStart();
}
 
void loop() {
  String messsage = String(getAltitudeBMP() + ',' 
                        + getTemperatureBMP() + ',' 
                        + getPressureBMP());
  loraSerial.print(messsage);
  Serial.println(messsage);
  delay(1250);

  loraSerial.print(messsage);
  Serial.println(messsage);
  delay(1250);
}