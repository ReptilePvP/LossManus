/**
 * Enhanced Loss Prevention Log
 * Hardware Abstraction Layer - Power Management Interface
 * 
 * This file contains the interface for power management operations
 */

#ifndef HAL_POWER_H
#define HAL_POWER_H

#include <M5Unified.h>
#include "../config.h"

class PowerHAL {
public:
    /**
     * Initialize the power management system
     * @return true if successful, false otherwise
     */
    static bool init();
    
    /**
     * Get battery level
     * @return battery level percentage (0-100)
     */
    static int getBatteryLevel();
    
    /**
     * Get battery voltage
     * @return battery voltage in millivolts
     */
    static int getBatteryVoltage();
    
    /**
     * Check if device is charging
     * @return true if charging, false otherwise
     */
    static bool isCharging();
    
    /**
     * Enter low power mode
     * @return true if successful, false otherwise
     */
    static bool enterLowPowerMode();
    
    /**
     * Exit low power mode
     * @return true if successful, false otherwise
     */
    static bool exitLowPowerMode();
    
    /**
     * Check if device is in low power mode
     * @return true if in low power mode, false otherwise
     */
    static bool isInLowPowerMode();
    
    /**
     * Set CPU frequency
     * @param freq frequency in MHz (80, 160, 240)
     * @return true if successful, false otherwise
     */
    static bool setCpuFrequency(int freq);
    
    /**
     * Get CPU frequency
     * @return frequency in MHz
     */
    static int getCpuFrequency();
    
    /**
     * Update power status (call in loop)
     */
    static void update();

private:
    static bool _initialized;
    static bool _lowPowerMode;
    static uint32_t _lastActivityTime;
};

#endif // HAL_POWER_H
