#include <Arduino.h>
#include "config.h"
#include "VoltageReader.h"
#include "BatteryAnalyzer.h"
#include "DisplayManager.h"
#include "DebugLogger.h"

void setup() {
    // Initialize debug logger first
    DebugLogger::begin(DEBUG_VERBOSITY);
    delay(100);
    DebugLogger::log("Starting LiPo Battery Tester...");
    DebugLogger::log("ESP32-C3 LiPo Battery Tester v1.0");
    DebugLogger::log("========================================");
    
    // Initialize voltage reader
    VoltageReader::begin();
    DebugLogger::log("Voltage reader initialized");
    
    // Initialize display (non-blocking)
    DebugLogger::log("Attempting to initialize display...");
    if (!DisplayManager::begin()) {
        DebugLogger::log("WARNING: Display initialization failed!");
        DebugLogger::log("Continuing without display (debug mode only)");
        // Don't halt - continue for debugging
    } else {
        DebugLogger::log("Display initialized successfully");
        // Show initialization message
        DisplayManager::displayInitMessage();
        delay(2000);
    }
    
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
