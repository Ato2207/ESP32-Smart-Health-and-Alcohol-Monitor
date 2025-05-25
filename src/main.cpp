#include "../lib/common.h"
#include "../lib/display.hpp"
#include "../lib/pulse_oxymeter.hpp"
#include "../lib/button_buzzer.h"
#include "../lib/mq3.h"

QueueHandle_t buzzerQueue;

void buzzer_task(void *param) {
    int freq;

    while (1) {
        if (xQueueReceive(buzzerQueue, &freq, portMAX_DELAY)) {
            ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0, freq);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, buzzerChannel, 128);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, buzzerChannel);

            vTaskDelay(pdMS_TO_TICKS(100));

            ledc_set_duty(LEDC_LOW_SPEED_MODE, buzzerChannel, 0);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, buzzerChannel);
        }
    }
}

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
}

void setup() {
  setup_prereq();

  init_display();
  init_gpio();
  init_pulsOxSensor();

  buzzerQueue = xQueueCreate(2, sizeof(int));
  xTaskCreate(buzzer_task, "buzzer_task", 2048, NULL, 1, NULL);

  delay(1000);
}

void loop() {
  if (buttonPressed) {
    int note = NOTE_D4;
    xQueueSend(buzzerQueue, &note, 0);

    change_state();
  }

  PulseOxData data = {0, 0};
  PulseOxData dummyData = {0, 0};
  switch (state) {
    case PULSE_OXIMETER:
      data = measure_pulseOx();
      change_display(0, data);
      break;
    case GAS:
      double gL = measure_alcohol();
      change_display(gL, dummyData);
      break;
  }
}
