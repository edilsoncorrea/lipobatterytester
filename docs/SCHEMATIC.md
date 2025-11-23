# LiPo Battery Tester - Schematic

## Circuit Connections

### Voltage Divider Circuit
```
                    Battery Pack
                    (+) ────────────┐
                                    │
                                   ┌┴┐
                                   │ │ R1 = 68kΩ
                                   │ │
                                   └┬┘
                                    │
                    ADC ────────────┤ (GPIO0)
                    (ESP32-C3)      │
                                   ┌┴┐
                                   │ │ R2 = 10kΩ
                                   │ │
                                   └┬┘
                                    │
                    (-) ────────────┴──── GND
                    Battery Pack
```

### OLED Display (I2C)
```
ESP32-C3          OLED Display
───────────       ────────────
GPIO8    ─────── SDA
GPIO9    ─────── SCL
3.3V     ─────── VCC
GND      ─────── GND
```

### Complete Pinout (ESP32-C3)

| ESP32-C3 Pin | Function        | Connected To           |
|--------------|-----------------|------------------------|
| GPIO0        | ADC Input       | Voltage Divider Output |
| GPIO8        | I2C SDA         | OLED SDA               |
| GPIO9        | I2C SCL         | OLED SCL               |
| 3.3V         | Power           | OLED VCC               |
| GND          | Ground          | OLED GND, R2, Battery (-) |

## Component Specifications

### Resistors (Voltage Divider)
- **R1**: 68kΩ, 1/4W or higher, ±1% tolerance recommended
- **R2**: 10kΩ, 1/4W or higher, ±1% tolerance recommended

**Note**: Use resistors with good tolerance (1% or better) for accurate voltage readings.

### OLED Display
- **Type**: SSD1306 based OLED
- **Size**: 0.91 inch (128x32 pixels)
- **Interface**: I2C
- **I2C Address**: Usually 0x3C (some models use 0x3D)
- **Operating Voltage**: 3.3V - 5V

### ESP32-C3 Development Board
- **Model**: ESP32-C3-DevKitM-1 or compatible
- **ADC Resolution**: 12-bit (0-4095)
- **ADC Reference Voltage**: 3.3V
- **I2C Support**: Hardware I2C available on any GPIO

## Voltage Calculations

### Voltage Divider Formula
```
Vout = Vin × (R2 / (R1 + R2))
```

With R1 = 68kΩ and R2 = 10kΩ:
```
Vout = Vin × (10kΩ / (68kΩ + 10kΩ))
Vout = Vin × (10kΩ / 78kΩ)
Vout = Vin × 0.1282
```

### Maximum Input Voltages
```
For ADC max voltage of 3.3V:
Vin_max = 3.3V / 0.1282 = 25.74V

Battery configurations:
1S: 4.2V  → ADC sees: 0.54V  ✓
2S: 8.4V  → ADC sees: 1.08V  ✓
3S: 12.6V → ADC sees: 1.62V  ✓
4S: 16.8V → ADC sees: 2.15V  ✓
5S: 21.0V → ADC sees: 2.69V  ✓
6S: 25.2V → ADC sees: 3.23V  ✓ (safe margin)
```

## Assembly Tips

1. **Solder resistors first**: Use good quality resistors with accurate values
2. **Use heat shrink**: Protect voltage divider connections
3. **Secure connections**: Use terminal blocks or quality connectors for battery input
4. **Add fuse**: Consider adding a fuse on the positive battery input for safety
5. **Label polarity**: Clearly mark positive and negative terminals
6. **Cable management**: Keep wiring neat to avoid shorts

## PCB Layout Recommendations (Optional)

If designing a PCB:
- Keep voltage divider traces short
- Add test points for calibration
- Include reverse polarity protection (diode)
- Add overvoltage protection (TVS diode)
- Include status LED
- Add screw terminals for battery connection
- Ground plane for noise reduction
- Keep I2C traces short and add pull-up resistors if needed (usually built into OLED module)

## Safety Features to Add

1. **Reverse Polarity Protection**: Add a Schottky diode or P-channel MOSFET
2. **Overvoltage Protection**: TVS diode or zener clamp at ADC input
3. **Fuse**: 1A fast-blow fuse on battery positive input
4. **Indicator LED**: Shows when device is powered and measuring

## Calibration

To calibrate the voltage readings:
1. Measure actual battery voltage with a quality multimeter
2. Compare with reading from device
3. Adjust `VOLTAGE_DIVIDER_R1` and `VOLTAGE_DIVIDER_R2` values in `config.h` to match measured resistor values
4. Or add a calibration factor to the voltage calculation
