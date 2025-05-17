#include "../lib/button_buzzer.hpp"

const int buzzerPin = 15;
const int buttonPin = 19;

int buzzerChannel = 0;
bool buttonPressed = false;

//variables to keep track of the timing of recent interrupts
unsigned long button_time = 0;
unsigned long last_button_time = 0;

void IRAM_ATTR isr_button() {
  button_time = millis();
  if (button_time - last_button_time > 250) {
    buttonPressed = true;
    last_button_time = button_time;
  }
}

void init_gpio() {
  ledcSetup(buzzerChannel, 0, 8);
  ledcAttachPin(buzzerPin, buzzerChannel);

  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(buttonPin, isr_button, FALLING);
}