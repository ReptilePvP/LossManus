/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - About Screen
 * 
 * This file contains the interface for the about screen
 * Compatible with LVGL 8.4.0
 */

#ifndef UI_ABOUT_SCREEN_H
#define UI_ABOUT_SCREEN_H

#include <Arduino.h>
#include <lvgl.h>
#include "../config.h"

class AboutScreen {
public:
    /**
     * Create about screen
     * @return screen object
     */
    static lv_obj_t* create();
    
    /**
     * Update about screen
     * @param screen screen object
     */
    static void update(lv_obj_t* screen);

private:
    // Event handlers
    static void _backButtonClickHandler(lv_event_t* e);
};

#endif // UI_ABOUT_SCREEN_H
