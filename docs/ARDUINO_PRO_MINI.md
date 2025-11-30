# Arduino Pro Mini Configuration

## Hardware Differences

| Feature | ESP32-C3 | Arduino Pro Mini |
|---------|-----------|------------------|
| **Microcontroller** | ESP32-C3 (RISC-V) | ATmega328P |
| **Voltage** | 3.3V | 5V |
| **ADC Resolution** | 12-bit (0-4095) | 10-bit (0-1023) |
| **ADC Reference** | 3.3V (calibrated 2.962V) | 5V |
| **Clock Speed** | 160MHz | 8MHz |
| **Flash** | 4MB | 32KB |
| **RAM** | 400KB | 2KB |

## Pin Connections

### Arduino Pro Mini Pinout

```
Battery (+) ----[R1: 67.2kΩ]----+----[R2: 10.05kΩ]---- GND
                                |
                                +---- A0 (Analog Pin 0)

OLED SDA ---- A4
OLED SCL ---- A5  
OLED VCC ---- VCC (5V or 3.3V)
OLED GND ---- GND
```

### Voltage Divider Calculation for Arduino Pro Mini

- **Input Voltage Range**: Up to 38.43V (safe for 6S+ batteries)
- **ADC Input Range**: 0V to 5V
- **Ratio**: (67.2kΩ + 10.05kΩ) / 10.05kΩ = 7.6866
- **Max Measurable**: 5V × 7.6866 = 38.43V

### Key Differences in Code

1. **ADC Reading**:
   - ESP32-C3: `analogRead()` returns 0-4095
   - Pro Mini: `analogRead()` returns 0-1023

2. **I2C Initialization**:
   - ESP32-C3: `Wire.begin(SDA_PIN, SCL_PIN)`
   - Pro Mini: `Wire.begin()` (uses default A4/A5)

3. **Memory Constraints**:
   - Pro Mini has only 2KB RAM vs 400KB on ESP32
   - Debug output may need to be reduced
   - String constants should be stored in PROGMEM

## Building for Arduino Pro Mini

### PlatformIO Commands

```bash
# Build for Arduino Pro Mini
pio run -e pro-mini

# Upload to Arduino Pro Mini  
pio run -e pro-mini -t upload

# Monitor serial output
pio run -e pro-mini -t monitor

# Build for both platforms
pio run
```

### Expected Performance

- **Accuracy**: Same algorithm, same accuracy as ESP32 version
- **Speed**: Slower due to 8MHz vs 160MHz (measurements every 1 second vs 0.5 seconds)
- **Debug**: Reduced verbosity to save memory

## Troubleshooting

### Common Issues

1. **Upload Problems**:
   - Ensure correct USB-to-Serial adapter (FTDI, CP2102, etc.)
   - Press reset button during upload if needed
   - Check baud rate (115200)

2. **Display Not Working**:
   - Verify I2C connections (A4=SDA, A5=SCL)
   - Check display voltage (3.3V or 5V compatible)
   - Test I2C scanner code if needed

3. **Incorrect Readings**:
   - Same voltage divider calibration as ESP32
   - Verify 5V reference voltage is stable
   - Check resistor values (67.2kΩ and 10.05kΩ)

### Memory Optimization

For Arduino Pro Mini's limited memory:

1. **Reduce Debug Output**:
   ```cpp
   #define DEBUG_VERBOSITY DEBUG_LEVEL_DISPLAY  // Instead of RAW
   ```

2. **Use PROGMEM for Strings**:
   ```cpp
   const char string_table[] PROGMEM = "Text in flash memory";
   ```

3. **Optimize Display Updates**:
   - Update less frequently
   - Use smaller fonts if needed

## Testing

### Verification Steps

1. **Power Supply**: Ensure stable 5V supply (not USB power for high-current)
2. **Voltage Divider**: Same 15V input should read ~15.0V
3. **Cell Detection**: Test with 1S-6S batteries
4. **Display**: Verify all information displays correctly
5. **Serial Output**: Check debug messages at 115200 baud

### Expected Results

With proper calibration, the Arduino Pro Mini version should provide identical functionality to the ESP32-C3 version, just at a slower update rate due to hardware limitations.

---

*Arduino Pro Mini support added November 29, 2025*