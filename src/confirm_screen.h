/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Confirmation Screen
 * 
 * This file contains the interface for the confirmation screen
 * Compatible with LVGL 8.4.0
 */

#ifndef UI_SCREENS_CONFIRM_SCREEN_H
#define UI_SCREENS_CONFIRM_SCREEN_H

#include <Arduino.h>
#include <lvgl.h>
#include "../../config.h"
#include "../ui_manager.h"
#include "../components/status_bar.h"
#include "../components/card.h"
#include "../../data/log_entry.h"
#include "../../data/database.h"

class ConfirmScreen {
public:
    /**
     * Create confirmation screen
     * @return confirmation screen object
     */
    static lv_obj_t* create();
    
    /**
     * Update confirmation screen
     * @param screen confirmation screen object
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
    static void _saveBtnClickHandler(lv_event_t* e);
    static void _editBtnClickHandler(lv_event_t* e);
    static void _cancelBtnClickHandler(lv_event_t* e);
    
    // Helper functions
    static String _getGenderString(Gender gender);
    static String _getItemTypeString(ItemType itemType);
};

#endif // UI_SCREENS_CONFIRM_SCREEN_H
