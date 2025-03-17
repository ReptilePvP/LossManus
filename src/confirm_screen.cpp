/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Confirmation Screen Implementation
 * 
 * Compatible with LVGL 8.4.0
 */

#include "confirm_screen.h"
#include "../../hal/rtc.h"
#include "../../data/sync.h"

// Static member initialization
LogEntry ConfirmScreen::_currentEntry;

lv_obj_t* ConfirmScreen::create() {
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
    lv_label_set_text(title, "Confirm Entry");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 30);
    
    // Create instructions
    lv_obj_t* instructions = lv_label_create(screen);
    lv_obj_set_style_text_font(instructions, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(instructions, lv_color_hex(0xCCCCCC), 0);
    lv_label_set_text(instructions, "Please review and confirm the entry");
    lv_obj_align(instructions, LV_ALIGN_TOP_MID, 0, 60);
    
    // Create summary container
    lv_obj_t* summaryContainer = lv_obj_create(screen);
    lv_obj_set_size(summaryContainer, DISPLAY_WIDTH - 40, 240);
    lv_obj_align(summaryContainer, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(summaryContainer, lv_color_hex(0x303030), 0);
    lv_obj_set_style_pad_all(summaryContainer, 15, 0);
    
    // Create summary text
    lv_obj_t* summaryText = lv_label_create(summaryContainer);
    lv_obj_set_style_text_font(summaryText, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(summaryText, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_long_mode(summaryText, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(summaryText, lv_pct(100));
    lv_obj_set_user_data(summaryText, (void*)"summary");
    lv_obj_align(summaryText, LV_ALIGN_TOP_LEFT, 0, 0);
    
    // Create button container
    lv_obj_t* btnContainer = lv_obj_create(screen);
    lv_obj_set_size(btnContainer, DISPLAY_WIDTH - 40, 50);
    lv_obj_align(btnContainer, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_set_style_bg_opa(btnContainer, LV_OPA_0, 0);
    lv_obj_set_style_border_width(btnContainer, 0, 0);
    lv_obj_set_style_pad_all(btnContainer, 0, 0);
    lv_obj_set_flex_flow(btnContainer, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(btnContainer, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    // Create save button
    lv_obj_t* saveBtn = lv_btn_create(btnContainer);
    lv_obj_set_size(saveBtn, 100, 50);
    lv_obj_set_style_bg_color(saveBtn, lv_color_hex(0x0078D7), 0); // Blue button
    lv_obj_add_event_cb(saveBtn, _saveBtnClickHandler, LV_EVENT_CLICKED, NULL);
    
    // Create save button label
    lv_obj_t* saveBtnLabel = lv_label_create(saveBtn);
    lv_label_set_text(saveBtnLabel, "Save");
    lv_obj_center(saveBtnLabel);
    
    // Create edit button
    lv_obj_t* editBtn = lv_btn_create(btnContainer);
    lv_obj_set_size(editBtn, 100, 50);
    lv_obj_set_style_bg_color(editBtn, lv_color_hex(0x505050), 0); // Gray button
    lv_obj_add_event_cb(editBtn, _editBtnClickHandler, LV_EVENT_CLICKED, NULL);
    
    // Create edit button label
    lv_obj_t* editBtnLabel = lv_label_create(editBtn);
    lv_label_set_text(editBtnLabel, "Edit");
    lv_obj_center(editBtnLabel);
    
    // Create cancel button
    lv_obj_t* cancelBtn = lv_btn_create(btnContainer);
    lv_obj_set_size(cancelBtn, 100, 50);
    lv_obj_set_style_bg_color(cancelBtn, lv_color_hex(0xD32F2F), 0); // Red button
    lv_obj_add_event_cb(cancelBtn, _cancelBtnClickHandler, LV_EVENT_CLICKED, NULL);
    
    // Create cancel button label
    lv_obj_t* cancelBtnLabel = lv_label_create(cancelBtn);
    lv_label_set_text(cancelBtnLabel, "Cancel");
    lv_obj_center(cancelBtnLabel);
    
    // Update screen
    update(screen);
    
    return screen;
}

void ConfirmScreen::update(lv_obj_t* screen) {
    if (!screen) {
        return;
    }
    
    // Update status bar if exists
    lv_obj_t* statusBar = lv_obj_get_child(screen, 0);
    if (statusBar) {
        StatusBar::update(statusBar);
    }
    
    // Find summary container
    lv_obj_t* summaryContainer = NULL;
    for (int i = 0; i < lv_obj_get_child_cnt(screen); i++) {
        lv_obj_t* child = lv_obj_get_child(screen, i);
        if (lv_obj_get_child_cnt(child) >= 1) { // Summary container has at least 1 child
            summaryContainer = child;
            break;
        }
    }
    
    if (!summaryContainer) {
        return;
    }
    
    // Find summary text
    lv_obj_t* summaryText = NULL;
    for (int i = 0; i < lv_obj_get_child_cnt(summaryContainer); i++) {
        lv_obj_t* child = lv_obj_get_child(summaryContainer, i);
        if (lv_obj_get_user_data(child) == (void*)"summary") {
            summaryText = child;
            break;
        }
    }
    
    if (!summaryText) {
        return;
    }
    
    // Update summary text
    String summary = "Date/Time: " + RtcHAL::formatCurrentTime("%Y-%m-%d %H:%M:%S") + "\n\n";
    summary += "Gender: " + _getGenderString(_currentEntry.getGender()) + "\n\n";
    summary += "Shirt Color: " + _currentEntry.getShirtColor().name + "\n";
    summary += "Pants Color: " + _currentEntry.getPantsColor().name + "\n";
    summary += "Shoes Color: " + _currentEntry.getShoesColor().name + "\n\n";
    summary += "Item Type: " + _getItemTypeString(_currentEntry.getItemType()) + "\n";
    summary += "Description: " + _currentEntry.getItemDescription() + "\n\n";
    
    if (!_currentEntry.getNotes().isEmpty()) {
        summary += "Notes: " + _currentEntry.getNotes() + "\n";
    }
    
    lv_label_set_text(summaryText, summary.c_str());
}

void ConfirmScreen::setCurrentEntry(const LogEntry& entry) {
    _currentEntry = entry;
}

LogEntry ConfirmScreen::getCurrentEntry() {
    return _currentEntry;
}

void ConfirmScreen::_saveBtnClickHandler(lv_event_t* e) {
    // Set current timestamp
    _currentEntry.setTimestamp(RtcHAL::getCurrentTime());
    
    // Save entry to database
    if (Database::addEntry(_currentEntry)) {
        // Queue for sync if WiFi is connected
        SyncManager::queueEntry(_currentEntry);
        
        // Show success message
        lv_obj_t* screen = lv_scr_act();
        StatusBar::showNotification(lv_obj_get_child(screen, 0), "Entry saved successfully");
        
        // Go back to main menu after a delay
        lv_timer_t* timer = lv_timer_create([](lv_timer_t* timer) {
            UIManager::setScreen(SCREEN_MAIN_MENU);
        }, 2000, NULL);
        lv_timer_set_repeat_count(timer, 1);
    } else {
        // Show error message
        lv_obj_t* screen = lv_scr_act();
        StatusBar::showNotification(lv_obj_get_child(screen, 0), "Failed to save entry");
    }
}

void ConfirmScreen::_editBtnClickHandler(lv_event_t* e) {
    // Go back to gender screen to start editing
    UIManager::setScreen(SCREEN_GENDER);
}

void ConfirmScreen::_cancelBtnClickHandler(lv_event_t* e) {
    // Go back to main menu
    UIManager::setScreen(SCREEN_MAIN_MENU);
}

String ConfirmScreen::_getGenderString(Gender gender) {
    switch (gender) {
        case GENDER_MALE:
            return "Male";
        case GENDER_FEMALE:
            return "Female";
        case GENDER_OTHER:
            return "Other";
        default:
            return "Unknown";
    }
}

String ConfirmScreen::_getItemTypeString(ItemType itemType) {
    switch (itemType) {
        case ITEM_CLOTHING:
            return "Clothing";
        case ITEM_ELECTRONICS:
            return "Electronics";
        case ITEM_JEWELRY:
            return "Jewelry";
        case ITEM_COSMETICS:
            return "Cosmetics";
        case ITEM_ACCESSORIES:
            return "Accessories";
        case ITEM_FOOD:
            return "Food";
        case ITEM_TOYS:
            return "Toys";
        case ITEM_TOOLS:
            return "Tools";
        case ITEM_OTHER:
            return "Other";
        case ITEM_CUSTOM:
            return "Custom";
        default:
            return "Unknown";
    }
}
