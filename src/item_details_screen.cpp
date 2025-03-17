/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Item Details Screen Implementation
 * 
 * Compatible with LVGL 8.4.0
 */

#include "item_details_screen.h"

// Static member initialization
LogEntry ItemDetailsScreen::_currentEntry;

lv_obj_t* ItemDetailsScreen::create() {
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
    lv_label_set_text(title, "Item Details");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 30);
    
    // Create instructions
    lv_obj_t* instructions = lv_label_create(screen);
    lv_obj_set_style_text_font(instructions, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(instructions, lv_color_hex(0xCCCCCC), 0);
    lv_label_set_text(instructions, "Please enter item description and notes");
    lv_obj_align(instructions, LV_ALIGN_TOP_MID, 0, 60);
    
    // Create form container
    lv_obj_t* formContainer = lv_obj_create(screen);
    lv_obj_set_size(formContainer, DISPLAY_WIDTH - 40, 200);
    lv_obj_align(formContainer, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(formContainer, lv_color_hex(0x303030), 0);
    lv_obj_set_style_pad_all(formContainer, 10, 0);
    
    // Create description label
    lv_obj_t* descLabel = lv_label_create(formContainer);
    lv_obj_set_style_text_font(descLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(descLabel, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(descLabel, "Item Description:");
    lv_obj_align(descLabel, LV_ALIGN_TOP_LEFT, 0, 0);
    
    // Create description text area
    lv_obj_t* descTextArea = lv_textarea_create(formContainer);
    lv_obj_set_size(descTextArea, lv_pct(100), 40);
    lv_obj_align_to(descTextArea, descLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_textarea_set_placeholder_text(descTextArea, "Enter item description");
    lv_obj_add_event_cb(descTextArea, _descriptionInputHandler, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(descTextArea, _keyboardEventHandler, LV_EVENT_FOCUSED, NULL);
    lv_obj_set_user_data(descTextArea, (void*)"description");
    
    // Create notes label
    lv_obj_t* notesLabel = lv_label_create(formContainer);
    lv_obj_set_style_text_font(notesLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(notesLabel, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(notesLabel, "Additional Notes:");
    lv_obj_align_to(notesLabel, descTextArea, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    
    // Create notes text area
    lv_obj_t* notesTextArea = lv_textarea_create(formContainer);
    lv_obj_set_size(notesTextArea, lv_pct(100), 80);
    lv_obj_align_to(notesTextArea, notesLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    lv_textarea_set_placeholder_text(notesTextArea, "Enter additional notes");
    lv_obj_add_event_cb(notesTextArea, _notesInputHandler, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(notesTextArea, _keyboardEventHandler, LV_EVENT_FOCUSED, NULL);
    lv_obj_set_user_data(notesTextArea, (void*)"notes");
    
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

void ItemDetailsScreen::update(lv_obj_t* screen) {
    if (!screen) {
        return;
    }
    
    // Update status bar if exists
    lv_obj_t* statusBar = lv_obj_get_child(screen, 0);
    if (statusBar) {
        StatusBar::update(statusBar);
    }
    
    // Find form container
    lv_obj_t* formContainer = NULL;
    for (int i = 0; i < lv_obj_get_child_cnt(screen); i++) {
        lv_obj_t* child = lv_obj_get_child(screen, i);
        if (lv_obj_get_child_cnt(child) >= 4) { // Form container has at least 4 children
            formContainer = child;
            break;
        }
    }
    
    if (!formContainer) {
        return;
    }
    
    // Find description text area
    lv_obj_t* descTextArea = NULL;
    for (int i = 0; i < lv_obj_get_child_cnt(formContainer); i++) {
        lv_obj_t* child = lv_obj_get_child(formContainer, i);
        if (lv_obj_get_user_data(child) == (void*)"description") {
            descTextArea = child;
            break;
        }
    }
    
    // Find notes text area
    lv_obj_t* notesTextArea = NULL;
    for (int i = 0; i < lv_obj_get_child_cnt(formContainer); i++) {
        lv_obj_t* child = lv_obj_get_child(formContainer, i);
        if (lv_obj_get_user_data(child) == (void*)"notes") {
            notesTextArea = child;
            break;
        }
    }
    
    // Update text areas with current entry data
    if (descTextArea) {
        lv_textarea_set_text(descTextArea, _currentEntry.getItemDescription().c_str());
    }
    
    if (notesTextArea) {
        lv_textarea_set_text(notesTextArea, _currentEntry.getNotes().c_str());
    }
}

void ItemDetailsScreen::setCurrentEntry(const LogEntry& entry) {
    _currentEntry = entry;
}

LogEntry ItemDetailsScreen::getCurrentEntry() {
    return _currentEntry;
}

void ItemDetailsScreen::_descriptionInputHandler(lv_event_t* e) {
    lv_obj_t* textArea = lv_event_get_target(e);
    const char* text = lv_textarea_get_text(textArea);
    
    // Update current entry
    _currentEntry.setItemDescription(String(text));
    
    DEBUG_PRINTF("Item description updated: %s", text);
}

void ItemDetailsScreen::_notesInputHandler(lv_event_t* e) {
    lv_obj_t* textArea = lv_event_get_target(e);
    const char* text = lv_textarea_get_text(textArea);
    
    // Update current entry
    _currentEntry.setNotes(String(text));
    
    DEBUG_PRINTF("Notes updated: %s", text);
}

void ItemDetailsScreen::_nextBtnClickHandler(lv_event_t* e) {
    // Check if item description is entered
    if (_currentEntry.getItemDescription().isEmpty()) {
        // Show error message
        lv_obj_t* screen = lv_scr_act();
        StatusBar::showNotification(lv_obj_get_child(screen, 0), "Please enter item description");
        return;
    }
    
    // Proceed to confirmation screen
    UIManager::setScreen(SCREEN_CONFIRM);
}

void ItemDetailsScreen::_backBtnClickHandler(lv_event_t* e) {
    // Go back to item type screen
    UIManager::setScreen(SCREEN_ITEM_TYPE);
}

void ItemDetailsScreen::_keyboardEventHandler(lv_event_t* e) {
    lv_obj_t* textArea = lv_event_get_target(e);
    
    // Show keyboard
    UIManager::showKeyboard(textArea);
}
