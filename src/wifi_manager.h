/**
 * Enhanced Loss Prevention Log
 * Connectivity Layer - WiFi Manager
 * 
 * This file contains the interface for WiFi connection management
 */

#ifndef CONNECTIVITY_WIFI_MANAGER_H
#define CONNECTIVITY_WIFI_MANAGER_H

#include <Arduino.h>
#include <vector>
#include "../hal/wifi_hardware.h"
#include "../config.h"

// WiFi network structure
struct WiFiNetwork {
    char ssid[33];
    char password[65];
    int8_t rssi;
    uint8_t encType;
    bool saved;
};

// WiFi manager callback function types
typedef void (*WiFiScanCallback)(const std::vector<WiFiNetwork>& networks);
typedef void (*WiFiStatusCallback)(bool connected, const String& message);

class WiFiManager {
public:
    /**
     * Initialize the WiFi manager
     * @return true if successful, false otherwise
     */
    static bool init();
    
    /**
     * Scan for available networks
     * @param callback function to call when scan is complete
     */
    static void scanNetworks(WiFiScanCallback callback);
    
    /**
     * Connect to a WiFi network
     * @param ssid network SSID
     * @param password network password
     * @param callback function to call with connection status
     * @param save whether to save the network credentials
     * @return true if connection attempt started, false otherwise
     */
    static bool connect(const String& ssid, const String& password, 
                       WiFiStatusCallback callback, bool save = true);
    
    /**
     * Connect to saved networks
     * @param callback function to call with connection status
     * @return true if connection attempt started, false otherwise
     */
    static bool connectToSavedNetworks(WiFiStatusCallback callback);
    
    /**
     * Disconnect from current network
     * @return true if successful, false otherwise
     */
    static bool disconnect();
    
    /**
     * Check if connected to a network
     * @return true if connected, false otherwise
     */
    static bool isConnected();
    
    /**
     * Get current SSID
     * @return current SSID or empty string if not connected
     */
    static String getCurrentSSID();
    
    /**
     * Get current signal strength
     * @return signal strength in dBm or 0 if not connected
     */
    static int getCurrentRSSI();
    
    /**
     * Get saved networks
     * @return vector of saved networks
     */
    static std::vector<WiFiNetwork> getSavedNetworks();
    
    /**
     * Save a network
     * @param ssid network SSID
     * @param password network password
     * @return true if successful, false otherwise
     */
    static bool saveNetwork(const String& ssid, const String& password);
    
    /**
     * Remove a saved network
     * @param ssid network SSID
     * @return true if successful, false otherwise
     */
    static bool removeSavedNetwork(const String& ssid);
    
    /**
     * Load saved networks from storage
     * @return true if successful, false otherwise
     */
    static bool loadSavedNetworks();
    
    /**
     * Save networks to storage
     * @return true if successful, false otherwise
     */
    static bool saveSavedNetworks();
    
    /**
     * Enable/disable auto reconnect
     * @param enabled true to enable, false to disable
     */
    static void setAutoReconnect(bool enabled);
    
    /**
     * Check if auto reconnect is enabled
     * @return true if enabled, false otherwise
     */
    static bool isAutoReconnectEnabled();
    
    /**
     * Update WiFi manager (call in loop)
     */
    static void update();

private:
    static bool _initialized;
    static std::vector<WiFiNetwork> _savedNetworks;
    static WiFiStatusCallback _statusCallback;
    static bool _connecting;
    static uint32_t _connectionStartTime;
    static int _connectionAttempt;
    static bool _autoReconnectEnabled;
    static uint32_t _lastReconnectAttempt;
    
    static void onScanComplete(int numNetworks);
    static WiFiScanCallback _scanCallback;
};

#endif // CONNECTIVITY_WIFI_MANAGER_H
