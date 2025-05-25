#pragma once

#ifndef PULSE_OXYMETER_H
#define PULSE_OXYMETER_H

#include <MAX30105.h>
#include "filters.h"
#include "common.h"

extern MAX30105 pulseOxSensor;

void init_pulsOxSensor();
PulseOxData measure_pulseOx();

#endif