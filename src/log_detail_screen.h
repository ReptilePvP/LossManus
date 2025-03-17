/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Log Detail Screen
 * 
 * This file contains the interface for the log detail screen
 * Compatible with LVGL 8.4.0
 */

#ifndef UI_LOG_DETAIL_SCREEN_H
#define UI_LOG_DETAIL_SCREEN_H

#include <Arduino.h>
#include <lvgl.h>
#include "../config.h"

class LogDetailScreen {
public:
    /**
     * Create log detail screen
     * @return screen object
     */
    static lv_obj_t* create();
    
    /**
     * Update log detail screen
     * @param screen screen object
     */
    static void update(lv_obj_t* screen);
    
    /**
     * Set log index to display
     * @param index log index
     */
    static void setLogIndex(int index);

private:
    static int _logIndex;
    
    // Event handlers
    static void _backButtonClickHandler(lv_event_t* e);
    static void _editButtonClickHandler(lv_event_t* e);
    static void _deleteButtonClickHandler(lv_event_t* e);
    static void _syncButtonClickHandler(lv_event_t* e);
    
    // Helper methods
    static void _loadLogDetails(lv_obj_t* screen);
    static void _confirmDelete();
};

#endif // UI_LOG_DETAIL_SCREEN_H
