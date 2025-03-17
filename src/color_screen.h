/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Color Selection Screen
 * 
 * This file contains the interface for the color selection screen
 * Compatible with LVGL 8.4.0
 */

#ifndef UI_SCREENS_COLOR_SCREEN_H
#define UI_SCREENS_COLOR_SCREEN_H

#include <Arduino.h>
#include <lvgl.h>
#include "../../config.h"
#include "../ui_manager.h"
#include "../components/status_bar.h"
#include "../components/color_picker.h"
#include "../../data/log_entry.h"

enum ColorScreenType {
    COLOR_SCREEN_SHIRT,
    COLOR_SCREEN_PANTS,
    COLOR_SCREEN_SHOES
};

class ColorScreen {
public:
    /**
     * Create color selection screen
     * @param type color screen type
     * @return color selection screen object
     */
    static lv_obj_t* create(ColorScreenType type);
    
    /**
     * Update color selection screen
     * @param screen color selection screen object
     */
    static void update(lv_obj_t* screen);
    
    /**
     * Set current log entry
     * @param entry log entry to set
     */
    static void setCurrentEntry(const LogEntry& entry);
    
    /**
     * Get current log entry
     * @return current log entry
     */
    static LogEntry getCurrentEntry();

private:
    static LogEntry _currentEntry;
    static ColorScreenType _currentType;
    static std::vector<ColorInfo> _colorList;
    
    // Initialize color list
    static void _initColorList();
    
    // Event handlers
    static void _colorSelectedHandler(const ColorInfo& color);
    static void _nextBtnClickHandler(lv_event_t* e);
    static void _backBtnClickHandler(lv_event_t* e);
};

#endif // UI_SCREENS_COLOR_SCREEN_H
