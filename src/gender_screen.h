/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Gender Selection Screen
 * 
 * This file contains the interface for the gender selection screen
 * Compatible with LVGL 8.4.0
 */

#ifndef UI_SCREENS_GENDER_SCREEN_H
#define UI_SCREENS_GENDER_SCREEN_H

#include <Arduino.h>
#include <lvgl.h>
#include "../../config.h"
#include "../ui_manager.h"
#include "../components/status_bar.h"
#include "../components/gender_selector.h"
#include "../../data/log_entry.h"

class GenderScreen {
public:
    /**
     * Create gender selection screen
     * @return gender selection screen object
     */
    static lv_obj_t* create();
    
    /**
     * Update gender selection screen
     * @param screen gender selection screen object
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
    
    // Event handlers
    static void _genderSelectedHandler(Gender gender);
    static void _nextBtnClickHandler(lv_event_t* e);
    static void _backBtnClickHandler(lv_event_t* e);
};

#endif // UI_SCREENS_GENDER_SCREEN_H
