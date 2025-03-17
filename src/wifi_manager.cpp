/**
 * Enhanced Loss Prevention Log
 * Connectivity Layer - WiFi Manager Implementation
 */

#include "wifi_manager.h"
#include "../hal/storage.h"
#include <Preferences.h>

// Static member initialization
bool WiFiManager::_initialized = false;
std::vector<WiFiNetwork> WiFiManager::_savedNetworks;
WiFiStatusCallback WiFiManager::_statusCallback = nullptr;
WiFiScanCallback WiFiManager::_scanCallback = nullptr;
bool WiFiManager::_connecting = false;
uint32_t WiFiManager::_connectionStartTime = 0;
int WiFiManager::_connectionAttempt = 0;
bool WiFiManager::_autoReconnectEnabled = true;
uint32_t WiFiManager::_lastReconnectAttempt = 0;

bool WiFiManager::init() {
    DEBUG_PRINT("Initializing WiFi manager...");
    
    if (_initialized) {
        DEBUG_PRINT("WiFi manager already initialized");
        return true;
    }
    
    // Initialize WiFi hardware
    if (!WiFiHAL::init()) {
        DEBUG_PRINT("Failed to initialize WiFi hardware");
        return false;
    }
    
    // Load saved networks
    loadSavedNetworks();
    
    _initialized = true;
    _connecting = false;
    _autoReconnectEnabled = true;
    
    DEBUG_PRINT("WiFi manager initialization complete");
    return true;
}

void WiFiManager::scanNetworks(WiFiScanCallback callback) {
    if (!_initialized) {
        if (!init()) {
            if (callback) {
                callback(std::vector<WiFiNetwork>());
            }
            return;
        }
    }
    
    // Enable WiFi if not already enabled
    if (!WiFiHAL::isEnabled()) {
        WiFiHAL::enable();
    }
    
    _scanCallback = callback;
    
    // Start scan
    WiFiHAL::scanNetworks(onScanComplete);
}

void WiFiManager::onScanComplete(int numNetworks) {
    std::vector<WiFiNetwork> networks;
    
    for (int i = 0; i < numNetworks; i++) {
        WiFiNetwork network;
        
        String ssid = WiFiHAL::getSSID(i);
        ssid.toCharArray(network.ssid, sizeof(network.ssid));
        
        network.rssi = WiFiHAL::getRSSI(i);
        network.encType = WiFiHAL::getEncryptionType(i);
        network.saved = false;
        
        // Check if this is a saved network
        for (const auto& savedNetwork : _savedNetworks) {
            if (strcmp(network.ssid, savedNetwork.ssid) == 0) {
                network.saved = true;
                strcpy(network.password, savedNetwork.password);
                break;
            }
        }
        
        networks.push_back(network);
    }
    
    // Sort networks by signal strength
    std::sort(networks.begin(), networks.end(), [](const WiFiNetwork& a, const WiFiNetwork& b) {
        return a.rssi > b.rssi;
    });
    
    if (_scanCallback) {
        _scanCallback(networks);
    }
}

bool WiFiManager::connect(const String& ssid, const String& password, 
                         WiFiStatusCallback callback, bool save) {
    if (!_initialized) {
        if (!init()) {
            if (callback) {
                callback(false, "WiFi manager initialization failed");
            }
            return false;
        }
    }
    
    // Enable WiFi if not already enabled
    if (!WiFiHAL::isEnabled()) {
        WiFiHAL::enable();
    }
    
    _statusCallback = callback;
    _connecting = true;
    _connectionStartTime = millis();
    _connectionAttempt = 0;
    
    DEBUG_PRINTF("Connecting to WiFi: %s", ssid.c_str());
    
    // Save network if requested
    if (save) {
        saveNetwork(ssid, password);
    }
    
    // Start connection
    WiFi.disconnect();
    WiFi.begin(ssid.c_str(), password.c_str());
    
    return true;
}

