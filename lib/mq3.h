#pragma once

#include <Arduino.h>

const int MQ3_PIN = 34;
const double RL = 10000.0;
const double R0 = 90000.0; // Calibration value for MQ3 sensor

double measure_alcohol();
void init_adc();