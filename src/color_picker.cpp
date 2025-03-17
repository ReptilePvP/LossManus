/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Color Picker Component Implementation
 * 
 * Compatible with LVGL 8.4.0
 */

#include "color_picker.h"

// Static member initialization
lv_style_t ColorPicker::_styleColorBtn;
lv_style_t ColorPicker::_styleColorBtnSelected;
lv_style_t ColorPicker::_styleColorLabel;
bool ColorPicker::_stylesInitialized = false;

void ColorPicker::_initStyles() {
    if (_stylesInitialized) {
        return;
    }
    
    // Color button style
    lv_style_init(&_styleColorBtn);
    lv_style_set_border_width(&_styleColorBtn, 2);
    lv_style_set_border_color(&_styleColorBtn, lv_color_hex(0x505050));
    lv_style_set_radius(&_styleColorBtn, 5);
    lv_style_set_pad_all(&_styleColorBtn, 5);
    
    // Selected color button style
    lv_style_init(&_styleColorBtnSelected);
    lv_style_set_border_width(&_styleColorBtnSelected, 3);
    lv_style_set_border_color(&_styleColorBtnSelected, lv_color_hex(0xFFFFFF));
    lv_style_set_shadow_width(&_styleColorBtnSelected, 10);
    lv_style_set_shadow_ofs_y(&_styleColorBtnSelected, 3);
    lv_style_set_shadow_opa(&_styleColorBtnSelected, LV_OPA_50);
    
    // Color label style
    lv_style_init(&_styleColorLabel);
    lv_style_set_text_color(&_styleColorLabel, lv_color_hex(0xFFFFFF));
    lv_style_set_text_font(&_styleColorLabel, &lv_font_montserrat_12);
    
    _stylesInitialized = true;
}

lv_obj_t* ColorPicker::create(lv_obj_t* parent, const std::vector<ColorInfo>& colorList, 
                           ColorSelectedCallback callback, uint8_t columns) {
    _initStyles();
    
    // Create color picker container
    lv_obj_t* colorPicker = lv_obj_create(parent);
    lv_obj_set_size(colorPicker, lv_pct(100), lv_pct(80));
    lv_obj_set_style_bg_opa(colorPicker, LV_OPA_0, 0);
    lv_obj_set_style_border_width(colorPicker, 0, 0);
    lv_obj_set_style_pad_all(colorPicker, 5, 0);
    lv_obj_set_flex_flow(colorPicker, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(colorPicker, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    
    // Store callback in user data
    lv_obj_set_user_data(colorPicker, (void*)callback);
    
    // Calculate button size based on columns
    int btnSize = (lv_obj_get_width(colorPicker) - 20) / columns;
    
    // Create color buttons
    for (const auto& color : colorList) {
        // Create button container
        lv_obj_t* btnContainer = lv_obj_create(colorPicker);
        lv_obj_set_size(btnContainer, btnSize, btnSize + 20); // Extra space for label
        lv_obj_set_style_bg_opa(btnContainer, LV_OPA_0, 0);
        lv_obj_set_style_border_width(btnContainer, 0, 0);
        lv_obj_set_style_pad_all(btnContainer, 2, 0);
        lv_obj_clear_flag(btnContainer, LV_OBJ_FLAG_SCROLLABLE);
        
        // Create color button
        lv_obj_t* btn = lv_btn_create(btnContainer);
        lv_obj_set_size(btn, btnSize - 10, btnSize - 10);
        lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 0);
        lv_obj_add_style(btn, &_styleColorBtn, 0);
        
        // Set button color
        lv_obj_set_style_bg_color(btn, lv_color_hex(color.rgb), 0);
        
        // Store color info in button user data
        lv_obj_set_user_data(btn, (void*)new ColorInfo(color));
        
        // Add click event
        lv_obj_add_event_cb(btn, _colorButtonClickHandler, LV_EVENT_CLICKED, colorPicker);
        
        // Create color label
        lv_obj_t* label = lv_label_create(btnContainer);
        lv_obj_add_style(label, &_styleColorLabel, 0);
        lv_label_set_text(label, color.name.c_str());
        lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, 0);
    }
    
    // Add custom color button if needed
    if (colorList.size() > 0) {
        addCustomColorButton(colorPicker);
    }
    
    return colorPicker;
}

