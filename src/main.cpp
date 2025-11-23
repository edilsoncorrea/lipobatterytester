#include <Arduino.h>
#include "config.h"
#include "VoltageReader.h"
#include "BatteryAnalyzer.h"
#include "DisplayManager.h"
#include "DebugLogger.h"

void setup() {
    // Initialize debug logger first
    DebugLogger::begin(DEBUG_VERBOSITY);
    DebugLogger::log("Starting LiPo Battery Tester...");
    
    // Initialize voltage reader
    VoltageReader::begin();
    DebugLogger::log("Voltage reader initialized");
    
    // Initialize display
    if (!DisplayManager::begin()) {
        DebugLogger::log("ERROR: Display initialization failed!");
        while (1) {
            delay(1000); // Halt if display fails
        }
    }
    DebugLogger::log("Display initialized");
    
    // Show initialization message
    DisplayManager::displayInitMessage();
    delay(2000);
    
    DebugLogger::log("System ready!\n");
}

void loop() {
    // Read raw ADC value for debugging
    int rawADC = VoltageReader::readRawADC();
    float adcVoltage = VoltageReader::readADCVoltage();
    
    // Log raw values if debug level is high enough
    DebugLogger::logRawADC(rawADC, adcVoltage);
    
    // Read battery voltage
    float batteryVoltage = VoltageReader::readBatteryVoltage();
    
    // Analyze battery
    BatteryInfo info = BatteryAnalyzer::analyzeBattery(batteryVoltage);
    
    // Log calculated values
    DebugLogger::logCalculatedValues(batteryVoltage, info);
    
    // Display battery information on OLED
    DisplayManager::displayBatteryInfo(info);
    
    // Log what's shown on display
    DebugLogger::logDisplayInfo(info);
    
    // Wait before next measurement
    delay(MEASUREMENT_DELAY_MS);
}
