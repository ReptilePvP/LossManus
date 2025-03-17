/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - WiFi Screen
 * 
 * This file contains the interface for the WiFi screen
 * Compatible with LVGL 8.4.0
 */

#ifndef UI_WIFI_SCREEN_H
#define UI_WIFI_SCREEN_H

#include <Arduino.h>
#include <lvgl.h>
#include "../config.h"

class WiFiScreen {
public:
    /**
     * Create WiFi screen
     * @return screen object
     */
    static lv_obj_t* create();
    
    /**
     * Update WiFi screen
     * @param screen screen object
     */
    static void update(lv_obj_t* screen);
    
    /**
     * Refresh WiFi networks list
     */
    static void refreshNetworks();

private:
    static lv_obj_t* _networksList;
    static lv_obj_t* _statusLabel;
    
    // Event handlers
    static void _backButtonClickHandler(lv_event_t* e);
    static void _scanButtonClickHandler(lv_event_t* e);
    static void _networkItemClickHandler(lv_event_t* e);
    static void _disconnectButtonClickHandler(lv_event_t* e);
    
    // Helper methods
    static void _showPasswordDialog(const char* ssid);
    static void _connectToNetwork(const char* ssid, const char* password);
    static void _updateConnectionStatus();
};

#endif // UI_WIFI_SCREEN_H
