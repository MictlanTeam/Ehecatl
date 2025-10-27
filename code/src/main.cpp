#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("¡Hola ESP32-S3-Zero!");
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  Serial.println("LED encendido");
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  Serial.println("LED apagado");
}