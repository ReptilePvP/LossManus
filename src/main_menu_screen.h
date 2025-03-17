/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Main Menu Screen
 * 
 * This file contains the interface for the main menu screen
 * Compatible with LVGL 8.4.0
 */

#ifndef UI_SCREENS_MAIN_MENU_SCREEN_H
#define UI_SCREENS_MAIN_MENU_SCREEN_H

#include <Arduino.h>
#include <lvgl.h>
#include "../../config.h"
#include "../ui_manager.h"
#include "../components/status_bar.h"
#include "../components/card.h"

class MainMenuScreen {
public:
    /**
     * Create main menu screen
     * @return main menu screen object
     */
    static lv_obj_t* create();
    
    /**
     * Update main menu screen
     * @param screen main menu screen object
     */
    static void update(lv_obj_t* screen);

private:
    // Event handlers
    static void _newEntryBtnClickHandler(lv_event_t* e);
    static void _viewLogsBtnClickHandler(lv_event_t* e);
    static void _exportBtnClickHandler(lv_event_t* e);
    static void _syncBtnClickHandler(lv_event_t* e);
    static void _settingsBtnClickHandler(lv_event_t* e);
    static void _aboutBtnClickHandler(lv_event_t* e);
};

#endif // UI_SCREENS_MAIN_MENU_SCREEN_H
