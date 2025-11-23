# LiPo Battery Tester

An automatic LiPo battery tester for ESP32-C3 that detects cell count (1S-6S) and displays battery status on a 0.91" OLED display.

## Features

- **Automatic Cell Detection**: Automatically identifies battery configuration from 1S to 6S based on voltage readings
- **Real-time Monitoring**: Displays total voltage, average cell voltage, and charge percentage
- **OLED Display**: Clear information display on 0.91" I2C OLED screen (128x32)
- **Voltage Divider Protection**: Safe measurement circuit prevents ADC damage when testing up to 6S batteries
- **Configurable Debug Levels**: Multiple verbosity levels for debugging and monitoring
- **Comprehensive Unit Tests**: Full test suite to validate cell detection accuracy

## Hardware Requirements

### Components
- ESP32-C3 Development Board (ESP32-C3-DevKitM-1 or similar)
- 0.91" OLED Display (128x32, I2C, SSD1306)
- Resistors for voltage divider:
  - R1: 68kΩ (high side)
  - R2: 10kΩ (low side/ground)
- Connecting wires
- Battery connector (XT60, JST, or compatible with your batteries)

### Circuit Diagram

```
Battery (+) ----[R1: 68kΩ]----+----[R2: 10kΩ]---- GND
                              |
                              +---- ADC Pin (GPIO0)

OLED SDA ---- GPIO8
OLED SCL ---- GPIO9
OLED VCC ---- 3.3V
OLED GND ---- GND
```

### Voltage Divider Calculation
- **Ratio**: (68kΩ + 10kΩ) / 10kΩ = 7.8:1
- **Max Input**: 3.3V × 7.8 = 25.74V
- **Safe for 6S**: 6 × 4.2V = 25.2V ✓

The voltage divider ensures the ADC never receives more than 3.3V, even when measuring a fully charged 6S battery.

## Software Features

### Cell Detection Algorithm
The system automatically detects the number of cells by:
1. Reading the total battery voltage
2. Calculating the average voltage per cell for each possible configuration (1S-6S)
3. Selecting the configuration where average cell voltage falls within valid range (2.9V - 4.2V)

### Charge Percentage Calculation
- **Empty**: 3.3V per cell = 0%
- **Full**: 4.2V per cell = 100%
- Linear interpolation between these values

### Debug Verbosity Levels
- **Level 0** (NONE): No debug output
- **Level 1** (DISPLAY): Shows the same information displayed on OLED
- **Level 2** (CALCULATED): Shows calculated values including cell detection
- **Level 3** (RAW): Shows raw ADC readings and all intermediate values

## Installation

### Prerequisites
- [PlatformIO](https://platformio.org/) installed in VS Code or CLI
- ESP32 board support package
- Git (for cloning the repository)

### Setup Instructions

1. **Clone the repository**
   ```bash
   git clone https://github.com/edilsoncorrea/lipobatterytester.git
   cd lipobatterytester
   ```

2. **Open in PlatformIO**
   - Open the project folder in VS Code with PlatformIO extension
   - Or use PlatformIO CLI

3. **Build the project**
   ```bash
   pio run
   ```

4. **Upload to ESP32-C3**
   ```bash
   pio run --target upload
   ```

5. **Monitor serial output** (optional)
   ```bash
   pio device monitor
   ```

## Configuration

All configuration options are in `include/config.h`:

### Pin Configuration
```cpp
#define ADC_PIN 0           // GPIO0 for voltage measurement
#define I2C_SDA 8           // GPIO8 for OLED data
#define I2C_SCL 9           // GPIO9 for OLED clock
```

### Voltage Divider
```cpp
#define VOLTAGE_DIVIDER_R1 68000.0  // 68kΩ
#define VOLTAGE_DIVIDER_R2 10000.0  // 10kΩ
```

### Cell Voltage Limits
```cpp
#define CELL_VOLTAGE_MIN 2.9    // Minimum safe voltage
#define CELL_VOLTAGE_MAX 4.2    // Maximum voltage (fully charged)
#define CELL_VOLTAGE_EMPTY 3.3  // Empty for percentage calculation
#define CELL_VOLTAGE_FULL 4.2   // Full for percentage calculation
```

### Debug Level
```cpp
#define DEBUG_VERBOSITY DEBUG_LEVEL_DISPLAY  // Default debug level
```

## Usage

1. **Connect the battery** to the input connector (observe polarity!)
2. **Power on** the ESP32-C3
3. **Read the display**:
   - Line 1: Cell count (e.g., "3S") and total voltage
   - Line 2: Average cell voltage (hidden for 1S to avoid redundancy)
   - Line 3: Charge percentage
   - Line 4: Visual charge bar graph

### Display Examples

**3S Battery (11.1V, 44% charge)**
```
3S 11.10V
Avg: 3.70V/cell
Charge: 44%
[========        ]
```

**1S Battery (3.85V, 61% charge)**
```
1S 3.85V

Charge: 61%
[============    ]
```

## Testing

The project includes comprehensive unit tests for the battery analyzer.

### Run Tests
```bash
pio test -e native
```

### Test Coverage
- Cell detection for 1S through 6S batteries
- Invalid voltage detection (too low/high)
- Average cell voltage calculation
- Charge percentage calculation
- Complete battery analysis validation
- Voltage validation functions

All tests simulate various battery voltages to ensure accurate cell detection across the full voltage range (2.9V - 4.2V per cell).

## Project Structure

```
lipobatterytester/
├── include/
│   ├── config.h              # Configuration constants
│   ├── VoltageReader.h       # ADC reading and voltage conversion
│   ├── BatteryAnalyzer.h     # Cell detection and analysis
│   ├── DisplayManager.h      # OLED display control
│   └── DebugLogger.h         # Debug output management
├── src/
│   ├── main.cpp              # Main application
│   ├── VoltageReader.cpp
│   ├── BatteryAnalyzer.cpp
│   ├── DisplayManager.cpp
│   └── DebugLogger.cpp
├── test/
│   └── test_battery_analyzer.cpp  # Unit tests
├── platformio.ini            # PlatformIO configuration
└── README.md                 # This file
```

## Safety Warnings

⚠️ **IMPORTANT SAFETY INFORMATION**

- **Never exceed 6S (25.2V)** input voltage
- **Check polarity** before connecting batteries
- **Do not short circuit** battery terminals
- **Monitor battery temperature** during testing
- **Use appropriate fuses** for protection
- **Never leave batteries unattended** while testing
- **Follow LiPo safety guidelines** at all times

## Troubleshooting

### Display not working
- Check I2C connections (SDA, SCL)
- Verify I2C address (0x3C is common, but some displays use 0x3D)
- Check power supply (3.3V)

### Incorrect cell detection
- Verify voltage divider resistor values
- Check ADC pin connection
- Ensure battery voltage is within valid range
- Run unit tests to verify algorithm

### No voltage reading
- Check battery connection and polarity
- Verify voltage divider circuit
- Test with multimeter to confirm actual voltage
- Check ADC_PIN configuration

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

This project is open source and available under the MIT License.

## Author

Edilson Correa
- GitHub: [@edilsoncorrea](https://github.com/edilsoncorrea)

## Acknowledgments

- Built with [PlatformIO](https://platformio.org/)
- Uses [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library) and [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306) libraries
- Designed for ESP32-C3 platform

## Version History

### v1.0 (Initial Release)
- Automatic cell detection (1S-6S)
- OLED display support
- Configurable debug levels
- Comprehensive unit tests
- Safe voltage divider circuit design
