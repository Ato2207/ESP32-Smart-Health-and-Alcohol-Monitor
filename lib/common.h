#pragma once

#ifndef STATE_H
#define STATE_H

#include <driver/i2c.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#define I2C_MASTER_NUM              I2C_NUM_0
#define I2C_MASTER_SDA_IO           21      // Set your SDA pin
#define I2C_MASTER_SCL_IO           22      // Set your SCL pin
#define I2C_MASTER_FREQ_HZ          100000  // 100kHz

typedef enum State {
    PULSE_OXIMETER,
    GAS,
} State;

typedef struct PulseOxData {
    int bpm;
    int spo2;
} PulseOxData;

extern State state;

#endif