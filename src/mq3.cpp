#include "../lib/mq3.h"

double measure_alcohol() {
    const int measurementDuration = 500; // milliseconds
    const int sampleInterval = 50; // milliseconds
    const int numSamples = measurementDuration / sampleInterval;
    int samples[numSamples];

    unsigned long startTime = millis();
    int sampleCount = 0;

    while (millis() - startTime < measurementDuration && sampleCount < numSamples) {
        samples[sampleCount++] = analogRead(MQ3_PIN);
        delay(sampleInterval);
    }

    float mean = 0.0;
    for (int i = 0; i < sampleCount; ++i) {
        mean += samples[i];
    }
    mean /= sampleCount;

    double Vout = static_cast<double>(mean) * (5 / 4096.0);
    double Rs = RL * (5 - Vout) / Vout;

    double ratio = Rs / R0;

    // Apply the log-log relationship to get ppm
    double ppm = pow(10, -1.76 * log10(ratio) + 2.3);

    // Optionally convert to mg/L (ethanol molecular weight = 46.07 g/mol)
    double mgL = (ppm * 46.07 / 24.45) / 1000.0;

    float finalValue = (0.4/200)*mgL*3.5;
    //Serial.println(finalValue);

    //Print readings
    Serial.print("ADC: ");
    Serial.print(mean);
    Serial.print(" | Rs: ");
    Serial.print(Rs);
    Serial.print(" | Ratio: ");
    Serial.print(ratio);
    Serial.print(" | Alcohol: ");
    Serial.print(ppm);
    Serial.print(" ppm, ");
    Serial.print(mgL);
    Serial.println(" mg/L");

    return finalValue;
}