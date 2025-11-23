#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h"
#include "BatteryAnalyzer.h"

/**
 * @brief Class for managing OLED display output
 */
class DisplayManager {
public:
    /**
     * @brief Initialize the display
     * @return true if successful, false otherwise
     */
    static bool begin();
    
    /**
     * @brief Clear the display
     */
    static void clear();
    
    /**
     * @brief Display battery information on OLED
     * @param info BatteryInfo structure with battery data
     */
    static void displayBatteryInfo(const BatteryInfo& info);
    
    /**
     * @brief Display error message
     * @param message Error message to display
     */
    static void displayError(const char* message);
    
    /**
     * @brief Display initialization message
     */
    static void displayInitMessage();

private:
    static Adafruit_SSD1306* display;
};

#endif // DISPLAY_MANAGER_H
