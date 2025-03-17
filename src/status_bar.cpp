/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Status Bar Component Implementation
 * 
 * Compatible with LVGL 8.4.0
 */

#include "status_bar.h"

// Static member initialization
lv_style_t StatusBar::_style;
lv_style_t StatusBar::_textStyle;
bool StatusBar::_stylesInitialized = false;

void StatusBar::_initStyles() {
    if (_stylesInitialized) {
        return;
    }
    
    // Initialize status bar style
    lv_style_init(&_style);
    lv_style_set_bg_color(&_style, lv_color_hex(0x101010)); // Very dark background
    lv_style_set_bg_opa(&_style, LV_OPA_COVER);
    lv_style_set_border_width(&_style, 0);
    lv_style_set_pad_all(&_style, 2);
    
    // Initialize text style
    lv_style_init(&_textStyle);
    lv_style_set_text_color(&_textStyle, lv_color_hex(0xCCCCCC)); // Light gray text
    lv_style_set_text_font(&_textStyle, &lv_font_montserrat_12); // Small font
    
    _stylesInitialized = true;
}

lv_obj_t* StatusBar::create(lv_obj_t* parent) {
    _initStyles();
    
    // Create status bar container
    lv_obj_t* statusBar = lv_obj_create(parent);
    lv_obj_set_size(statusBar, lv_pct(100), 20);
    lv_obj_align(statusBar, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_add_style(statusBar, &_style, 0);
    lv_obj_clear_flag(statusBar, LV_OBJ_FLAG_SCROLLABLE);
    
    // Create time label
    lv_obj_t* timeLabel = lv_label_create(statusBar);
    lv_obj_add_style(timeLabel, &_textStyle, 0);
    lv_obj_align(timeLabel, LV_ALIGN_LEFT_MID, 5, 0);
    lv_obj_set_user_data(timeLabel, (void*)"time"); // Tag for updating
    lv_label_set_text(timeLabel, "00:00:00");
    
    // Create battery label
    lv_obj_t* batteryLabel = lv_label_create(statusBar);
    lv_obj_add_style(batteryLabel, &_textStyle, 0);
    lv_obj_align(batteryLabel, LV_ALIGN_RIGHT_MID, -5, 0);
    lv_obj_set_user_data(batteryLabel, (void*)"battery"); // Tag for updating
    lv_label_set_text(batteryLabel, "100%");
    
    // Create WiFi label
    lv_obj_t* wifiLabel = lv_label_create(statusBar);
    lv_obj_add_style(wifiLabel, &_textStyle, 0);
    lv_obj_align(wifiLabel, LV_ALIGN_RIGHT_MID, -60, 0);
    lv_obj_set_user_data(wifiLabel, (void*)"wifi"); // Tag for updating
    lv_label_set_text(wifiLabel, "WiFi: ---");
    
    // Create notification label (hidden by default)
    lv_obj_t* notificationLabel = lv_label_create(statusBar);
    lv_obj_add_style(notificationLabel, &_textStyle, 0);
    lv_obj_align(notificationLabel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_user_data(notificationLabel, (void*)"notification"); // Tag for updating
    lv_label_set_text(notificationLabel, "");
    lv_obj_add_flag(notificationLabel, LV_OBJ_FLAG_HIDDEN);
    
    // Update status bar
    update(statusBar);
    
    return statusBar;
}

void StatusBar::update(lv_obj_t* statusBar) {
    if (!statusBar) {
        return;
    }
    
    // Update time label
    lv_obj_t* timeLabel = nullptr;
    lv_obj_t* child = lv_obj_get_child(statusBar, 0);
    while (child) {
        if (lv_obj_get_user_data(child) == (void*)"time") {
            timeLabel = child;
            break;
        }
        child = lv_obj_get_child(statusBar, lv_obj_get_index(child) + 1);
    }
    
    if (timeLabel) {
        String timeStr = RtcHAL::formatCurrentTime("%H:%M:%S");
        lv_label_set_text(timeLabel, timeStr.c_str());
    }
    
    // Update battery label
    lv_obj_t* batteryLabel = nullptr;
    child = lv_obj_get_child(statusBar, 0);
    while (child) {
        if (lv_obj_get_user_data(child) == (void*)"battery") {
            batteryLabel = child;
            break;
        }
        child = lv_obj_get_child(statusBar, lv_obj_get_index(child) + 1);
    }
    
    if (batteryLabel) {
        int batteryLevel = PowerHAL::getBatteryLevel();
        String batteryStr = String(batteryLevel) + "%";
        if (PowerHAL::isCharging()) {
            batteryStr += " ⚡";
        }
        lv_label_set_text(batteryLabel, batteryStr.c_str());
    }
    
    // Update WiFi label
    lv_obj_t* wifiLabel = nullptr;
    child = lv_obj_get_child(statusBar, 0);
    while (child) {
        if (lv_obj_get_user_data(child) == (void*)"wifi") {
            wifiLabel = child;
            break;
        }
        child = lv_obj_get_child(statusBar, lv_obj_get_index(child) + 1);
    }
    
    if (wifiLabel) {
        String wifiStr;
        if (WiFiHAL::isEnabled()) {
            if (WiFi.status() == WL_CONNECTED) {
                int rssi = WiFiHAL::getSignalStrength();
                if (rssi > -50) {
                    wifiStr = "WiFi: ●●●●";
                } else if (rssi > -65) {
                    wifiStr = "WiFi: ●●●○";
                } else if (rssi > -75) {
                    wifiStr = "WiFi: ●●○○";
                } else if (rssi > -85) {
                    wifiStr = "WiFi: ●○○○";
                } else {
                    wifiStr = "WiFi: ○○○○";
                }
            } else {
                wifiStr = "WiFi: ✕";
            }
        } else {
            wifiStr = "WiFi: OFF";
        }
        lv_label_set_text(wifiLabel, wifiStr.c_str());
    }
}

void StatusBar::setVisible(lv_obj_t* statusBar, bool visible) {
    if (!statusBar) {
        return;
    }
    
    if (visible) {
        lv_obj_clear_flag(statusBar, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(statusBar, LV_OBJ_FLAG_HIDDEN);
    }
}

void StatusBar::showNotification(lv_obj_t* statusBar, const String& message, uint32_t duration) {
    if (!statusBar) {
        return;
    }
    
    // Find notification label
    lv_obj_t* notificationLabel = nullptr;
    lv_obj_t* child = lv_obj_get_child(statusBar, 0);
    while (child) {
        if (lv_obj_get_user_data(child) == (void*)"notification") {
            notificationLabel = child;
            break;
        }
        child = lv_obj_get_child(statusBar, lv_obj_get_index(child) + 1);
    }
    
    if (!notificationLabel) {
        return;
    }
    
    // Hide other labels
    child = lv_obj_get_child(statusBar, 0);
    while (child) {
        if (child != notificationLabel) {
            lv_obj_add_flag(child, LV_OBJ_FLAG_HIDDEN);
        }
        child = lv_obj_get_child(statusBar, lv_obj_get_index(child) + 1);
    }
    
    // Show notification
    lv_label_set_text(notificationLabel, message.c_str());
    lv_obj_clear_flag(notificationLabel, LV_OBJ_FLAG_HIDDEN);
    
    // Create timer to hide notification
    lv_timer_t* timer = lv_timer_create(_notificationTimerCallback, duration, statusBar);
    lv_timer_set_repeat_count(timer, 1);
}

void StatusBar::_notificationTimerCallback(lv_timer_t* timer) {
    lv_obj_t* statusBar = (lv_obj_t*)timer->user_data;
    
    // Find notification label
    lv_obj_t* notificationLabel = nullptr;
    lv_obj_t* child = lv_obj_get_child(statusBar, 0);
    while (child) {
        if (lv_obj_get_user_data(child) == (void*)"notification") {
            notificationLabel = child;
            break;
        }
        child = lv_obj_get_child(statusBar, lv_obj_get_index(child) + 1);
    }
    
    if (!notificationLabel) {
        return;
    }
    
    // Hide notification
    lv_obj_add_flag(notificationLabel, LV_OBJ_FLAG_HIDDEN);
    
    // Show other labels
    child = lv_obj_get_child(statusBar, 0);
    while (child) {
        if (child != notificationLabel) {
            lv_obj_clear_flag(child, LV_OBJ_FLAG_HIDDEN);
        }
        child = lv_obj_get_child(statusBar, lv_obj_get_index(child) + 1);
    }
}
