/**
 * Enhanced Loss Prevention Log
 * Hardware Abstraction Layer - WiFi Hardware Implementation
 */

#include "wifi_hardware.h"

// Static member initialization
bool WiFiHAL::_initialized = false;
bool WiFiHAL::_enabled = false;

bool WiFiHAL::init() {
    DEBUG_PRINT("Initializing WiFi hardware...");
    
    if (_initialized) {
        DEBUG_PRINT("WiFi hardware already initialized");
        return true;
    }
    
    // Set WiFi to station mode
    WiFi.mode(WIFI_STA);
    
    // Set hostname
    WiFi.setHostname("LossPreventionLog");
    
    _initialized = true;
    DEBUG_PRINT("WiFi hardware initialization complete");
    return true;
}

bool WiFiHAL::enable() {
    if (!_initialized) {
        if (!init()) {
            return false;
        }
    }
    
    if (_enabled) {
        DEBUG_PRINT("WiFi already enabled");
        return true;
    }
    
    WiFi.mode(WIFI_STA);
    _enabled = true;
    DEBUG_PRINT("WiFi enabled");
    return true;
}

bool WiFiHAL::disable() {
    if (!_initialized || !_enabled) {
        DEBUG_PRINT("WiFi already disabled");
        return true;
    }
    
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    _enabled = false;
    DEBUG_PRINT("WiFi disabled");
    return true;
}

bool WiFiHAL::isEnabled() {
    return _enabled;
}

int WiFiHAL::getSignalStrength() {
    if (!_initialized || !_enabled || WiFi.status() != WL_CONNECTED) {
        return -100; // Very weak signal as default
    }
    
    return WiFi.RSSI();
}

String WiFiHAL::getMacAddress() {
    if (!_initialized) {
        return "00:00:00:00:00:00";
    }
    
    return WiFi.macAddress();
}

String WiFiHAL::getIPAddress() {
    if (!_initialized || !_enabled || WiFi.status() != WL_CONNECTED) {
        return "0.0.0.0";
    }
    
    return WiFi.localIP().toString();
}

void WiFiHAL::scanNetworks(void (*callback)(int numNetworks)) {
    if (!_initialized || !_enabled) {
        if (callback) {
            callback(0);
        }
        return;
    }
    
    DEBUG_PRINT("Scanning for WiFi networks...");
    
    // Start async scan
    WiFi.scanNetworks(true, true); // Async, show hidden networks
    
    // Wait for scan to complete
    int scanStatus;
    do {
        delay(100);
        scanStatus = WiFi.scanComplete();
    } while (scanStatus == WIFI_SCAN_RUNNING);
    
    if (scanStatus < 0) {
        DEBUG_PRINT("WiFi scan failed");
        if (callback) {
            callback(0);
        }
        return;
    }
    
    DEBUG_PRINTF("WiFi scan complete, found %d networks", scanStatus);
    
    if (callback) {
        callback(scanStatus);
    }
}

String WiFiHAL::getSSID(int index) {
    if (index < 0 || index >= getNetworkCount()) {
        return "";
    }
    
    return WiFi.SSID(index);
}

int WiFiHAL::getRSSI(int index) {
    if (index < 0 || index >= getNetworkCount()) {
        return -100;
    }
    
    return WiFi.RSSI(index);
}

uint8_t WiFiHAL::getEncryptionType(int index) {
    if (index < 0 || index >= getNetworkCount()) {
        return 0;
    }
    
    return WiFi.encryptionType(index);
}

int WiFiHAL::getNetworkCount() {
    int scanStatus = WiFi.scanComplete();
    return (scanStatus > 0) ? scanStatus : 0;
}
