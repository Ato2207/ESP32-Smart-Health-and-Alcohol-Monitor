#pragma once

#ifndef STATE_H
#define STATE_H

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