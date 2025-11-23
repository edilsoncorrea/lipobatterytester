#ifndef DEBUG_LOGGER_H
#define DEBUG_LOGGER_H

#include <Arduino.h>
#include "config.h"
#include "BatteryAnalyzer.h"

/**
 * @brief Class for managing debug output with verbosity levels
 */
class DebugLogger {
public:
    /**
     * @brief Initialize the debug logger
     * @param level Initial debug verbosity level
     */
    static void begin(int level = DEBUG_VERBOSITY);
    
    /**
     * @brief Set debug verbosity level
     * @param level Debug level (0-3)
     */
    static void setLevel(int level);
    
    /**
     * @brief Get current debug level
     * @return Current debug level
     */
    static int getLevel();
    
    /**
     * @brief Log raw ADC reading (Level 3)
     * @param rawValue Raw ADC value
     * @param adcVoltage Voltage at ADC pin
     */
    static void logRawADC(int rawValue, float adcVoltage);
    
    /**
     * @brief Log calculated values (Level 2)
     * @param batteryVoltage Total battery voltage
     * @param info Battery analysis information
     */
    static void logCalculatedValues(float batteryVoltage, const BatteryInfo& info);
    
    /**
     * @brief Log display information (Level 1)
     * @param info Battery analysis information
     */
    static void logDisplayInfo(const BatteryInfo& info);
    
    /**
     * @brief Log general message
     * @param message Message to log
     */
    static void log(const char* message);

private:
    static int debugLevel;
};

#endif // DEBUG_LOGGER_H
