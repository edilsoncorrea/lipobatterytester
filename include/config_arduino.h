#ifndef CONFIG_ARDUINO_H
#define CONFIG_ARDUINO_H

// Arduino Pro Mini Specific Configuration
#ifdef ARDUINO_PRO_MINI

// Hardware Configuration for Arduino Pro Mini
#define ADC_PIN A0                   // A0 pin for ADC reading
#define ADC_RESOLUTION 10            // 10-bit ADC (0-1023) on Arduino
#define ADC_MAX_VALUE 1023           // Maximum ADC value
#define ADC_VREF 5.0                 // Reference voltage (5V for Pro Mini)

// Voltage Divider Configuration (same resistor values)
// Calibrated values: R1 = 67.2kΩ (measured), R2 = 10.05kΩ (measured)
// Ratio: 7.6866 (calculated from resistor values)
// Max input voltage: 5.0V * 7.6866 = 38.43V (safe for 6S at 25.2V)
#define VOLTAGE_DIVIDER_R1 67200.0   // 67.2kΩ resistor (high side) - measured value
#define VOLTAGE_DIVIDER_R2 10050.0   // 10.05kΩ resistor (low side/GND) - measured value

// I2C Pins for Arduino Pro Mini
#define I2C_SDA A4                   // SDA on A4
#define I2C_SCL A5                   // SCL on A5

// Display Configuration (same OLED)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1                // Reset pin (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C          // I2C address for 0.91" OLED

// Measurement Configuration
#define ADC_SAMPLES 10               // Number of ADC samples for averaging
#define MEASUREMENT_DELAY_MS 1000    // Longer delay for Arduino (slower processing)

// Debug Levels (same as ESP32)
#define DEBUG_LEVEL_NONE 0           // No debug output
#define DEBUG_LEVEL_DISPLAY 1        // Show only what's on display
#define DEBUG_LEVEL_CALCULATED 2     // Show calculated values
#define DEBUG_LEVEL_RAW 3            // Show raw ADC values

// Default debug level for Arduino
#ifndef DEBUG_VERBOSITY
#define DEBUG_VERBOSITY DEBUG_LEVEL_DISPLAY  // Less verbose for Arduino
#endif

#endif // ARDUINO_PRO_MINI

#endif // CONFIG_ARDUINO_H