#include "BatteryAnalyzer.h"
#include <cmath>

int BatteryAnalyzer::detectCellCount(float voltage) {
    // Hard-code values to debug
    const float MIN_CELL_V = 2.9f;
    const float MAX_CELL_V = 4.2f;
    const int MAX_CELLS_COUNT = 6;
    
    // Check if voltage is too low to be valid  
    if (voltage < MIN_CELL_V * 0.8) {
        return 0; // Invalid - voltage too low
    }
    
    // Check if voltage exceeds maximum possible (6S)
    if (voltage > MAX_CELL_V * MAX_CELLS_COUNT * 1.1) {
        return 0; // Invalid - voltage too high
    }
    
    // Strategy: Try each cell count from 1 to MAX_CELLS
    // Return the FIRST one that produces an average voltage within valid range
    // This prefers lower cell counts, which makes sense because:
    // - 16.8V is more likely 4S@4.2V than 5S@3.36V
    // - Higher voltage per cell is more common in normal use
    
    // Strategy: Find first valid cell count (lowest number of cells)
    // This works well in practice for most common scenarios
    // Note: Some edge cases with low battery voltage may be ambiguous
    
    for (int cells = 1; cells <= MAX_CELLS_COUNT; cells++) {
        float avgCellVoltage = voltage / cells;
        
        // Check if this produces a valid cell voltage
        // Add small tolerance for floating point comparison (0.001V = 1mV)
        if (avgCellVoltage >= (MIN_CELL_V - 0.001f) && avgCellVoltage <= (MAX_CELL_V + 0.001f)) {
            return cells;
        }
    }
    
    return 0;  // No valid configuration found
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
    if (averageCellVoltage >= CELL_VOLTAGE_FULL) {  // >= to handle 4.2V exactly
        return 100;
    }
    
    // Calculate percentage based on voltage range
    float voltageRange = CELL_VOLTAGE_FULL - CELL_VOLTAGE_EMPTY;
    float voltageAboveEmpty = averageCellVoltage - CELL_VOLTAGE_EMPTY;
    
    // Round instead of truncate for better accuracy
    int percentage = (int)((voltageAboveEmpty / voltageRange) * 100.0 + 0.5);
    
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
