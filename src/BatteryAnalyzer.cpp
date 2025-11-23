#include "BatteryAnalyzer.h"
#include <cmath>

int BatteryAnalyzer::detectCellCount(float voltage) {
    // LiPo cell voltage specifications
    const float MIN_CELL_V = 2.9f;  // Minimum safe voltage per cell
    const float MAX_CELL_V = 4.2f;  // Maximum voltage per cell (fully charged)
    const int MAX_CELLS_COUNT = 6;  // Support up to 6S batteries
    
    // Check if voltage is too low to be valid (with 20% margin)
    if (voltage < MIN_CELL_V * 0.8) {
        return 0; // Invalid - voltage too low
    }
    
    // Check if voltage exceeds maximum possible (6S with 10% margin)
    if (voltage > MAX_CELL_V * MAX_CELLS_COUNT * 1.1) {
        return 0; // Invalid - voltage too high
    }
    
    /* Cell Detection Strategy:
     * 
     * Algorithm: "First Valid Match"
     * - Iterate from 1S to 6S
     * - Return the first configuration where voltage/cells falls in valid range
     * - This naturally prefers fewer cells (higher voltage per cell)
     * 
     * Rationale:
     * - Batteries are typically used in the 3.3V-4.2V/cell range
     * - Lower cell count with higher voltage is more common than higher cell count at minimum
     * - Example: 11.6V is more likely 3S@3.87V than 4S@2.9V (critically low)
     * 
     * Edge Cases:
     * - At exactly 2.9V/cell, multiple configurations may be valid
     * - Algorithm chooses configuration with higher voltage/cell (fewer cells)
     * - This represents the most probable real-world scenario
     * 
     * Floating Point Handling:
     * - Adds 1mV tolerance to handle floating point precision
     * - Prevents edge case failures (e.g., 12.6V/3 = 4.2V might be 4.199999...)
     */
    
    for (int cells = 1; cells <= MAX_CELLS_COUNT; cells++) {
        float avgCellVoltage = voltage / cells;
        
        // Check if this produces a valid cell voltage
        // Tolerance of 0.001V (1mV) for floating point comparison
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
