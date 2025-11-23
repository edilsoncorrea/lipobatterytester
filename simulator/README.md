# LiPo Battery Tester - Simulator

This simulator allows you to test the battery detection algorithm on your PC without an ESP32-C3 hardware.

## Features

- **Same Algorithm**: Uses the exact same cell detection logic as the ESP32 version
- **ADC Simulation**: Simulates the voltage divider and ADC quantization/noise
- **Multiple Modes**: Demo, Interactive, and Real-time monitoring
- **Visual Output**: Console-based display mimicking the OLED screen

## Building the Simulator

### Windows (Visual Studio)

```cmd
cd simulator
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Windows (MinGW)

```cmd
cd simulator
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
```

### Linux/Mac

```bash
cd simulator
mkdir build
cd build
cmake ..
make
```

## Running the Simulator

### Mode 1: Demo Mode (Recommended for first time)

Runs predefined test cases automatically:

```cmd
cd build
.\Release\lipo_simulator.exe demo
```

Or on Linux/Mac:
```bash
./lipo_simulator demo
```

**Output:**
- Tests 15 different voltage scenarios
- Shows how the algorithm handles each case
- Demonstrates ambiguous cases (e.g., 11.6V)

### Mode 2: Interactive Mode

Enter voltages manually:

```cmd
.\Release\lipo_simulator.exe interactive
```

**Example session:**
```
Enter battery voltage (0 to quit): 11.1
Actual voltage: 11.100V
Measured voltage: 11.098V (with ADC noise)

╔════════════════════════════════╗
║   LiPo Battery Tester (SIM)   ║
╠════════════════════════════════╣
║ Cells: 3S                       ║
║ Total:  11.10V                ║
║ Average:  3.70V/cell          ║
║ Charge:  61% [████████████        ]║
╚════════════════════════════════╝
```

### Mode 3: Monitor Mode

Simulates a battery discharging in real-time:

```cmd
.\Release\lipo_simulator.exe monitor
```

**Features:**
- Starts at 12.6V (3S fully charged)
- Discharges to 9.9V (3S low battery)
- Updates every 1 second
- Shows live voltage, cell count, and charge percentage

### No Arguments (Menu Mode)

Run without arguments to see an interactive menu:

```cmd
.\Release\lipo_simulator.exe
```

## Test Cases in Demo Mode

| Voltage | Expected Result | Description |
|---------|----------------|-------------|
| 3.7V | 1S @ 3.7V (88%) | Nominal 1S |
| 4.2V | 1S @ 4.2V (100%) | Fully charged 1S |
| 7.4V | 2S @ 3.7V (61%) | Nominal 2S |
| 11.1V | 3S @ 3.7V (61%) | Nominal 3S |
| 12.6V | 3S @ 4.2V (100%) | Fully charged 3S |
| **11.6V** | **3S @ 3.87V (74%)** | **Ambiguous case** |
| 14.8V | 4S @ 3.7V (61%) | Nominal 4S |
| 22.2V | 6S @ 3.7V (61%) | Nominal 6S |
| 25.2V | 6S @ 4.2V (100%) | Fully charged 6S |
| 1.5V | Invalid | Too low |
| 26.0V | Invalid | Too high |

## Simulator Features

### ADC Noise Simulation

The simulator adds realistic ADC noise (±2 counts) to mimic actual hardware behavior:

```cpp
int noise = (rand() % 5) - 2;  // Random ±2 ADC counts
adcValue += noise;
```

This means the same voltage input may produce slightly different results each time, just like the real hardware.

### Voltage Divider Simulation

Correctly simulates the 68kΩ + 10kΩ voltage divider (7.8:1 ratio):

```
Battery Voltage → ÷7.8 → ADC Input → Quantized → ×7.8 → Measured Voltage
```

### Charge Bar Visualization

The simulator displays a visual charge indicator:

```
Charge: 100% [████████████████████]  ← Fully charged
Charge:  61% [████████████        ]  ← Medium charge
Charge:  15% [███                 ]  ← Low battery
Charge:   0% [                    ]  ← Critical
```

## Comparing with Hardware

The simulator helps you:

1. **Test edge cases** without risking battery damage
2. **Verify algorithm behavior** before deploying to hardware
3. **Demonstrate the project** without physical setup
4. **Debug issues** faster than with hardware

## Limitations

The simulator does **not** include:

- ❌ I2C communication (no real OLED)
- ❌ ESP32-specific peripherals
- ❌ Power management features
- ❌ WiFi/Bluetooth capabilities

It **only** simulates:

- ✅ Voltage measurement (ADC + voltage divider)
- ✅ Cell detection algorithm
- ✅ Charge percentage calculation
- ✅ Display logic (text-based)

## Source Code

The simulator is a single-file C++ program (`main.cpp`) that reimplements:

- `SimulatedBatteryAnalyzer::detectCellCount()` - Same as `BatteryAnalyzer.cpp`
- `SimulatedBatteryAnalyzer::calculateChargePercent()` - Same calculation logic
- `simulateADCReading()` - Mimics ESP32 ADC with noise

## Troubleshooting

### Build Errors

**Problem:** `cmake: command not found`
- **Solution:** Install CMake from https://cmake.org/download/

**Problem:** No compiler found
- **Windows:** Install Visual Studio or MinGW
- **Linux:** `sudo apt install build-essential`
- **Mac:** `xcode-select --install`

### Runtime Issues

**Problem:** Characters display incorrectly (squares/boxes)
- **Solution:** Use a terminal with UTF-8 support (Windows Terminal recommended)

**Problem:** Screen doesn't clear in monitor mode
- **Solution:** Normal behavior on some terminals, output will scroll

## Next Steps

After testing in the simulator:

1. ✅ Verify algorithm works as expected
2. ✅ Test edge cases (11.6V, 14.5V, etc.)
3. ✅ Upload to ESP32-C3 hardware
4. ✅ Test with real batteries (safely!)

---

*Simulator version 1.0 - November 23, 2025*
