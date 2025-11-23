#define SIMULATOR
#include <iostream>
#include <iomanip>
#include <cmath>
#include <thread>
#include <chrono>

// Simulated configuration
const float VOLTAGE_DIVIDER_RATIO = 7.8f;
const float CELL_VOLTAGE_MIN = 2.9f;
const float CELL_VOLTAGE_MAX = 4.2f;
const float REFERENCE_VOLTAGE = 3.3f;
const int ADC_RESOLUTION = 4095;

/**
 * @brief Simulated Battery Analyzer
 * 
 * Implements the same cell detection algorithm as the ESP32 version
 * but runs on PC for testing and demonstration purposes.
 */
class SimulatedBatteryAnalyzer {
private:
    float totalVoltage;
    int detectedCells;
    float avgVoltagePerCell;
    int chargePercent;

    /**
     * @brief Calculate charge percentage based on average cell voltage
     * 
     * Uses linear interpolation between MIN (2.9V = 0%) and MAX (4.2V = 100%)
     */
    int calculateChargePercent(float avgVoltage) {
        if (avgVoltage <= CELL_VOLTAGE_MIN) return 0;
        if (avgVoltage >= CELL_VOLTAGE_MAX) return 100;
        
        float range = CELL_VOLTAGE_MAX - CELL_VOLTAGE_MIN;
        float position = avgVoltage - CELL_VOLTAGE_MIN;
        return (int)((position / range) * 100.0f);
    }

public:
    SimulatedBatteryAnalyzer() : totalVoltage(0), detectedCells(0), avgVoltagePerCell(0), chargePercent(0) {}

    /**
     * @brief Detect number of cells using "First Valid Match" algorithm
     * 
     * Same algorithm as BatteryAnalyzer.cpp - tests 1S through 6S sequentially
     * and returns the first configuration where voltage per cell is valid.
     */
    int detectCellCount(float voltage) {
        const float TOLERANCE = 0.001f;  // 1mV tolerance for floating-point
        
        for (int cells = 1; cells <= 6; cells++) {
            float avgVoltage = voltage / cells;
            
            if (avgVoltage >= (CELL_VOLTAGE_MIN - TOLERANCE) && 
                avgVoltage <= (CELL_VOLTAGE_MAX + TOLERANCE)) {
                return cells;
            }
        }
        
        return 0;  // Invalid voltage
    }

    /**
     * @brief Analyze battery with given voltage
     */
    void analyze(float voltage) {
        totalVoltage = voltage;
        detectedCells = detectCellCount(voltage);
        
        if (detectedCells > 0) {
            avgVoltagePerCell = totalVoltage / detectedCells;
            chargePercent = calculateChargePercent(avgVoltagePerCell);
        } else {
            avgVoltagePerCell = 0;
            chargePercent = 0;
        }
    }

    /**
     * @brief Display results in console (simulating OLED display)
     */
    void displayResults() {
        std::cout << "\n╔════════════════════════════════╗\n";
        std::cout << "║   LiPo Battery Tester (SIM)   ║\n";
        std::cout << "╠════════════════════════════════╣\n";
        
        if (detectedCells == 0) {
            std::cout << "║ ERROR: Invalid Voltage!        ║\n";
            std::cout << "║ Total: " << std::fixed << std::setprecision(2) 
                      << std::setw(7) << totalVoltage << "V                ║\n";
        } else {
            std::cout << "║ Cells: " << detectedCells << "S                       ║\n";
            std::cout << "║ Total: " << std::fixed << std::setprecision(2) 
                      << std::setw(7) << totalVoltage << "V                ║\n";
            
            if (detectedCells > 1) {
                std::cout << "║ Average: " << std::setw(5) << avgVoltagePerCell << "V/cell          ║\n";
            }
            
            std::cout << "║ Charge: " << std::setw(3) << chargePercent << "%";
            
            // Draw charge bar (20 characters wide)
            std::cout << " [";
            int bars = (chargePercent * 20) / 100;
            for (int i = 0; i < 20; i++) {
                std::cout << (i < bars ? "█" : " ");
            }
            std::cout << "]║\n";
        }
        
        std::cout << "╚════════════════════════════════╝\n";
    }

    // Getters for testing
    int getCells() const { return detectedCells; }
    float getAvgVoltage() const { return avgVoltagePerCell; }
    int getCharge() const { return chargePercent; }
};

/**
 * @brief Simulate ADC reading from actual battery voltage
 * 
 * Converts real voltage to ADC value (as if measured through voltage divider)
 * then back to voltage (simulating the ESP32 measurement process)
 */
float simulateADCReading(float actualVoltage) {
    // Simulate voltage divider reduction
    float dividedVoltage = actualVoltage / VOLTAGE_DIVIDER_RATIO;
    
    // Simulate ADC quantization
    int adcValue = (int)((dividedVoltage / REFERENCE_VOLTAGE) * ADC_RESOLUTION);
    
    // Add some noise (±2 ADC counts)
    int noise = (rand() % 5) - 2;
    adcValue += noise;
    
    // Clamp to valid range
    if (adcValue < 0) adcValue = 0;
    if (adcValue > ADC_RESOLUTION) adcValue = ADC_RESOLUTION;
    
    // Convert back to voltage (with voltage divider compensation)
    float measuredVoltage = ((float)adcValue / ADC_RESOLUTION) * REFERENCE_VOLTAGE * VOLTAGE_DIVIDER_RATIO;
    
    return measuredVoltage;
}

