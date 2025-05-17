#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_I2CDevice.h>
#include "../lib/display.hpp"

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

bool redraw_display = true;

void init_display() {
  // Initialize the display
  tft.initR(INITR_144GREENTAB);
  tft.setRotation(2);

  // Clear the screen with a color
  tft.fillScreen(ST77XX_BLACK);

  // Display text
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(0, 30);
  tft.println("Starting...");
}

void change_display(double mgL, PulseOxData data) {
  static double lastMgL = -1; // Store last value to avoid unnecessary redraws

  if (state == GAS) {
    if (redraw_display) {
      // Draw static elements only once
      tft.fillScreen(ST77XX_BLACK);
      tft.setTextColor(ST77XX_WHITE);
      tft.setTextSize(2);
      tft.setCursor(3, 35);
      tft.println("Alcooltest");

      tft.drawFastHLine(0, 60, tft.width(), ST77XX_WHITE);

      redraw_display = false;
      lastMgL = -1;
    }

    // Only update value if it changed
    if (mgL != lastMgL) {
      // Overwrite old value area
      tft.fillRect(10, 70, 120, 20, ST77XX_BLACK); // Adjust size as needed
      tft.setCursor(10, 70);
      tft.setTextColor(ST77XX_WHITE);
      tft.print(mgL, 2);
      tft.print(" mg/L");
      lastMgL = mgL;
    }
  }

  static int lastBpm = -1;
  static float lastSpo2 = -1;

  if (state == PULSE_OXIMETER) {
    if (redraw_display) {
        tft.fillScreen(ST77XX_BLACK);
        tft.setTextColor(ST77XX_WHITE);
        tft.setTextSize(2);
        tft.setCursor(25, 5);
        tft.println("PulseOx");

        // Draw a heart below "PulseOx" (adjust coordinates as needed)
        int x = 64;  // center x
        int y = 35;  // center y for the heart
        int r = 10;  // radius for the heart lobes
        uint16_t color = ST77XX_RED;

        // Left lobe
        tft.fillCircle(x - r, y, r, color);
        // Right lobe
        tft.fillCircle(x + r, y, r, color);
        // Bottom triangle
        tft.fillTriangle(x - 2 * r, y + 4, x + 2 * r, y + 4, x, y + 2 * r + 4, color);

        tft.drawFastHLine(0, 67, tft.width(), ST77XX_WHITE);

        tft.setCursor(0, 75);
        tft.print("BPM: ");
        tft.setCursor(0, 100);
        tft.print("SpO2:");
        redraw_display = false;
        lastBpm = -1;
        lastSpo2 = -1;
    }

    // Only update BPM if it changed
    if (data.bpm != lastBpm && data.bpm != 0) {
        tft.fillRect(60, 75, 60, 20, ST77XX_BLACK); // Clear old BPM value
        tft.setCursor(60, 75);
        tft.setTextColor(ST77XX_WHITE);

        if (data.bpm >= 0) {
          tft.print(data.bpm);
        }

        lastBpm = data.bpm;
    }

    // Only update SpO2 if it changed
    if (data.spo2 != lastSpo2 && data.spo2 != 0.0f) {
        tft.fillRect(65, 100, 60, 20, ST77XX_BLACK); // Clear old SpO2 value
        tft.setCursor(65, 100);

        if (data.spo2 >= 95.0) {
            tft.setTextColor(ST77XX_GREEN);
        } else if (data.spo2 >= 90.0) {
            tft.setTextColor(ST77XX_YELLOW);
        } else {
            tft.setTextColor(ST77XX_RED);
        }

        if (data.spo2 >= 0) {
            tft.print(data.spo2);
            tft.print("%");
        }

        lastSpo2 = data.spo2;
    }
  }
}