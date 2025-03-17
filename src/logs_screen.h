/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Logs Screen
 * 
 * This file contains the interface for the logs screen
 * Compatible with LVGL 8.4.0
 */

#ifndef UI_LOGS_SCREEN_H
#define UI_LOGS_SCREEN_H

#include <Arduino.h>
#include <lvgl.h>
#include "../config.h"

// Log screen mode
enum LogScreenMode {
    LOG_SCREEN_ALL,
    LOG_SCREEN_TODAY,
    LOG_SCREEN_PENDING
};

class LogsScreen {
public:
    /**
     * Create logs screen
     * @return screen object
     */
    static lv_obj_t* create();
    
    /**
     * Update logs screen
     * @param screen screen object
     */
    static void update(lv_obj_t* screen);
    
    /**
     * Set logs screen mode
     * @param mode screen mode
     */
    static void setMode(LogScreenMode mode);
    
    /**
     * Refresh logs list
     */
    static void refreshLogs();

private:
    static LogScreenMode _currentMode;
    static lv_obj_t* _logsList;
    static lv_obj_t* _modeSelector;
    static int _selectedLogIndex;
    
    // Event handlers
    static void _backButtonClickHandler(lv_event_t* e);
    static void _modeSelectorEventHandler(lv_event_t* e);
    static void _logItemClickHandler(lv_event_t* e);
    static void _syncButtonClickHandler(lv_event_t* e);
    static void _exportButtonClickHandler(lv_event_t* e);
    
    // Helper methods
    static void _loadLogs(lv_obj_t* list, LogScreenMode mode);
    static void _createLogItem(lv_obj_t* parent, const char* title, const char* description, 
                             const char* timestamp, bool synced, int index);
};

#endif // UI_LOGS_SCREEN_H
