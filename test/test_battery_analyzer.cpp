#include <unity.h>

// Define UNIT_TEST before including anything
#define UNIT_TEST

// Include config first to ensure constants are defined
#include "../include/config.h"

// Include only the battery analyzer implementation
#include "../include/BatteryAnalyzer.h"
#include "../src/BatteryAnalyzer.cpp"

// Test cell detection for 1S battery
void test_detect_1S_battery() {
    // First, let's verify our constants are correct
    // This will help debug the cell detection issues
    // CELL_VOLTAGE_MIN should be 2.9
    // CELL_VOLTAGE_MAX should be 4.2
    
    // 1S battery fully charged (4.2V)
    TEST_ASSERT_EQUAL(1, BatteryAnalyzer::detectCellCount(4.2));
    
    // 1S battery nominal (3.7V)
    TEST_ASSERT_EQUAL(1, BatteryAnalyzer::detectCellCount(3.7));
    
    // 1S battery empty (3.3V)
    TEST_ASSERT_EQUAL(1, BatteryAnalyzer::detectCellCount(3.3));
    
    // 1S battery minimum (2.9V)
    TEST_ASSERT_EQUAL(1, BatteryAnalyzer::detectCellCount(2.9));
}

// Test cell detection for 2S battery
void test_detect_2S_battery() {
    // 2S battery fully charged (8.4V)
    TEST_ASSERT_EQUAL(2, BatteryAnalyzer::detectCellCount(8.4));
    
    // 2S battery nominal (7.4V)
    TEST_ASSERT_EQUAL(2, BatteryAnalyzer::detectCellCount(7.4));
    
    // 2S battery low (6.6V)
    TEST_ASSERT_EQUAL(2, BatteryAnalyzer::detectCellCount(6.6));
    
    // 2S battery minimum (5.8V)
    TEST_ASSERT_EQUAL(2, BatteryAnalyzer::detectCellCount(5.8));
}

// Test cell detection for 3S battery
void test_detect_3S_battery() {
    // 3S battery fully charged (12.6V)
    TEST_ASSERT_EQUAL(3, BatteryAnalyzer::detectCellCount(12.6));
    
    // 3S battery nominal (11.1V)
    TEST_ASSERT_EQUAL(3, BatteryAnalyzer::detectCellCount(11.1));
    
    // 3S battery low (9.9V)
    TEST_ASSERT_EQUAL(3, BatteryAnalyzer::detectCellCount(9.9));
    
    // 3S battery minimum (8.7V)
    TEST_ASSERT_EQUAL(3, BatteryAnalyzer::detectCellCount(8.7));
}

// Test cell detection for 4S battery
void test_detect_4S_battery() {
    // 4S battery fully charged (16.8V)
    TEST_ASSERT_EQUAL(4, BatteryAnalyzer::detectCellCount(16.8));
    
    // 4S battery nominal (14.8V)
    TEST_ASSERT_EQUAL(4, BatteryAnalyzer::detectCellCount(14.8));
    
    // 4S battery low (13.2V)
    TEST_ASSERT_EQUAL(4, BatteryAnalyzer::detectCellCount(13.2));
    
    // 4S battery minimum (11.6V)
    TEST_ASSERT_EQUAL(4, BatteryAnalyzer::detectCellCount(11.6));
}

// Test cell detection for 5S battery
void test_detect_5S_battery() {
    // 5S battery fully charged (21.0V)
    TEST_ASSERT_EQUAL(5, BatteryAnalyzer::detectCellCount(21.0));
    
    // 5S battery nominal (18.5V)
    TEST_ASSERT_EQUAL(5, BatteryAnalyzer::detectCellCount(18.5));
    
    // 5S battery low (16.5V)
    TEST_ASSERT_EQUAL(5, BatteryAnalyzer::detectCellCount(16.5));
    
    // 5S battery minimum (14.5V)
    TEST_ASSERT_EQUAL(5, BatteryAnalyzer::detectCellCount(14.5));
}

// Test cell detection for 6S battery
void test_detect_6S_battery() {
    // 6S battery fully charged (25.2V)
    TEST_ASSERT_EQUAL(6, BatteryAnalyzer::detectCellCount(25.2));
    
    // 6S battery nominal (22.2V)
    TEST_ASSERT_EQUAL(6, BatteryAnalyzer::detectCellCount(22.2));
    
    // 6S battery low (19.8V)
    TEST_ASSERT_EQUAL(6, BatteryAnalyzer::detectCellCount(19.8));
    
    // 6S battery minimum (17.4V)
    TEST_ASSERT_EQUAL(6, BatteryAnalyzer::detectCellCount(17.4));
}

// Test invalid voltage detection (too low)
void test_detect_invalid_low_voltage() {
    // Voltage too low to be valid
    TEST_ASSERT_EQUAL(0, BatteryAnalyzer::detectCellCount(2.5));
    TEST_ASSERT_EQUAL(0, BatteryAnalyzer::detectCellCount(1.0));
    TEST_ASSERT_EQUAL(0, BatteryAnalyzer::detectCellCount(0.0));
}

// Test invalid voltage detection (too high)
void test_detect_invalid_high_voltage() {
    // Voltage too high (more than 6S max)
    TEST_ASSERT_EQUAL(0, BatteryAnalyzer::detectCellCount(26.0));
    TEST_ASSERT_EQUAL(0, BatteryAnalyzer::detectCellCount(30.0));
}

