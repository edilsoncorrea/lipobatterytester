#include "BatteryAnalyzer.h"
#include <math.h>

int BatteryAnalyzer::detectCellCount(float voltage) {
    // Check if voltage is too low to be valid
    if (voltage < CELL_VOLTAGE_MIN * 0.9) {
        return 0; // Invalid - voltage too low
    }
    
    // Check if voltage exceeds maximum possible (6S)
    if (voltage > CELL_VOLTAGE_MAX * MAX_CELLS) {
        return 0; // Invalid - voltage too high
    }
    
    // Try to detect cell count by finding the best fit
    // We'll check each possible cell count and see which gives
    // an average cell voltage within the valid range
    
    for (int cells = 1; cells <= MAX_CELLS; cells++) {
        float avgCellVoltage = voltage / cells;
        
        // Check if this average falls within valid cell voltage range
        // Using a slightly wider range to account for measurement variations
        if (avgCellVoltage >= (CELL_VOLTAGE_MIN - 0.1) && 
            avgCellVoltage <= (CELL_VOLTAGE_MAX + 0.1)) {
            return cells;
        }
    }
    
    return 0; // No valid cell count found
}

float BatteryAnalyzer::calculateAverageCellVoltage(float totalVoltage, int cellCount) {
    if (cellCount <= 0) {
        return 0.0;
    }
    
    return totalVoltage / cellCount;
}

int BatteryAnalyzer::calculateChargePercentage(float averageCellVoltage) {
    // Clamp voltage to valid range
    if (averageCellVoltage < CELL_VOLTAGE_EMPTY) {
        return 0;
    }
    if (averageCellVoltage > CELL_VOLTAGE_FULL) {
        return 100;
    }
    
    // Calculate percentage based on voltage range
    float voltageRange = CELL_VOLTAGE_FULL - CELL_VOLTAGE_EMPTY;
    float voltageAboveEmpty = averageCellVoltage - CELL_VOLTAGE_EMPTY;
    
    int percentage = (int)((voltageAboveEmpty / voltageRange) * 100.0);
    
    // Ensure percentage is within 0-100 range
    if (percentage < 0) percentage = 0;
    if (percentage > 100) percentage = 100;
    
    return percentage;
}

BatteryInfo BatteryAnalyzer::analyzeBattery(float voltage) {
    BatteryInfo info;
    
    info.totalVoltage = voltage;
    info.cellCount = detectCellCount(voltage);
    
    if (info.cellCount > 0) {
        info.averageCellVoltage = calculateAverageCellVoltage(voltage, info.cellCount);
        info.chargePercentage = calculateChargePercentage(info.averageCellVoltage);
        info.isValid = true;
    } else {
        info.averageCellVoltage = 0.0;
        info.chargePercentage = 0;
        info.isValid = false;
    }
    
    return info;
}

bool BatteryAnalyzer::isVoltageValid(float voltage, int cellCount) {
    if (cellCount < 1 || cellCount > MAX_CELLS) {
        return false;
    }
    
    float minVoltage = CELL_VOLTAGE_MIN * cellCount;
    float maxVoltage = CELL_VOLTAGE_MAX * cellCount;
    
    return (voltage >= minVoltage && voltage <= maxVoltage);
}
