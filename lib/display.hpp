#pragma once

#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_I2CDevice.h>
#include "../lib/common.h"

// Display TFT 128x128
#define TFT_MOSI 23
#define TFT_SCK 18
#define TFT_CS  5
#define TFT_DC 4
#define TFT_RST 2

extern Adafruit_ST7735 tft;
extern bool redraw_display;

void init_display();
void change_display(double, PulseOxData);

#endif