/**
 * @brief Run interactive simulation mode
 */
void runInteractiveMode() {
    SimulatedBatteryAnalyzer analyzer;
    float voltage;
    
    std::cout << "\n=== LiPo Battery Tester Simulator ===\n";
    std::cout << "Enter battery voltage (0 to quit): ";
    
    while (std::cin >> voltage && voltage > 0) {
        // Simulate ADC measurement
        float measured = simulateADCReading(voltage);
        
        std::cout << "\nActual voltage: " << std::fixed << std::setprecision(3) 
                  << voltage << "V\n";
        std::cout << "Measured voltage: " << measured << "V (with ADC noise)\n";
        
        analyzer.analyze(measured);
        analyzer.displayResults();
        
        std::cout << "\nEnter battery voltage (0 to quit): ";
    }
}

/**
 * @brief Run automated demo with predefined test cases
 */
void runDemoMode() {
    SimulatedBatteryAnalyzer analyzer;
    
    std::cout << "\n=== LiPo Battery Tester - Demo Mode ===\n";
    std::cout << "Running automated test cases...\n";
    
    // Test cases: {voltage, description}
    struct TestCase {
        float voltage;
        const char* description;
    };
    
    TestCase testCases[] = {
        {3.7f, "1S LiPo - Nominal voltage"},
        {4.2f, "1S LiPo - Fully charged"},
        {3.3f, "1S LiPo - Low battery"},
        {7.4f, "2S LiPo - Nominal voltage"},
        {8.4f, "2S LiPo - Fully charged"},
        {11.1f, "3S LiPo - Nominal voltage"},
        {12.6f, "3S LiPo - Fully charged"},
        {11.6f, "3S LiPo - Ambiguous case (could be 4S @ 2.9V)"},
        {14.8f, "4S LiPo - Nominal voltage"},
        {16.8f, "4S LiPo - Fully charged"},
        {18.5f, "5S LiPo - Nominal voltage"},
        {22.2f, "6S LiPo - Nominal voltage"},
        {25.2f, "6S LiPo - Fully charged"},
        {1.5f, "Too low - Invalid"},
        {26.0f, "Too high - Invalid"}
    };
    
    for (const auto& test : testCases) {
        std::cout << "\n----------------------------------------\n";
        std::cout << "Test: " << test.description << "\n";
        std::cout << "Input: " << std::fixed << std::setprecision(2) 
                  << test.voltage << "V\n";
        
        analyzer.analyze(test.voltage);
        analyzer.displayResults();
        
        // Pause between tests
        std::this_thread::sleep_for(std::chrono::milliseconds(800));
    }
    
    std::cout << "\n========================================\n";
    std::cout << "Demo completed!\n";
}

/**
 * @brief Run continuous monitoring simulation
 * 
 * Simulates a battery discharging over time
 */
void runMonitoringMode() {
    SimulatedBatteryAnalyzer analyzer;
    
    std::cout << "\n=== Battery Discharge Simulation ===\n";
    std::cout << "Simulating 3S LiPo discharge from 12.6V to 9.9V\n";
    std::cout << "Press Ctrl+C to stop...\n\n";
    
    float voltage = 12.6f;  // Start fully charged (3S @ 4.2V/cell)
    const float dischargeRate = 0.05f;  // 50mV per iteration
    
    while (voltage >= 9.9f) {  // Stop at 3.3V/cell
        float measured = simulateADCReading(voltage);
        
        analyzer.analyze(measured);
        
        // Clear screen (cross-platform)
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
        
        std::cout << "=== Real-time Battery Monitor ===\n";
        std::cout << "Simulated voltage: " << std::fixed << std::setprecision(2) 
                  << voltage << "V\n";
        
        analyzer.displayResults();
        
        std::cout << "\nDischarging... (-" << dischargeRate << "V/iteration)\n";
        
        // Update for next iteration
        voltage -= dischargeRate;
        
        // Wait 1 second
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    std::cout << "\nBattery discharged! Simulation stopped.\n";
}

/**
 * @brief Main entry point
 */
int main(int argc, char* argv[]) {
    srand(time(nullptr));
    
    if (argc > 1) {
        std::string mode = argv[1];
        
        if (mode == "demo") {
            runDemoMode();
        } else if (mode == "monitor") {
            runMonitoringMode();
        } else if (mode == "interactive") {
            runInteractiveMode();
        } else {
            std::cout << "Unknown mode: " << mode << "\n";
            std::cout << "Available modes: demo, monitor, interactive\n";
            return 1;
        }
    } else {
        // Default: show menu
        std::cout << "\n=== LiPo Battery Tester Simulator ===\n\n";
        std::cout << "Select mode:\n";
        std::cout << "1. Demo mode (automated test cases)\n";
        std::cout << "2. Interactive mode (enter voltages manually)\n";
        std::cout << "3. Monitor mode (discharge simulation)\n";
        std::cout << "\nChoice (1-3): ";
        
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                runDemoMode();
                break;
            case 2:
                runInteractiveMode();
                break;
            case 3:
                runMonitoringMode();
                break;
            default:
                std::cout << "Invalid choice!\n";
                return 1;
        }
    }
    
    return 0;
}
