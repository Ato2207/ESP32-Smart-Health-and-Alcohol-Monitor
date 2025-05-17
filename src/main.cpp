#include <SPI.h>
#include <Wire.h>

#include "../lib/common.hpp"
#include "../lib/display.hpp"
#include "../lib/pulse_oxymeter.h"
#include "../lib/button_buzzer.hpp"
#include "../lib/mq3.hpp"
#include "../lib/common.hpp"

State state = PULSE_OXIMETER;

void change_state() {
    buttonPressed = false;
    switch (state) {
      case PULSE_OXIMETER:
        state = GAS;
        break;
      case GAS:
        state = PULSE_OXIMETER;
        break;
    }

    redraw_display = true;
}

void setup_prereq() {
  Serial.begin(115200);
  Wire.begin();
}

void setup() {
  setup_prereq();

  init_display();
  init_gpio();
  init_pulsOxSensor();

  delay(1000);
}

void loop() {
  if (buttonPressed) {
    Serial.printf("Button pressed.\n");
    ledcWriteTone(buzzerChannel, NOTE_C4);
    delay(100); 
    ledcWriteTone(buzzerChannel, 0);
    change_state();
  }


  PulseOxData data = {0, 0};

  switch (state) {
    case PULSE_OXIMETER:
      data = measure_pulseOx();
      change_display(0, data);
      break;
    case GAS:
      double mgL = measure_alcohol();
      change_display(mgL, PulseOxData{});
      break;
  }
}