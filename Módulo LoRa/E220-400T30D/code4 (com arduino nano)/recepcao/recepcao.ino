#include <avr/io.h>

#define RX_PIN 2
#define TX_PIN 3

#define BUTTON 5

#define BAUD_RATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

float initialTime = 0;
float previousTime = 0;
float nextTime = 0;

bool initiated = false;
int counter = 0;

void serialSetup() {
  UBRR0H = (uint8_t)(BAUD_PRESCALLER >> 8);
  UBRR0L = (uint8_t)(BAUD_PRESCALLER);
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); // Enable receiver, transmitter and RX interrupt
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
}

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON, INPUT_PULLUP);
  serialSetup();
}

ISR(USART_RX_vect) {
  char receivedByte = UDR0;
  Serial.print("Received: ");
  Serial.println(receivedByte);

  // Handle received data here
}

void loop() {
  if (digitalRead(BUTTON) == LOW) {
    initialTime = millis();
    Serial.print("Initial Time: ");
    Serial.println(initialTime);
    initiated = true;
  }

  if (initiated) {
    previousTime = millis();
    // Implement your logic here
  }
}
