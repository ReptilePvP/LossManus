/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Gender Selector Component Implementation
 * 
 * Compatible with LVGL 8.4.0
 */

#include "gender_selector.h"

// Static member initialization
lv_style_t GenderSelector::_styleGenderBtn;
lv_style_t GenderSelector::_styleGenderBtnSelected;
lv_style_t GenderSelector::_styleGenderLabel;
bool GenderSelector::_stylesInitialized = false;

// Gender information
const GenderSelector::GenderInfo GenderSelector::_genders[] = {
    { GENDER_MALE, "Male", LV_SYMBOL_MALE },
    { GENDER_FEMALE, "Female", LV_SYMBOL_FEMALE },
    { GENDER_OTHER, "Other", "?" }
};
const int GenderSelector::_genderCount = sizeof(GenderSelector::_genders) / sizeof(GenderSelector::_genders[0]);

void GenderSelector::_initStyles() {
    if (_stylesInitialized) {
        return;
    }
    
    // Gender button style
    lv_style_init(&_styleGenderBtn);
    lv_style_set_bg_color(&_styleGenderBtn, lv_color_hex(0x303030)); // Dark background
    lv_style_set_bg_opa(&_styleGenderBtn, LV_OPA_COVER);
    lv_style_set_border_width(&_styleGenderBtn, 1);
    lv_style_set_border_color(&_styleGenderBtn, lv_color_hex(0x505050));
    lv_style_set_radius(&_styleGenderBtn, 10);
    lv_style_set_pad_all(&_styleGenderBtn, 10);
    
    // Selected gender button style
    lv_style_init(&_styleGenderBtnSelected);
    lv_style_set_bg_color(&_styleGenderBtnSelected, lv_color_hex(0x0078D7)); // Blue background when selected
    lv_style_set_border_width(&_styleGenderBtnSelected, 2);
    lv_style_set_border_color(&_styleGenderBtnSelected, lv_color_hex(0xFFFFFF));
    lv_style_set_shadow_width(&_styleGenderBtnSelected, 10);
    lv_style_set_shadow_ofs_y(&_styleGenderBtnSelected, 3);
    lv_style_set_shadow_opa(&_styleGenderBtnSelected, LV_OPA_50);
    
    // Gender label style
    lv_style_init(&_styleGenderLabel);
    lv_style_set_text_color(&_styleGenderLabel, lv_color_hex(0xFFFFFF)); // White text
    lv_style_set_text_font(&_styleGenderLabel, &lv_font_montserrat_16);
    
    _stylesInitialized = true;
}