// Test average cell voltage calculation
void test_calculate_average_cell_voltage() {
    TEST_ASSERT_FLOAT_WITHIN(0.01, 4.2, BatteryAnalyzer::calculateAverageCellVoltage(4.2, 1));
    TEST_ASSERT_FLOAT_WITHIN(0.01, 4.2, BatteryAnalyzer::calculateAverageCellVoltage(8.4, 2));
    TEST_ASSERT_FLOAT_WITHIN(0.01, 4.2, BatteryAnalyzer::calculateAverageCellVoltage(12.6, 3));
    TEST_ASSERT_FLOAT_WITHIN(0.01, 3.7, BatteryAnalyzer::calculateAverageCellVoltage(14.8, 4));
    TEST_ASSERT_FLOAT_WITHIN(0.01, 3.7, BatteryAnalyzer::calculateAverageCellVoltage(18.5, 5));
    TEST_ASSERT_FLOAT_WITHIN(0.01, 3.7, BatteryAnalyzer::calculateAverageCellVoltage(22.2, 6));
}

// Test charge percentage calculation
void test_calculate_charge_percentage() {
    // Full charge (4.2V)
    TEST_ASSERT_EQUAL(100, BatteryAnalyzer::calculateChargePercentage(4.2));
    
    // Empty (3.3V)
    TEST_ASSERT_EQUAL(0, BatteryAnalyzer::calculateChargePercentage(3.3));
    
    // Mid-range (3.75V) - approximately 50%
    int midPercentage = BatteryAnalyzer::calculateChargePercentage(3.75);
    TEST_ASSERT_INT_WITHIN(5, 50, midPercentage);
    
    // Nominal (3.7V) - approximately 44%
    int nominalPercentage = BatteryAnalyzer::calculateChargePercentage(3.7);
    TEST_ASSERT_INT_WITHIN(5, 44, nominalPercentage);
    
    // Below empty should return 0
    TEST_ASSERT_EQUAL(0, BatteryAnalyzer::calculateChargePercentage(3.0));
    
    // Above full should return 100
    TEST_ASSERT_EQUAL(100, BatteryAnalyzer::calculateChargePercentage(4.5));
}

// Test complete battery analysis for various scenarios
void test_analyze_battery_1S() {
    BatteryInfo info = BatteryAnalyzer::analyzeBattery(3.7);
    
    TEST_ASSERT_TRUE(info.isValid);
    TEST_ASSERT_EQUAL(1, info.cellCount);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 3.7, info.totalVoltage);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 3.7, info.averageCellVoltage);
    TEST_ASSERT_INT_WITHIN(5, 44, info.chargePercentage);
}

void test_analyze_battery_3S() {
    BatteryInfo info = BatteryAnalyzer::analyzeBattery(11.1);
    
    TEST_ASSERT_TRUE(info.isValid);
    TEST_ASSERT_EQUAL(3, info.cellCount);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 11.1, info.totalVoltage);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 3.7, info.averageCellVoltage);
    TEST_ASSERT_INT_WITHIN(5, 44, info.chargePercentage);
}

void test_analyze_battery_6S() {
    BatteryInfo info = BatteryAnalyzer::analyzeBattery(25.2);
    
    TEST_ASSERT_TRUE(info.isValid);
    TEST_ASSERT_EQUAL(6, info.cellCount);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 25.2, info.totalVoltage);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 4.2, info.averageCellVoltage);
    TEST_ASSERT_EQUAL(100, info.chargePercentage);
}

void test_analyze_battery_invalid() {
    BatteryInfo info = BatteryAnalyzer::analyzeBattery(0.5);
    
    TEST_ASSERT_FALSE(info.isValid);
    TEST_ASSERT_EQUAL(0, info.cellCount);
}

// Test voltage validation
void test_voltage_validation() {
    // Valid voltages
    TEST_ASSERT_TRUE(BatteryAnalyzer::isVoltageValid(4.2, 1));
    TEST_ASSERT_TRUE(BatteryAnalyzer::isVoltageValid(8.4, 2));
    TEST_ASSERT_TRUE(BatteryAnalyzer::isVoltageValid(12.6, 3));
    
    // Invalid voltages
    TEST_ASSERT_FALSE(BatteryAnalyzer::isVoltageValid(5.0, 1)); // Too high for 1S
    TEST_ASSERT_FALSE(BatteryAnalyzer::isVoltageValid(2.0, 1)); // Too low for 1S
    TEST_ASSERT_FALSE(BatteryAnalyzer::isVoltageValid(12.6, 0)); // Invalid cell count
    TEST_ASSERT_FALSE(BatteryAnalyzer::isVoltageValid(12.6, 7)); // Invalid cell count
}

// Main test runner
int main(int argc, char **argv) {
    UNITY_BEGIN();
    
    // Cell detection tests
    RUN_TEST(test_detect_1S_battery);
    RUN_TEST(test_detect_2S_battery);
    RUN_TEST(test_detect_3S_battery);
    RUN_TEST(test_detect_4S_battery);
    RUN_TEST(test_detect_5S_battery);
    RUN_TEST(test_detect_6S_battery);
    RUN_TEST(test_detect_invalid_low_voltage);
    RUN_TEST(test_detect_invalid_high_voltage);
    
    // Calculation tests
    RUN_TEST(test_calculate_average_cell_voltage);
    RUN_TEST(test_calculate_charge_percentage);
    
    // Complete analysis tests
    RUN_TEST(test_analyze_battery_1S);
    RUN_TEST(test_analyze_battery_3S);
    RUN_TEST(test_analyze_battery_6S);
    RUN_TEST(test_analyze_battery_invalid);
    
    // Validation tests
    RUN_TEST(test_voltage_validation);
    
    return UNITY_END();
}
