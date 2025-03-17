/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Status Bar Component
 * 
 * This file contains the interface for the status bar component
 * Compatible with LVGL 8.4.0
 */

#ifndef UI_COMPONENTS_STATUS_BAR_H
#define UI_COMPONENTS_STATUS_BAR_H

#include <Arduino.h>
#include <lvgl.h>
#include "../../config.h"
#include "../../hal/rtc.h"
#include "../../hal/power.h"
#include "../../hal/wifi_hardware.h"

class StatusBar {
public:
    /**
     * Create status bar
     * @param parent parent object
     * @return status bar object
     */
    static lv_obj_t* create(lv_obj_t* parent);
    
    /**
     * Update status bar
     * @param statusBar status bar object
     */
    static void update(lv_obj_t* statusBar);
    
    /**
     * Set status bar visibility
     * @param statusBar status bar object
     * @param visible visibility state
     */
    static void setVisible(lv_obj_t* statusBar, bool visible);
    
    /**
     * Show notification in status bar
     * @param statusBar status bar object
     * @param message notification message
     * @param duration notification duration in milliseconds
     */
    static void showNotification(lv_obj_t* statusBar, const String& message, uint32_t duration = 3000);

private:
    static lv_style_t _style;
    static lv_style_t _textStyle;
    static bool _stylesInitialized;
    
    static void _initStyles();
    static void _notificationTimerCallback(lv_timer_t* timer);
};

#endif // UI_COMPONENTS_STATUS_BAR_H
