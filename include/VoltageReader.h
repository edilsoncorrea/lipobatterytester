#ifndef VOLTAGE_READER_H
#define VOLTAGE_READER_H

#include <Arduino.h>
#include "config.h"

/**
 * @brief Class for reading battery voltage using ADC with voltage divider
 */
class VoltageReader {
public:
    /**
     * @brief Initialize the voltage reader
     */
    static void begin();
    
    /**
     * @brief Read raw ADC value with averaging
     * @param samples Number of samples to average
     * @return Average raw ADC value (0-4095)
     */
    static int readRawADC(int samples = ADC_SAMPLES);
    
    /**
     * @brief Read voltage at ADC pin (after voltage divider)
     * @return Voltage at ADC pin in volts
     */
    static float readADCVoltage();
    
    /**
     * @brief Read actual battery voltage (compensated for voltage divider)
     * @return Battery voltage in volts
     */
    static float readBatteryVoltage();
    
    /**
     * @brief Calculate voltage divider ratio
     * @return Voltage divider multiplication factor
     */
    static float getVoltageDividerRatio();

private:
    static float voltageDividerRatio;
};

#endif // VOLTAGE_READER_H
