#include <avr/io.h>

#define RX_PIN 2
#define TX_PIN 3

#define BUTTON 5

String turnOn = "00000,00000,00000,00000,00000,00000,00000,00000,00000,0000";
String turnOff = "10000,00000,00000,00000,00000,00000,00000,00000,00000,0000";

#define BAUD_RATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

void serialSetup() {
  UBRR0H = (uint8_t)(BAUD_PRESCALLER >> 8);
  UBRR0L = (uint8_t)(BAUD_PRESCALLER);
  UCSR0B = (1 << TXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
}

void serialWrite(char data) {
  while (!(UCSR0A & (1 << UDRE0))); // Wait for empty transmit buffer
  UDR0 = data; // Put data into buffer, sends the data
}

void setup() {
  Serial.begin(9600); // Just for debugging, uses hardware serial
  pinMode(BUTTON, INPUT_PULLUP);
  serialSetup(); // Initialize software serial using registers
}
 
void loop() {
  if (digitalRead(BUTTON) == LOW) {
    Serial.print("Button Pressed\n");
    serialWrite('T'); // Start transmission marker
    for (int i = 0; i < turnOn.length(); i++) {
      serialWrite(turnOn[i]);
    }
    serialWrite('\n'); // End transmission marker
    delay(1000); // Delay for stability
  }
}