ColorInfo ColorPicker::getSelectedColor(lv_obj_t* colorPicker) {
    if (!colorPicker) {
        return ColorInfo();
    }
    
    // Find selected button
    lv_obj_t* container = lv_obj_get_child(colorPicker, 0);
    while (container) {
        lv_obj_t* btn = lv_obj_get_child(container, 0);
        if (btn && lv_obj_has_state(btn, LV_STATE_CHECKED)) {
            // Get color info from button user data
            ColorInfo* colorInfo = (ColorInfo*)lv_obj_get_user_data(btn);
            if (colorInfo) {
                return *colorInfo;
            }
        }
        container = lv_obj_get_child(colorPicker, lv_obj_get_index(container) + 1);
    }
    
    return ColorInfo();
}

bool ColorPicker::setSelectedColor(lv_obj_t* colorPicker, const String& colorName) {
    if (!colorPicker) {
        return false;
    }
    
    // Clear current selection
    lv_obj_t* container = lv_obj_get_child(colorPicker, 0);
    while (container) {
        lv_obj_t* btn = lv_obj_get_child(container, 0);
        if (btn) {
            lv_obj_clear_state(btn, LV_STATE_CHECKED);
            lv_obj_remove_style(btn, &_styleColorBtnSelected, LV_STATE_CHECKED);
        }
        container = lv_obj_get_child(colorPicker, lv_obj_get_index(container) + 1);
    }
    
    // Find button with matching color name
    container = lv_obj_get_child(colorPicker, 0);
    while (container) {
        lv_obj_t* btn = lv_obj_get_child(container, 0);
        if (btn) {
            ColorInfo* colorInfo = (ColorInfo*)lv_obj_get_user_data(btn);
            if (colorInfo && colorInfo->name.equalsIgnoreCase(colorName)) {
                // Select this button
                lv_obj_add_state(btn, LV_STATE_CHECKED);
                lv_obj_add_style(btn, &_styleColorBtnSelected, LV_STATE_CHECKED);
                return true;
            }
        }
        container = lv_obj_get_child(colorPicker, lv_obj_get_index(container) + 1);
    }
    
    return false;
}

bool ColorPicker::setSelectedColorByRGB(lv_obj_t* colorPicker, uint32_t rgb) {
    if (!colorPicker) {
        return false;
    }
    
    // Clear current selection
    lv_obj_t* container = lv_obj_get_child(colorPicker, 0);
    while (container) {
        lv_obj_t* btn = lv_obj_get_child(container, 0);
        if (btn) {
            lv_obj_clear_state(btn, LV_STATE_CHECKED);
            lv_obj_remove_style(btn, &_styleColorBtnSelected, LV_STATE_CHECKED);
        }
        container = lv_obj_get_child(colorPicker, lv_obj_get_index(container) + 1);
    }
    
    // Find button with matching RGB value
    container = lv_obj_get_child(colorPicker, 0);
    while (container) {
        lv_obj_t* btn = lv_obj_get_child(container, 0);
        if (btn) {
            ColorInfo* colorInfo = (ColorInfo*)lv_obj_get_user_data(btn);
            if (colorInfo && colorInfo->rgb == rgb) {
                // Select this button
                lv_obj_add_state(btn, LV_STATE_CHECKED);
                lv_obj_add_style(btn, &_styleColorBtnSelected, LV_STATE_CHECKED);
                return true;
            }
        }
        container = lv_obj_get_child(colorPicker, lv_obj_get_index(container) + 1);
    }
    
    return false;
}

