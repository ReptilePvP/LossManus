/**
 * Enhanced Loss Prevention Log
 * Hardware Abstraction Layer - WiFi Hardware Interface
 * 
 * This file contains the interface for WiFi hardware operations
 */

#ifndef HAL_WIFI_HARDWARE_H
#define HAL_WIFI_HARDWARE_H

#include <WiFi.h>
#include "../config.h"

class WiFiHAL {
public:
    /**
     * Initialize the WiFi hardware
     * @return true if successful, false otherwise
     */
    static bool init();
    
    /**
     * Enable WiFi
     * @return true if successful, false otherwise
     */
    static bool enable();
    
    /**
     * Disable WiFi
     * @return true if successful, false otherwise
     */
    static bool disable();
    
    /**
     * Check if WiFi is enabled
     * @return true if enabled, false otherwise
     */
    static bool isEnabled();
    
    /**
     * Get WiFi signal strength
     * @return signal strength in dBm
     */
    static int getSignalStrength();
    
    /**
     * Get WiFi MAC address
     * @return MAC address as string
     */
    static String getMacAddress();
    
    /**
     * Get current IP address
     * @return IP address as string
     */
    static String getIPAddress();
    
    /**
     * Scan for available networks
     * @param callback function to call with scan results
     */
    static void scanNetworks(void (*callback)(int numNetworks));
    
    /**
     * Get SSID of network at index
     * @param index network index
     * @return SSID as string
     */
    static String getSSID(int index);
    
    /**
     * Get RSSI of network at index
     * @param index network index
     * @return RSSI in dBm
     */
    static int getRSSI(int index);
    
    /**
     * Get encryption type of network at index
     * @param index network index
     * @return encryption type
     */
    static uint8_t getEncryptionType(int index);
    
    /**
     * Get number of networks found in last scan
     * @return number of networks
     */
    static int getNetworkCount();

private:
    static bool _initialized;
    static bool _enabled;
};

#endif // HAL_WIFI_HARDWARE_H
