/**
 * Enhanced Loss Prevention Log
 * Hardware Abstraction Layer - Display Interface
 * 
 * This file contains the interface for display operations
 */

#ifndef HAL_DISPLAY_H
#define HAL_DISPLAY_H

#include <M5Unified.h>
#include <M5GFX.h>
#include <lvgl.h>
#include "../config.h"

class DisplayHAL {
public:
    /**
     * Initialize the display
     * @return true if successful, false otherwise
     */
    static bool init();
    
    /**
     * Set the display brightness
     * @param brightness level (0-255)
     */
    static void setBrightness(uint8_t brightness);
    
    /**
     * Get the current display brightness
     * @return brightness level (0-255)
     */
    static uint8_t getBrightness();
    
    /**
     * Enter power save mode
     */
    static void enterPowerSaveMode();
    
    /**
     * Exit power save mode
     */
    static void exitPowerSaveMode();
    
    /**
     * LVGL display flush callback
     */
    static void lvglFlushCb(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
    
    /**
     * LVGL touch read callback
     */
    static void lvglTouchReadCb(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
    
    /**
     * Update the display (call in loop)
     */
    static void update();

private:
    static uint8_t _brightness;
    static bool _powerSaveMode;
    static uint32_t _lastActivityTime;
};

#endif // HAL_DISPLAY_H
