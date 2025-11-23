#ifndef BATTERY_ANALYZER_H
#define BATTERY_ANALYZER_H

#include <Arduino.h>
#include "config.h"

/**
 * @brief Structure to hold battery analysis results
 */
struct BatteryInfo {
    float totalVoltage;      // Total battery voltage
    int cellCount;           // Detected number of cells
    float averageCellVoltage; // Average voltage per cell
    int chargePercentage;    // Battery charge percentage (0-100)
    bool isValid;            // Whether the reading is valid
};

/**
 * @brief Class for analyzing battery characteristics
 */
class BatteryAnalyzer {
public:
    /**
     * @brief Detect the number of cells based on voltage
     * @param voltage Total battery voltage
     * @return Number of cells detected (1-6), or 0 if invalid
     */
    static int detectCellCount(float voltage);
    
    /**
     * @brief Calculate average voltage per cell
     * @param totalVoltage Total battery voltage
     * @param cellCount Number of cells
     * @return Average voltage per cell
     */
    static float calculateAverageCellVoltage(float totalVoltage, int cellCount);
    
    /**
     * @brief Calculate battery charge percentage
     * @param averageCellVoltage Average voltage per cell
     * @return Charge percentage (0-100)
     */
    static int calculateChargePercentage(float averageCellVoltage);
    
    /**
     * @brief Analyze battery and return complete information
     * @param voltage Total battery voltage
     * @return BatteryInfo structure with all calculated values
     */
    static BatteryInfo analyzeBattery(float voltage);
    
    /**
     * @brief Check if voltage is within valid range for given cell count
     * @param voltage Total battery voltage
     * @param cellCount Number of cells
     * @return true if voltage is valid for the cell count
     */
    static bool isVoltageValid(float voltage, int cellCount);
};

#endif // BATTERY_ANALYZER_H
