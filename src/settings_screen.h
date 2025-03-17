/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Settings Screen
 * 
 * This file contains the interface for the settings screen
 * Compatible with LVGL 8.4.0
 */

#ifndef UI_SETTINGS_SCREEN_H
#define UI_SETTINGS_SCREEN_H

#include <Arduino.h>
#include <lvgl.h>
#include "../config.h"

class SettingsScreen {
public:
    /**
     * Create settings screen
     * @return screen object
     */
    static lv_obj_t* create();
    
    /**
     * Update settings screen
     * @param screen screen object
     */
    static void update(lv_obj_t* screen);

private:
    // Event handlers
    static void _backButtonClickHandler(lv_event_t* e);
    static void _brightnessSliderEventHandler(lv_event_t* e);
    static void _volumeSliderEventHandler(lv_event_t* e);
    static void _soundSwitchEventHandler(lv_event_t* e);
    static void _wifiButtonClickHandler(lv_event_t* e);
    static void _aboutButtonClickHandler(lv_event_t* e);
    static void _resetButtonClickHandler(lv_event_t* e);
    
    // Helper methods
    static void _saveSettings();
    static void _loadSettings();
    static void _confirmReset();
};

#endif // UI_SETTINGS_SCREEN_H
