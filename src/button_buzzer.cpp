#include "../lib/button_buzzer.h"

const int buzzerPin = 15;
const gpio_num_t buttonPin = GPIO_NUM_19;

ledc_channel_t buzzerChannel = LEDC_CHANNEL_0;
volatile bool buttonPressed = false;

//variables to keep track of the timing of recent interrupts
unsigned long button_time = 0;
unsigned long last_button_time = 0;

void IRAM_ATTR isr_button(void* arg) {
  // Check if GPIO19 caused the interrupt
  if (GPIO.status & (1 << 19)) {
    GPIO.status_w1tc = (1 << 19); // Clear interrupt
    button_time = esp_timer_get_time() / 1000;
    if (button_time - last_button_time > 250) {
      buttonPressed = true;
      last_button_time = button_time;
    }
  }
}

void init_gpio() {
  //Setup buzzer PWM (LEDC)
  ledc_timer_config_t ledc_timer = {
      .speed_mode = LEDC_LOW_SPEED_MODE,
      .duty_resolution = LEDC_TIMER_8_BIT,
      .timer_num = LEDC_TIMER_0,
      .freq_hz = 500,
      .clk_cfg = LEDC_AUTO_CLK
  };
  ledc_timer_config(&ledc_timer);

  ledc_channel_config_t ledc_channel = {
      .gpio_num = buzzerPin,
      .speed_mode = LEDC_LOW_SPEED_MODE,
      .channel = buzzerChannel,
      .timer_sel = LEDC_TIMER_0,
      .duty = 0,
      .hpoint = 0
  };
  ledc_channel_config(&ledc_channel);

  // Set GPIO19 as input
  GPIO.enable_w1tc = (1 << 19); // Clear output enable (input mode)

  // Enable pull-up
  GPIO.pin[19].pad_driver = 0; // Normal output (not open-drain)
  REG_SET_BIT(GPIO_PIN_MUX_REG[19], FUN_PU); // Enable pull-up (register macro)

  // Set interrupt type: negative edge
  GPIO.pin[19].int_type = 1; // 1 = negative edge

  // Enable GPIO interrupt for GPIO19
  GPIO.status_w1tc = (1 << 19); // Clear any pending
  GPIO.pin[19].int_ena = 1;     // Enable interrupt for CPU 0

  // Register the ISR (still need ESP-IDF API for this part)
  esp_intr_alloc(ETS_GPIO_INTR_SOURCE, 0, isr_button, NULL, NULL);

  // Enable global GPIO interrupts
  ets_intr_unlock();
}