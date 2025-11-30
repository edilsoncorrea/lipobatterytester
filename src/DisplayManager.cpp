#include "DisplayManager.h"

Adafruit_SSD1306* DisplayManager::display = nullptr;

bool DisplayManager::begin() {
    // Initialize I2C with platform-specific pins
#ifdef ARDUINO_PRO_MINI
    Wire.begin();  // Arduino Pro Mini uses default I2C pins (A4=SDA, A5=SCL)
#else
    Wire.begin(I2C_SDA, I2C_SCL);  // ESP32-C3 uses custom pins
#endif
    
    // Create display object
    display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
    
    // Initialize display
    if (!display->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        return false;
    }
    
    // Clear display
    display->clearDisplay();
    display->display();
    
    return true;
}

void DisplayManager::clear() {
    if (display) {
        display->clearDisplay();
        display->display();
    }
}

void DisplayManager::displayBatteryInfo(const BatteryInfo& info) {
    if (!display) return;
    
    display->clearDisplay();
    display->setTextSize(1);
    display->setTextColor(SSD1306_WHITE);
    display->setCursor(0, 0);
    
    if (!info.isValid) {
        display->println("Invalid Battery!");
        display->display();
        return;
    }
    
    // Line 1: Cell count and total voltage
    display->print(info.cellCount);
    display->print("S ");
    display->print(info.totalVoltage, 2);
    display->println("V");
    
    // Line 2: Average cell voltage or single voltage
    // If 1S, show voltage only once (avoid duplicate info)
    if (info.cellCount > 1) {
        display->print("Avg: ");
        display->print(info.averageCellVoltage, 2);
        display->println("V/cell");
    } else {
        // For 1S, the line above already shows the voltage
        // Show a different info or leave space
        display->println("");
    }
    
    // Line 3: Charge percentage with bar
    display->print("Charge: ");
    display->print(info.chargePercentage);
    display->println("%");
    
    // Line 4: Simple bar graph
    int barWidth = (info.chargePercentage * (SCREEN_WIDTH - 4)) / 100;
    display->drawRect(0, 24, SCREEN_WIDTH, 8, SSD1306_WHITE);
    display->fillRect(2, 26, barWidth, 4, SSD1306_WHITE);
    
    display->display();
}

void DisplayManager::displayError(const char* message) {
    if (!display) return;
    
    display->clearDisplay();
    display->setTextSize(1);
    display->setTextColor(SSD1306_WHITE);
    display->setCursor(0, 0);
    display->println("ERROR:");
    display->println(message);
    display->display();
}

void DisplayManager::displayInitMessage() {
    if (!display) return;
    
    display->clearDisplay();
    display->setTextSize(1);
    display->setTextColor(SSD1306_WHITE);
    display->setCursor(0, 0);
    display->println("LiPo Battery");
    display->println("Tester v1.0");
    display->println("");
    display->println("Initializing...");
    display->display();
}