bool ColorPicker::addCustomColorButton(lv_obj_t* colorPicker) {
    if (!colorPicker) {
        return false;
    }
    
    // Calculate button size based on existing buttons
    lv_obj_t* firstContainer = lv_obj_get_child(colorPicker, 0);
    if (!firstContainer) {
        return false;
    }
    
    int btnSize = lv_obj_get_width(firstContainer);
    
    // Create button container
    lv_obj_t* btnContainer = lv_obj_create(colorPicker);
    lv_obj_set_size(btnContainer, btnSize, btnSize);
    lv_obj_set_style_bg_opa(btnContainer, LV_OPA_0, 0);
    lv_obj_set_style_border_width(btnContainer, 0, 0);
    lv_obj_set_style_pad_all(btnContainer, 2, 0);
    lv_obj_clear_flag(btnContainer, LV_OBJ_FLAG_SCROLLABLE);
    
    // Create custom color button
    lv_obj_t* btn = lv_btn_create(btnContainer);
    lv_obj_set_size(btn, btnSize - 10, btnSize - 10);
    lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_add_style(btn, &_styleColorBtn, 0);
    
    // Set button appearance
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x808080), 0); // Gray background
    
    // Create custom color icon/label
    lv_obj_t* label = lv_label_create(btn);
    lv_label_set_text(label, LV_SYMBOL_EDIT);
    lv_obj_center(label);
    
    // Create custom color text label
    lv_obj_t* textLabel = lv_label_create(btnContainer);
    lv_obj_add_style(textLabel, &_styleColorLabel, 0);
    lv_label_set_text(textLabel, "Custom");
    lv_obj_align(textLabel, LV_ALIGN_BOTTOM_MID, 0, 0);
    
    // Add click event
    lv_obj_add_event_cb(btn, _customColorClickHandler, LV_EVENT_CLICKED, colorPicker);
    
    return true;
}

void ColorPicker::_colorButtonClickHandler(lv_event_t* e) {
    lv_obj_t* btn = lv_event_get_target(e);
    lv_obj_t* colorPicker = (lv_obj_t*)lv_event_get_user_data(e);
    
    if (!btn || !colorPicker) {
        return;
    }
    
    // Clear current selection
    lv_obj_t* container = lv_obj_get_child(colorPicker, 0);
    while (container) {
        lv_obj_t* otherBtn = lv_obj_get_child(container, 0);
        if (otherBtn && otherBtn != btn) {
            lv_obj_clear_state(otherBtn, LV_STATE_CHECKED);
            lv_obj_remove_style(otherBtn, &_styleColorBtnSelected, LV_STATE_CHECKED);
        }
        container = lv_obj_get_child(colorPicker, lv_obj_get_index(container) + 1);
    }
    
    // Select this button
    lv_obj_add_state(btn, LV_STATE_CHECKED);
    lv_obj_add_style(btn, &_styleColorBtnSelected, LV_STATE_CHECKED);
    
    // Get color info from button user data
    ColorInfo* colorInfo = (ColorInfo*)lv_obj_get_user_data(btn);
    if (!colorInfo) {
        return;
    }
    
    // Call callback
    ColorSelectedCallback callback = (ColorSelectedCallback)lv_obj_get_user_data(colorPicker);
    if (callback) {
        callback(*colorInfo);
    }
}

void ColorPicker::_customColorClickHandler(lv_event_t* e) {
    lv_obj_t* btn = lv_event_get_target(e);
    lv_obj_t* colorPicker = (lv_obj_t*)lv_event_get_user_data(e);
    
    if (!btn || !colorPicker) {
        return;
    }
    
    // TODO: Implement custom color picker dialog
    // For now, just create a basic color with a unique name
    
    static int customColorCount = 0;
    customColorCount++;
    
    String colorName = "Custom " + String(customColorCount);
    uint32_t rgb = 0xFF0000; // Default to red
    
    ColorInfo customColor;
    customColor.name = colorName;
    customColor.rgb = rgb;
    
    // Call callback
    ColorSelectedCallback callback = (ColorSelectedCallback)lv_obj_get_user_data(colorPicker);
    if (callback) {
        callback(customColor);
    }
}
