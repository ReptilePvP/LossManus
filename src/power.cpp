/**
 * Enhanced Loss Prevention Log
 * Hardware Abstraction Layer - Power Management Implementation
 */

#include "power.h"

// Static member initialization
bool PowerHAL::_initialized = false;
bool PowerHAL::_lowPowerMode = false;
uint32_t PowerHAL::_lastActivityTime = 0;

bool PowerHAL::init() {
    DEBUG_PRINT("Initializing power management...");
    
    if (_initialized) {
        DEBUG_PRINT("Power management already initialized");
        return true;
    }
    
    // Initialize M5 Power
    M5.Power.begin();
    
    // Enable external bus power
    M5.Power.setExtOutput(true);
    DEBUG_PRINTF("External bus power enabled: %s\n", M5.Power.getExtOutput() ? "Yes" : "No");
    
    // Configure ALDO3 to 3.3V
    M5.Power.Axp2101.writeRegister8(0x94, 28); // Set ALDO3 to 3.3V
    uint8_t reg90 = M5.Power.Axp2101.readRegister8(0x90);
    M5.Power.Axp2101.writeRegister8(0x90, reg90 | 0x08); // Enable ALDO3
    DEBUG_PRINT("ALDO3 configured to 3.3V");
    
    // Set CPU frequency to default
    setCpuFrequency(240);
    
    _initialized = true;
    _lastActivityTime = millis();
    
    DEBUG_PRINT("Power management initialization complete");
    return true;
}

int PowerHAL::getBatteryLevel() {
    if (!_initialized) {
        return 0;
    }
    
    return M5.Power.getBatteryLevel();
}

int PowerHAL::getBatteryVoltage() {
    if (!_initialized) {
        return 0;
    }
    
    return M5.Power.getBatteryVoltage();
}

bool PowerHAL::isCharging() {
    if (!_initialized) {
        return false;
    }
    
    return M5.Power.isCharging();
}

bool PowerHAL::enterLowPowerMode() {
    if (!_initialized || _lowPowerMode) {
        return false;
    }
    
    DEBUG_PRINT("Entering low power mode");
    
    // Reduce CPU frequency
    setCpuFrequency(80);
    
    _lowPowerMode = true;
    return true;
}

bool PowerHAL::exitLowPowerMode() {
    if (!_initialized || !_lowPowerMode) {
        return false;
    }
    
    DEBUG_PRINT("Exiting low power mode");
    
    // Restore CPU frequency
    setCpuFrequency(240);
    
    _lowPowerMode = false;
    _lastActivityTime = millis();
    return true;
}

bool PowerHAL::isInLowPowerMode() {
    return _lowPowerMode;
}

bool PowerHAL::setCpuFrequency(int freq) {
    if (!_initialized) {
        return false;
    }
    
    // Validate frequency
    if (freq != 80 && freq != 160 && freq != 240) {
        DEBUG_PRINTF("Invalid CPU frequency: %d MHz\n", freq);
        return false;
    }
    
    setCpuFrequencyMhz(freq);
    DEBUG_PRINTF("CPU frequency set to %d MHz\n", freq);
    return true;
}

int PowerHAL::getCpuFrequency() {
    return getCpuFrequencyMhz();
}

void PowerHAL::update() {
    if (!_initialized) {
        return;
    }
    
    // Check for inactivity to enter low power mode
    if (!_lowPowerMode && (millis() - _lastActivityTime > POWER_SAVE_TIMEOUT)) {
        enterLowPowerMode();
    }
    
    // Update battery status periodically
    static uint32_t lastBatteryCheck = 0;
    if (millis() - lastBatteryCheck > 60000) { // Check every minute
        int batteryLevel = getBatteryLevel();
        int batteryVoltage = getBatteryVoltage();
        bool charging = isCharging();
        
        DEBUG_PRINTF("Battery: %d%%, %dmV, %s\n", 
                    batteryLevel, 
                    batteryVoltage,
                    charging ? "Charging" : "Not charging");
        
        lastBatteryCheck = millis();
    }
}
