/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Search Screen
 * 
 * This file contains the interface for the search screen
 * Compatible with LVGL 8.4.0
 */

#ifndef UI_SEARCH_SCREEN_H
#define UI_SEARCH_SCREEN_H

#include <Arduino.h>
#include <lvgl.h>
#include "../config.h"

class SearchScreen {
public:
    /**
     * Create search screen
     * @return screen object
     */
    static lv_obj_t* create();
    
    /**
     * Update search screen
     * @param screen screen object
     */
    static void update(lv_obj_t* screen);
    
    /**
     * Perform search with current criteria
     */
    static void performSearch();

private:
    static lv_obj_t* _searchInput;
    static lv_obj_t* _resultsList;
    static lv_obj_t* _dateFilterDropdown;
    static lv_obj_t* _itemFilterDropdown;
    
    // Event handlers
    static void _backButtonClickHandler(lv_event_t* e);
    static void _searchButtonClickHandler(lv_event_t* e);
    static void _clearButtonClickHandler(lv_event_t* e);
    static void _dateFilterEventHandler(lv_event_t* e);
    static void _itemFilterEventHandler(lv_event_t* e);
    static void _resultItemClickHandler(lv_event_t* e);
    
    // Helper methods
    static void _displaySearchResults(const char* query, int dateFilter, int itemFilter);
    static void _createResultItem(lv_obj_t* parent, const char* title, const char* description, 
                                const char* timestamp, bool synced, int index);
};

#endif // UI_SEARCH_SCREEN_H
