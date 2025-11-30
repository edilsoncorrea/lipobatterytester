#ifndef CONFIG_H
#define CONFIG_H

// Include Arduino-specific configuration if building for Arduino Pro Mini
#ifdef ARDUINO_PRO_MINI
#include "config_arduino.h"
#else
// ESP32-C3 Configuration (default)

// Hardware Configuration
#define ADC_PIN 0                    // GPIO0 for ADC reading
#define ADC_RESOLUTION 12            // 12-bit ADC (0-4095)
#define ADC_MAX_VALUE 4095           // Maximum ADC value
#define ADC_VREF 2.962               // Reference voltage (calibrated: 3.3V × 0.8976)

// Voltage Divider Configuration
// Calibrated values: R1 = 67.2kΩ (measured), R2 = 10.05kΩ (measured)
// Measured ratio: 15V input / 1.953V output = 7.6816:1
// Max input voltage: 3.3V * 7.6816 = 25.35V (safe for 6S at 25.2V)
#define VOLTAGE_DIVIDER_R1 67200.0   // 67.2kΩ resistor (high side) - measured value
#define VOLTAGE_DIVIDER_R2 10050.0   // 10.05kΩ resistor (low side/GND) - measured value

// I2C Pins for ESP32-C3
#define I2C_SDA 8                    // GPIO8
#define I2C_SCL 9                    // GPIO9

// Measurement Configuration
#define ADC_SAMPLES 10               // Number of ADC samples for averaging
#define MEASUREMENT_DELAY_MS 500     // Delay between measurements

// Default debug level (can be changed at runtime)
#ifndef DEBUG_VERBOSITY
#define DEBUG_VERBOSITY DEBUG_LEVEL_RAW
#endif

#endif // End ESP32-C3 specific config

// Common Configuration (shared between ESP32-C3 and Arduino Pro Mini)

// Battery Cell Specifications
#define CELL_VOLTAGE_MIN 2.9         // Minimum safe cell voltage
#define CELL_VOLTAGE_MAX 4.2         // Maximum cell voltage (fully charged)
#define CELL_VOLTAGE_EMPTY 3.3       // Empty cell voltage for percentage calculation
#define CELL_VOLTAGE_FULL 4.2        // Full cell voltage for percentage calculation
#define MAX_CELLS 6                  // Maximum number of cells (6S)

// Display Configuration (I2C OLED 0.91" 128x32)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1                // Reset pin (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C          // I2C address for 0.91" OLED

// Debug Levels
#define DEBUG_LEVEL_NONE 0           // No debug output
#define DEBUG_LEVEL_DISPLAY 1        // Show only what's on display
#define DEBUG_LEVEL_CALCULATED 2     // Show calculated values
#define DEBUG_LEVEL_RAW 3            // Show raw ADC values

#endif // CONFIG_H