bool WiFiManager::connectToSavedNetworks(WiFiStatusCallback callback) {
    if (!_initialized) {
        if (!init()) {
            if (callback) {
                callback(false, "WiFi manager initialization failed");
            }
            return false;
        }
    }
    
    if (_savedNetworks.empty()) {
        DEBUG_PRINT("No saved networks");
        if (callback) {
            callback(false, "No saved networks");
        }
        return false;
    }
    
    // Enable WiFi if not already enabled
    if (!WiFiHAL::isEnabled()) {
        WiFiHAL::enable();
    }
    
    _statusCallback = callback;
    _connecting = true;
    _connectionStartTime = millis();
    _connectionAttempt = 0;
    
    // Sort saved networks by signal strength
    scanNetworks([](const std::vector<WiFiNetwork>& networks) {
        // Create a map of SSID to RSSI
        std::map<String, int> rssiMap;
        for (const auto& network : networks) {
            rssiMap[String(network.ssid)] = network.rssi;
        }
        
        // Sort saved networks by RSSI
        std::sort(_savedNetworks.begin(), _savedNetworks.end(), [&rssiMap](const WiFiNetwork& a, const WiFiNetwork& b) {
            int rssiA = rssiMap.count(String(a.ssid)) ? rssiMap[String(a.ssid)] : -100;
            int rssiB = rssiMap.count(String(b.ssid)) ? rssiMap[String(b.ssid)] : -100;
            return rssiA > rssiB;
        });
        
        // Try to connect to the first saved network
        if (!_savedNetworks.empty()) {
            WiFi.disconnect();
            WiFi.begin(_savedNetworks[0].ssid, _savedNetworks[0].password);
            DEBUG_PRINTF("Connecting to saved network: %s", _savedNetworks[0].ssid);
        }
    });
    
    return true;
}

bool WiFiManager::disconnect() {
    if (!_initialized) {
        return false;
    }
    
    WiFi.disconnect();
    _connecting = false;
    
    DEBUG_PRINT("Disconnected from WiFi");
    return true;
}

bool WiFiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String WiFiManager::getCurrentSSID() {
    if (isConnected()) {
        return WiFi.SSID();
    }
    return "";
}

int WiFiManager::getCurrentRSSI() {
    if (isConnected()) {
        return WiFi.RSSI();
    }
    return 0;
}

std::vector<WiFiNetwork> WiFiManager::getSavedNetworks() {
    if (!_initialized) {
        init();
    }
    
    return _savedNetworks;
}

bool WiFiManager::saveNetwork(const String& ssid, const String& password) {
    if (!_initialized) {
        if (!init()) {
            return false;
        }
    }
    
    // Check if network already exists
    for (auto& network : _savedNetworks) {
        if (strcmp(network.ssid, ssid.c_str()) == 0) {
            // Update password
            strcpy(network.password, password.c_str());
            DEBUG_PRINTF("Updated saved network: %s", ssid.c_str());
            saveSavedNetworks();
            return true;
        }
    }
    
    // Add new network
    if (_savedNetworks.size() >= MAX_WIFI_NETWORKS) {
        // Remove the last network to make room
        _savedNetworks.pop_back();
    }
    
    WiFiNetwork network;
    ssid.toCharArray(network.ssid, sizeof(network.ssid));
    password.toCharArray(network.password, sizeof(network.password));
    network.rssi = 0;
    network.encType = 0;
    network.saved = true;
    
    _savedNetworks.push_back(network);
    
    DEBUG_PRINTF("Added saved network: %s", ssid.c_str());
    saveSavedNetworks();
    
    return true;
}

bool WiFiManager::removeSavedNetwork(const String& ssid) {
    if (!_initialized) {
        if (!init()) {
            return false;
        }
    }
    
    for (auto it = _savedNetworks.begin(); it != _savedNetworks.end(); ++it) {
        if (strcmp(it->ssid, ssid.c_str()) == 0) {
            _savedNetworks.erase(it);
            DEBUG_PRINTF("Removed saved network: %s", ssid.c_str());
            saveSavedNetworks();
            return true;
        }
    }
    
    DEBUG_PRINTF("Network not found: %s", ssid.c_str());
    return false;
}

