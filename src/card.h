/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Card Component
 * 
 * This file contains the interface for the card component
 * Compatible with LVGL 8.4.0
 */

#ifndef UI_COMPONENTS_CARD_H
#define UI_COMPONENTS_CARD_H

#include <Arduino.h>
#include <lvgl.h>
#include "../../config.h"

class Card {
public:
    /**
     * Create card
     * @param parent parent object
     * @param width card width
     * @param height card height
     * @param title card title
     * @param icon card icon (LVGL symbol or nullptr)
     * @return card object
     */
    static lv_obj_t* create(lv_obj_t* parent, int width, int height, 
                          const char* title = nullptr, const char* icon = nullptr);
    
    /**
     * Set card title
     * @param card card object
     * @param title card title
     */
    static void setTitle(lv_obj_t* card, const char* title);
    
    /**
     * Set card icon
     * @param card card object
     * @param icon card icon (LVGL symbol)
     */
    static void setIcon(lv_obj_t* card, const char* icon);
    
    /**
     * Set card content
     * @param card card object
     * @param content content text
     */
    static void setContent(lv_obj_t* card, const char* content);
    
    /**
     * Add button to card
     * @param card card object
     * @param text button text
     * @param callback button click callback
     * @param userData user data to pass to callback
     * @return button object
     */
    static lv_obj_t* addButton(lv_obj_t* card, const char* text, 
                             lv_event_cb_t callback, void* userData = nullptr);
    
    /**
     * Add image to card
     * @param card card object
     * @param src image source
     * @return image object
     */
    static lv_obj_t* addImage(lv_obj_t* card, const void* src);
    
    /**
     * Get card content area
     * @param card card object
     * @return content area object
     */
    static lv_obj_t* getContentArea(lv_obj_t* card);
    
    /**
     * Set card clickable
     * @param card card object
     * @param clickable whether card is clickable
     * @param callback click callback
     * @param userData user data to pass to callback
     */
    static void setClickable(lv_obj_t* card, bool clickable, 
                           lv_event_cb_t callback = nullptr, void* userData = nullptr);

private:
    static lv_style_t _styleCard;
    static lv_style_t _styleCardPressed;
    static lv_style_t _styleTitle;
    static lv_style_t _styleContent;
    static lv_style_t _styleBtn;
    static lv_style_t _styleBtnPressed;
    static bool _stylesInitialized;
    
    static void _initStyles();
};

#endif // UI_COMPONENTS_CARD_H
