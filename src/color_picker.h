/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Color Picker Component
 * 
 * This file contains the interface for the color picker component
 * Compatible with LVGL 8.4.0
 */

#ifndef UI_COMPONENTS_COLOR_PICKER_H
#define UI_COMPONENTS_COLOR_PICKER_H

#include <Arduino.h>
#include <lvgl.h>
#include <vector>
#include "../../config.h"
#include "../../data/log_entry.h"

// Color selection callback
typedef void (*ColorSelectedCallback)(const ColorInfo& color);

class ColorPicker {
public:
    /**
     * Create color picker grid
     * @param parent parent object
     * @param colorList list of colors to display
     * @param callback function to call when color is selected
     * @param columns number of columns in the grid
     * @return color picker object
     */
    static lv_obj_t* create(lv_obj_t* parent, const std::vector<ColorInfo>& colorList, 
                          ColorSelectedCallback callback, uint8_t columns = 3);
    
    /**
     * Get selected color
     * @param colorPicker color picker object
     * @return selected color or empty color if none selected
     */
    static ColorInfo getSelectedColor(lv_obj_t* colorPicker);
    
    /**
     * Set selected color
     * @param colorPicker color picker object
     * @param colorName name of color to select
     * @return true if color was found and selected, false otherwise
     */
    static bool setSelectedColor(lv_obj_t* colorPicker, const String& colorName);
    
    /**
     * Set selected color
     * @param colorPicker color picker object
     * @param rgb RGB value of color to select
     * @return true if color was found and selected, false otherwise
     */
    static bool setSelectedColorByRGB(lv_obj_t* colorPicker, uint32_t rgb);
    
    /**
     * Add custom color button
     * @param colorPicker color picker object
     * @return true if successful, false otherwise
     */
    static bool addCustomColorButton(lv_obj_t* colorPicker);

private:
    static lv_style_t _styleColorBtn;
    static lv_style_t _styleColorBtnSelected;
    static lv_style_t _styleColorLabel;
    static bool _stylesInitialized;
    
    static void _initStyles();
    static void _colorButtonClickHandler(lv_event_t* e);
    static void _customColorClickHandler(lv_event_t* e);
};

#endif // UI_COMPONENTS_COLOR_PICKER_H