bool WiFiManager::loadSavedNetworks() {
    _savedNetworks.clear();
    
    Preferences preferences;
    preferences.begin("wifi", false);
    
    int count = preferences.getInt("count", 0);
    
    for (int i = 0; i < count && i < MAX_WIFI_NETWORKS; i++) {
        WiFiNetwork network;
        
        String ssidKey = "ssid" + String(i);
        String pwdKey = "pwd" + String(i);
        
        String ssid = preferences.getString(ssidKey.c_str(), "");
        String password = preferences.getString(pwdKey.c_str(), "");
        
        if (ssid.length() > 0) {
            ssid.toCharArray(network.ssid, sizeof(network.ssid));
            password.toCharArray(network.password, sizeof(network.password));
            network.rssi = 0;
            network.encType = 0;
            network.saved = true;
            
            _savedNetworks.push_back(network);
        }
    }
    
    preferences.end();
    
    DEBUG_PRINTF("Loaded %d saved networks", _savedNetworks.size());
    return true;
}

bool WiFiManager::saveSavedNetworks() {
    Preferences preferences;
    preferences.begin("wifi", false);
    
    preferences.putInt("count", _savedNetworks.size());
    
    for (size_t i = 0; i < _savedNetworks.size(); i++) {
        String ssidKey = "ssid" + String(i);
        String pwdKey = "pwd" + String(i);
        
        preferences.putString(ssidKey.c_str(), _savedNetworks[i].ssid);
        preferences.putString(pwdKey.c_str(), _savedNetworks[i].password);
    }
    
    preferences.end();
    
    DEBUG_PRINTF("Saved %d networks", _savedNetworks.size());
    return true;
}

void WiFiManager::setAutoReconnect(bool enabled) {
    _autoReconnectEnabled = enabled;
    DEBUG_PRINTF("Auto reconnect %s", enabled ? "enabled" : "disabled");
}

bool WiFiManager::isAutoReconnectEnabled() {
    return _autoReconnectEnabled;
}

void WiFiManager::update() {
    if (!_initialized) {
        return;
    }
    
    // Handle connection process
    if (_connecting) {
        if (WiFi.status() == WL_CONNECTED) {
            // Connection successful
            _connecting = false;
            DEBUG_PRINTF("Connected to WiFi: %s", WiFi.SSID().c_str());
            
            if (_statusCallback) {
                _statusCallback(true, "Connected to " + WiFi.SSID());
            }
        } else if (millis() - _connectionStartTime > WIFI_CONNECT_TIMEOUT) {
            // Connection timeout
            _connectionAttempt++;
            
            if (_connectionAttempt >= MAX_WIFI_CONNECTION_ATTEMPTS) {
                // Give up after max attempts
                _connecting = false;
                DEBUG_PRINT("Failed to connect to WiFi after maximum attempts");
                
                if (_statusCallback) {
                    _statusCallback(false, "Connection failed after " + String(MAX_WIFI_CONNECTION_ATTEMPTS) + " attempts");
                }
            } else {
                // Try the next saved network if available
                if (_connectionAttempt < _savedNetworks.size()) {
                    WiFi.disconnect();
                    WiFi.begin(_savedNetworks[_connectionAttempt].ssid, _savedNetworks[_connectionAttempt].password);
                    _connectionStartTime = millis();
                    
                    DEBUG_PRINTF("Trying next saved network: %s (attempt %d/%d)", 
                                _savedNetworks[_connectionAttempt].ssid, 
                                _connectionAttempt + 1, MAX_WIFI_CONNECTION_ATTEMPTS);
                } else {
                    // No more networks to try
                    _connecting = false;
                    DEBUG_PRINT("No more saved networks to try");
                    
                    if (_statusCallback) {
                        _statusCallback(false, "No more saved networks to try");
                    }
                }
            }
        }
    } else if (_autoReconnectEnabled && WiFi.status() != WL_CONNECTED) {
        // Auto reconnect
        if (millis() - _lastReconnectAttempt > WIFI_RECONNECT_INTERVAL) {
            _lastReconnectAttempt = millis();
            
            if (!_savedNetworks.empty()) {
                DEBUG_PRINT("Auto reconnecting to saved networks");
                connectToSavedNetworks(nullptr);
            }
        }
    }
}
