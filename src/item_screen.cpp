/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Item Selection Screen Implementation
 * 
 * Compatible with LVGL 8.4.0
 */

#include "item_screen.h"

// Static member initialization
LogEntry ItemScreen::_currentEntry;

lv_obj_t* ItemScreen::create() {
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
    lv_label_set_text(title, "Select Item Type");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 30);
    
    // Create instructions
    lv_obj_t* instructions = lv_label_create(screen);
    lv_obj_set_style_text_font(instructions, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(instructions, lv_color_hex(0xCCCCCC), 0);
    lv_label_set_text(instructions, "Please select the type of item taken");
    lv_obj_align(instructions, LV_ALIGN_TOP_MID, 0, 60);
    
    // Create item selector
    lv_obj_t* itemSelector = ItemSelector::create(screen, _itemSelectedHandler);
    lv_obj_align(itemSelector, LV_ALIGN_CENTER, 0, 0);
    
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

void ItemScreen::update(lv_obj_t* screen) {
    if (!screen) {
        return;
    }
    
    // Update status bar if exists
    lv_obj_t* statusBar = lv_obj_get_child(screen, 0);
    if (statusBar) {
        StatusBar::update(statusBar);
    }
    
    // Find item selector
    lv_obj_t* itemSelector = NULL;
    for (int i = 0; i < lv_obj_get_child_cnt(screen); i++) {
        lv_obj_t* child = lv_obj_get_child(screen, i);
        if (lv_obj_get_child_cnt(child) > 5) { // Item selector has many buttons
            itemSelector = child;
            break;
        }
    }
    
    if (!itemSelector) {
        return;
    }
    
    // Update selected item type
    ItemSelector::setSelectedItemType(itemSelector, _currentEntry.getItemType());
}

void ItemScreen::setCurrentEntry(const LogEntry& entry) {
    _currentEntry = entry;
}

LogEntry ItemScreen::getCurrentEntry() {
    return _currentEntry;
}

void ItemScreen::_itemSelectedHandler(ItemType itemType) {
    // Update current entry
    _currentEntry.setItemType(itemType);
    
    DEBUG_PRINTF("Item type selected: %d", itemType);
}

void ItemScreen::_nextBtnClickHandler(lv_event_t* e) {
    // Check if item type is selected
    if (_currentEntry.getItemType() == ITEM_UNKNOWN) {
        // Show error message
        lv_obj_t* screen = lv_scr_act();
        StatusBar::showNotification(lv_obj_get_child(screen, 0), "Please select an item type");
        return;
    }
    
    // Proceed to item details screen
    UIManager::setScreen(SCREEN_ITEM_DETAILS);
}

void ItemScreen::_backBtnClickHandler(lv_event_t* e) {
    // Go back to shoes color screen
    UIManager::setScreen(SCREEN_SHOES_COLOR);
}