lv_obj_t* GenderSelector::create(lv_obj_t* parent, GenderSelectedCallback callback) {
    _initStyles();
    
    // Create gender selector container
    lv_obj_t* genderSelector = lv_obj_create(parent);
    lv_obj_set_size(genderSelector, lv_pct(100), 120);
    lv_obj_set_style_bg_opa(genderSelector, LV_OPA_0, 0);
    lv_obj_set_style_border_width(genderSelector, 0, 0);
    lv_obj_set_style_pad_all(genderSelector, 10, 0);
    lv_obj_set_flex_flow(genderSelector, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(genderSelector, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    // Store callback in user data
    lv_obj_set_user_data(genderSelector, (void*)callback);
    
    // Calculate button size
    int btnWidth = (lv_obj_get_width(genderSelector) - 40) / _genderCount;
    int btnHeight = 100;
    
    // Create gender buttons
    for (int i = 0; i < _genderCount; i++) {
        // Create button
        lv_obj_t* btn = lv_btn_create(genderSelector);
        lv_obj_set_size(btn, btnWidth, btnHeight);
        lv_obj_add_style(btn, &_styleGenderBtn, 0);
        
        // Store gender in button user data
        lv_obj_set_user_data(btn, (void*)(intptr_t)_genders[i].gender);
        
        // Create button content container
        lv_obj_t* contentContainer = lv_obj_create(btn);
        lv_obj_set_size(contentContainer, lv_pct(100), lv_pct(100));
        lv_obj_set_style_bg_opa(contentContainer, LV_OPA_0, 0);
        lv_obj_set_style_border_width(contentContainer, 0, 0);
        lv_obj_set_style_pad_all(contentContainer, 0, 0);
        lv_obj_clear_flag(contentContainer, LV_OBJ_FLAG_SCROLLABLE);
        
        // Create icon label
        lv_obj_t* iconLabel = lv_label_create(contentContainer);
        lv_obj_add_style(iconLabel, &_styleGenderLabel, 0);
        lv_label_set_text(iconLabel, _genders[i].icon);
        lv_obj_set_style_text_font(iconLabel, &lv_font_montserrat_22, 0); // Larger font for icon
        lv_obj_align(iconLabel, LV_ALIGN_TOP_MID, 0, 10);
        
        // Create text label
        lv_obj_t* textLabel = lv_label_create(contentContainer);
        lv_obj_add_style(textLabel, &_styleGenderLabel, 0);
        lv_label_set_text(textLabel, _genders[i].name);
        lv_obj_align(textLabel, LV_ALIGN_BOTTOM_MID, 0, -10);
        
        // Add click event
        lv_obj_add_event_cb(btn, _genderButtonClickHandler, LV_EVENT_CLICKED, genderSelector);
    }
    
    return genderSelector;
}

Gender GenderSelector::getSelectedGender(lv_obj_t* genderSelector) {
    if (!genderSelector) {
        return GENDER_UNKNOWN;
    }
    
    // Find selected button
    lv_obj_t* btn = lv_obj_get_child(genderSelector, 0);
    while (btn) {
        if (lv_obj_has_state(btn, LV_STATE_CHECKED)) {
            // Get gender from button user data
            return (Gender)(intptr_t)lv_obj_get_user_data(btn);
        }
        btn = lv_obj_get_child(genderSelector, lv_obj_get_index(btn) + 1);
    }
    
    return GENDER_UNKNOWN;
}

bool GenderSelector::setSelectedGender(lv_obj_t* genderSelector, Gender gender) {
    if (!genderSelector) {
        return false;
    }
    
    // Clear current selection
    lv_obj_t* btn = lv_obj_get_child(genderSelector, 0);
    while (btn) {
        lv_obj_clear_state(btn, LV_STATE_CHECKED);
        lv_obj_remove_style(btn, &_styleGenderBtnSelected, LV_STATE_CHECKED);
        btn = lv_obj_get_child(genderSelector, lv_obj_get_index(btn) + 1);
    }
    
    // Find button with matching gender
    btn = lv_obj_get_child(genderSelector, 0);
    while (btn) {
        Gender btnGender = (Gender)(intptr_t)lv_obj_get_user_data(btn);
        if (btnGender == gender) {
            // Select this button
            lv_obj_add_state(btn, LV_STATE_CHECKED);
            lv_obj_add_style(btn, &_styleGenderBtnSelected, LV_STATE_CHECKED);
            return true;
        }
        btn = lv_obj_get_child(genderSelector, lv_obj_get_index(btn) + 1);
    }
    
    return false;
}

void GenderSelector::_genderButtonClickHandler(lv_event_t* e) {
    lv_obj_t* btn = lv_event_get_target(e);
    lv_obj_t* genderSelector = (lv_obj_t*)lv_event_get_user_data(e);
    
    if (!btn || !genderSelector) {
        return;
    }
    
    // Clear current selection
    lv_obj_t* otherBtn = lv_obj_get_child(genderSelector, 0);
    while (otherBtn) {
        if (otherBtn != btn) {
            lv_obj_clear_state(otherBtn, LV_STATE_CHECKED);
            lv_obj_remove_style(otherBtn, &_styleGenderBtnSelected, LV_STATE_CHECKED);
        }
        otherBtn = lv_obj_get_child(genderSelector, lv_obj_get_index(otherBtn) + 1);
    }
    
    // Select this button
    lv_obj_add_state(btn, LV_STATE_CHECKED);
    lv_obj_add_style(btn, &_styleGenderBtnSelected, LV_STATE_CHECKED);
    
    // Get gender from button user data
    Gender gender = (Gender)(intptr_t)lv_obj_get_user_data(btn);
    
    // Call callback
    GenderSelectedCallback callback = (GenderSelectedCallback)lv_obj_get_user_data(genderSelector);
    if (callback) {
        callback(gender);
    }
}
