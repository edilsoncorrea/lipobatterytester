#ifndef CONFIG_H
#define CONFIG_H

// Hardware Configuration
#define ADC_PIN 0                    // GPIO0 for ADC reading
#define ADC_RESOLUTION 12            // 12-bit ADC (0-4095)
#define ADC_MAX_VALUE 4095           // Maximum ADC value
#define ADC_VREF 3.3                 // Reference voltage (3.3V)

// Voltage Divider Configuration
// For 6S battery (max 25.2V) to be read safely at 3.3V
// R1 = 68kΩ, R2 = 10kΩ gives ratio of ~7.8:1
// Max input voltage: 3.3V * 7.8 = 25.74V (safe for 6S)
#define VOLTAGE_DIVIDER_R1 68000.0   // 68kΩ resistor (high side)
#define VOLTAGE_DIVIDER_R2 10000.0   // 10kΩ resistor (low side/GND)

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

// I2C Pins for ESP32-C3
#define I2C_SDA 8                    // GPIO8
#define I2C_SCL 9                    // GPIO9

// Measurement Configuration
#define ADC_SAMPLES 10               // Number of ADC samples for averaging
#define MEASUREMENT_DELAY_MS 500     // Delay between measurements

// Debug Levels
#define DEBUG_LEVEL_NONE 0           // No debug output
#define DEBUG_LEVEL_DISPLAY 1        // Show only what's on display
#define DEBUG_LEVEL_CALCULATED 2     // Show calculated values
#define DEBUG_LEVEL_RAW 3            // Show raw ADC values

// Default debug level (can be changed at runtime)
#ifndef DEBUG_VERBOSITY
#define DEBUG_VERBOSITY DEBUG_LEVEL_DISPLAY
#endif

#endif // CONFIG_H
