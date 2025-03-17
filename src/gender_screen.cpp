/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Gender Selection Screen Implementation
 * 
 * Compatible with LVGL 8.4.0
 */

#include "gender_screen.h"

// Static member initialization
LogEntry GenderScreen::_currentEntry;

lv_obj_t* GenderScreen::create() {
    // Create screen
    lv_obj_t* screen = lv_obj_create(NULL);
    lv_obj_set_size(screen, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x202020), 0); // Dark background
    
    // Add status bar
    StatusBar::create(screen);
    
    // Create title
    lv_obj_t* title = lv_label_create(screen);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(title, "Select Gender");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 30);
    
    // Create instructions
    lv_obj_t* instructions = lv_label_create(screen);
    lv_obj_set_style_text_font(instructions, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(instructions, lv_color_hex(0xCCCCCC), 0);
    lv_label_set_text(instructions, "Please select the person's gender");
    lv_obj_align(instructions, LV_ALIGN_TOP_MID, 0, 60);
    
    // Create gender selector
    lv_obj_t* genderSelector = GenderSelector::create(screen, _genderSelectedHandler);
    lv_obj_align(genderSelector, LV_ALIGN_CENTER, 0, 0);
    
    // Create next button
    lv_obj_t* nextBtn = lv_btn_create(screen);
    lv_obj_set_size(nextBtn, 120, 50);
    lv_obj_align(nextBtn, LV_ALIGN_BOTTOM_RIGHT, -20, -20);
    lv_obj_set_style_bg_color(nextBtn, lv_color_hex(0x0078D7), 0); // Blue button
    lv_obj_add_event_cb(nextBtn, _nextBtnClickHandler, LV_EVENT_CLICKED, NULL);
    
    // Create next button label
    lv_obj_t* nextBtnLabel = lv_label_create(nextBtn);
    lv_label_set_text(nextBtnLabel, "Next");
    lv_obj_center(nextBtnLabel);
    
    // Create back button
    lv_obj_t* backBtn = lv_btn_create(screen);
    lv_obj_set_size(backBtn, 120, 50);
    lv_obj_align(backBtn, LV_ALIGN_BOTTOM_LEFT, 20, -20);
    lv_obj_set_style_bg_color(backBtn, lv_color_hex(0x505050), 0); // Gray button
    lv_obj_add_event_cb(backBtn, _backBtnClickHandler, LV_EVENT_CLICKED, NULL);
    
    // Create back button label
    lv_obj_t* backBtnLabel = lv_label_create(backBtn);
    lv_label_set_text(backBtnLabel, "Back");
    lv_obj_center(backBtnLabel);
    
    // Update screen
    update(screen);
    
    return screen;
}

void GenderScreen::update(lv_obj_t* screen) {
    if (!screen) {
        return;
    }
    
    // Update status bar if exists
    lv_obj_t* statusBar = lv_obj_get_child(screen, 0);
    if (statusBar) {
        StatusBar::update(statusBar);
    }
    
    // Find gender selector
    lv_obj_t* genderSelector = NULL;
    for (int i = 0; i < lv_obj_get_child_cnt(screen); i++) {
        lv_obj_t* child = lv_obj_get_child(screen, i);
        if (lv_obj_get_child_cnt(child) >= 3) { // Gender selector has at least 3 buttons
            genderSelector = child;
            break;
        }
    }
    
    if (!genderSelector) {
        return;
    }
    
    // Update selected gender
    GenderSelector::setSelectedGender(genderSelector, _currentEntry.getGender());
}

void GenderScreen::setCurrentEntry(const LogEntry& entry) {
    _currentEntry = entry;
}

LogEntry GenderScreen::getCurrentEntry() {
    return _currentEntry;
}

void GenderScreen::_genderSelectedHandler(Gender gender) {
    // Update current entry
    _currentEntry.setGender(gender);
    
    DEBUG_PRINTF("Gender selected: %d", gender);
}

void GenderScreen::_nextBtnClickHandler(lv_event_t* e) {
    // Check if gender is selected
    if (_currentEntry.getGender() == GENDER_UNKNOWN) {
        // Show error message
        lv_obj_t* screen = lv_scr_act();
        StatusBar::showNotification(lv_obj_get_child(screen, 0), "Please select a gender");
        return;
    }
    
    // Proceed to shirt color selection
    UIManager::setScreen(SCREEN_SHIRT_COLOR);
}

void GenderScreen::_backBtnClickHandler(lv_event_t* e) {
    // Go back to main menu
    UIManager::setScreen(SCREEN_MAIN_MENU);
}
