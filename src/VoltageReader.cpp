#include "VoltageReader.h"

float VoltageReader::voltageDividerRatio = 0.0;

void VoltageReader::begin() {
    // Configure ADC
    pinMode(ADC_PIN, INPUT);
    analogReadResolution(ADC_RESOLUTION);
    
    // Calculate voltage divider ratio: (R1 + R2) / R2
    voltageDividerRatio = (VOLTAGE_DIVIDER_R1 + VOLTAGE_DIVIDER_R2) / VOLTAGE_DIVIDER_R2;
}

int VoltageReader::readRawADC(int samples) {
    long sum = 0;
    
    for (int i = 0; i < samples; i++) {
        sum += analogRead(ADC_PIN);
        delay(10); // Small delay between samples
    }
    
    return sum / samples;
}

float VoltageReader::readADCVoltage() {
    int rawValue = readRawADC();
    
    // Convert ADC value to voltage
    float voltage = (rawValue * ADC_VREF) / ADC_MAX_VALUE;
    
    return voltage;
}

float VoltageReader::readBatteryVoltage() {
    float adcVoltage = readADCVoltage();
    
    // Compensate for voltage divider
    float batteryVoltage = adcVoltage * voltageDividerRatio;
    
    return batteryVoltage;
}

float VoltageReader::getVoltageDividerRatio() {
    return voltageDividerRatio;
}
