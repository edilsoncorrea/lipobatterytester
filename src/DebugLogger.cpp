#include "DebugLogger.h"

int DebugLogger::debugLevel = DEBUG_VERBOSITY;

void DebugLogger::begin(int level) {
    debugLevel = level;
    
    if (debugLevel > DEBUG_LEVEL_NONE) {
        Serial.begin(115200);
        delay(1000); // Wait for serial to initialize properly
        
        // Send multiple messages to ensure connection
        for (int i = 0; i < 3; i++) {
            Serial.println("\n=== LiPo Battery Tester Debug Logger ===");
            delay(100);
        }
        
        Serial.print("Debug Level: ");
        Serial.println(debugLevel);
        Serial.println("========================================\n");
        Serial.flush();
    }
}

void DebugLogger::setLevel(int level) {
    if (level < DEBUG_LEVEL_NONE) level = DEBUG_LEVEL_NONE;
    if (level > DEBUG_LEVEL_RAW) level = DEBUG_LEVEL_RAW;
    
    debugLevel = level;
    
    if (debugLevel > DEBUG_LEVEL_NONE) {
        Serial.print("Debug level changed to: ");
        Serial.println(debugLevel);
    }
}

int DebugLogger::getLevel() {
    return debugLevel;
}

void DebugLogger::logRawADC(int rawValue, float adcVoltage) {
    if (debugLevel >= DEBUG_LEVEL_RAW) {
        Serial.println("--- Raw ADC Reading ---");
        Serial.print("Raw ADC Value: ");
        Serial.println(rawValue);
        Serial.print("ADC Pin Voltage: ");
        Serial.print(adcVoltage, 4);
        Serial.println(" V");
        Serial.println();
    }
}

void DebugLogger::logCalculatedValues(float batteryVoltage, const BatteryInfo& info) {
    if (debugLevel >= DEBUG_LEVEL_CALCULATED) {
        Serial.println("--- Calculated Values ---");
        Serial.print("Battery Voltage: ");
        Serial.print(batteryVoltage, 3);
        Serial.println(" V");
        Serial.print("Detected Cells: ");
        Serial.println(info.cellCount);
        
        if (info.isValid) {
            Serial.print("Average Cell Voltage: ");
            Serial.print(info.averageCellVoltage, 3);
            Serial.println(" V");
            Serial.print("Charge Percentage: ");
            Serial.print(info.chargePercentage);
            Serial.println(" %");
        } else {
            Serial.println("Invalid battery reading!");
        }
        Serial.println();
    }
}

void DebugLogger::logDisplayInfo(const BatteryInfo& info) {
    if (debugLevel >= DEBUG_LEVEL_DISPLAY) {
        Serial.println("--- Display Output ---");
        
        if (info.isValid) {
            Serial.print(info.cellCount);
            Serial.print("S ");
            Serial.print(info.totalVoltage, 2);
            Serial.println("V");
            
            if (info.cellCount > 1) {
                Serial.print("Avg: ");
                Serial.print(info.averageCellVoltage, 2);
                Serial.println("V/cell");
            }
            
            Serial.print("Charge: ");
            Serial.print(info.chargePercentage);
            Serial.println("%");
        } else {
            Serial.println("Invalid Battery!");
        }
        
        Serial.println();
    }
}

void DebugLogger::log(const char* message) {
    if (debugLevel > DEBUG_LEVEL_NONE) {
        Serial.println(message);
    }
}
