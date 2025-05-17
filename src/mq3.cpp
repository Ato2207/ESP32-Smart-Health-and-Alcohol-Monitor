#include "../lib/mq3.hpp"

double measure_alcohol() {
    const int measurementDuration = 1000; // milliseconds
    const int sampleInterval = 50; // milliseconds
    const int numSamples = measurementDuration / sampleInterval;
    int samples[numSamples];

    unsigned long startTime = millis();
    int sampleCount = 0;

    while (millis() - startTime < measurementDuration && sampleCount < numSamples) {
        samples[sampleCount++] = analogRead(MQ3_PIN);
        delay(sampleInterval);
    }

    // Simple insertion sort for median
    for (int i = 1; i < sampleCount; ++i) {
        int key = samples[i];
        int j = i - 1;
        while (j >= 0 && samples[j] > key) {
            samples[j + 1] = samples[j];
            --j;
        }
        samples[j + 1] = key;
    }

    int median;
    if (sampleCount % 2 == 0) {
        median = (samples[sampleCount/2 - 1] + samples[sampleCount/2]) / 2;
    } else {
        median = samples[sampleCount/2];
    }

    double Vout = static_cast<double>(median) * (5 / 4096.0);
    double Rs = RL * (5 - Vout) / Vout;
    
    double ratio = Rs / R0;

    // Apply the log-log relationship to get ppm
    double ppm = pow(10, -1.76 * log10(ratio) + 2.3);

    // Optionally convert to mg/L (ethanol molecular weight = 46.07 g/mol)
    double mgL = (ppm * 46.07 / 24.45) / 1000.0;

    //Print readings
    Serial.print("ADC: ");
    Serial.print(median);
    Serial.print(" | Rs: ");
    Serial.print(Rs);
    Serial.print(" | Ratio: ");
    Serial.print(ratio);
    Serial.print(" | Alcohol: ");
    Serial.print(ppm);
    Serial.print(" ppm, ");
    Serial.print(mgL);
    Serial.println(" mg/L");

    return mgL;
}