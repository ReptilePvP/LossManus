/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Color Selection Screen Implementation
 * 
 * Compatible with LVGL 8.4.0
 */

#include "color_screen.h"

// Static member initialization
LogEntry ColorScreen::_currentEntry;
ColorScreenType ColorScreen::_currentType = COLOR_SCREEN_SHIRT;
std::vector<ColorInfo> ColorScreen::_colorList;

void ColorScreen::_initColorList() {
    if (_colorList.size() > 0) {
        return;
    }
    
    // Initialize color list with common colors
    _colorList.push_back({.name = "Red", .rgb = 0xFF0000});
    _colorList.push_back({.name = "Green", .rgb = 0x00FF00});
    _colorList.push_back({.name = "Blue", .rgb = 0x0000FF});
    _colorList.push_back({.name = "Yellow", .rgb = 0xFFFF00});
    _colorList.push_back({.name = "Purple", .rgb = 0x800080});
    _colorList.push_back({.name = "Orange", .rgb = 0xFFA500});
    _colorList.push_back({.name = "Pink", .rgb = 0xFFC0CB});
    _colorList.push_back({.name = "Brown", .rgb = 0x8B4513});
    _colorList.push_back({.name = "Black", .rgb = 0x000000});
    _colorList.push_back({.name = "White", .rgb = 0xFFFFFF});
    _colorList.push_back({.name = "Gray", .rgb = 0x808080});
    _colorList.push_back({.name = "Beige", .rgb = 0xF5F5DC});
    _colorList.push_back({.name = "Teal", .rgb = 0x008080});
    _colorList.push_back({.name = "Navy", .rgb = 0x000080});
    _colorList.push_back({.name = "Maroon", .rgb = 0x800000});
}

lv_obj_t* ColorScreen::create(ColorScreenType type) {
    _currentType = type;
    _initColorList();
    
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
    
    // Set title based on screen type
    switch (_currentType) {
        case COLOR_SCREEN_SHIRT:
            lv_label_set_text(title, "Select Shirt Color");
            break;
        case COLOR_SCREEN_PANTS:
            lv_label_set_text(title, "Select Pants Color");
            break;
        case COLOR_SCREEN_SHOES:
            lv_label_set_text(title, "Select Shoes Color");
            break;
    }
    
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 30);
    
    // Create instructions
    lv_obj_t* instructions = lv_label_create(screen);
    lv_obj_set_style_text_font(instructions, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(instructions, lv_color_hex(0xCCCCCC), 0);
    
    // Set instructions based on screen type
    switch (_currentType) {
        case COLOR_SCREEN_SHIRT:
            lv_label_set_text(instructions, "Please select the shirt color");
            break;
        case COLOR_SCREEN_PANTS:
            lv_label_set_text(instructions, "Please select the pants color");
            break;
        case COLOR_SCREEN_SHOES:
            lv_label_set_text(instructions, "Please select the shoes color");
            break;
    }
    
    lv_obj_align(instructions, LV_ALIGN_TOP_MID, 0, 60);
    
    // Create color picker
    lv_obj_t* colorPicker = ColorPicker::create(screen, _colorList, _colorSelectedHandler);
    lv_obj_align(colorPicker, LV_ALIGN_CENTER, 0, 0);
    
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

void ColorScreen::update(lv_obj_t* screen) {
    if (!screen) {
        return;
    }
    
    // Update status bar if exists
    lv_obj_t* statusBar = lv_obj_get_child(screen, 0);
    if (statusBar) {
        StatusBar::update(statusBar);
    }
    
    // Find color picker
    lv_obj_t* colorPicker = NULL;
    for (int i = 0; i < lv_obj_get_child_cnt(screen); i++) {
        lv_obj_t* child = lv_obj_get_child(screen, i);
        if (lv_obj_get_child_cnt(child) > 10) { // Color picker has many buttons
            colorPicker = child;
            break;
        }
    }
    
    if (!colorPicker) {
        return;
    }
    
    // Update selected color based on screen type
    switch (_currentType) {
        case COLOR_SCREEN_SHIRT: {
            ColorInfo color = _currentEntry.getShirtColor();
            if (!color.name.isEmpty()) {
                ColorPicker::setSelectedColor(colorPicker, color.name);
            }
            break;
        }
        case COLOR_SCREEN_PANTS: {
            ColorInfo color = _currentEntry.getPantsColor();
            if (!color.name.isEmpty()) {
                ColorPicker::setSelectedColor(colorPicker, color.name);
            }
            break;
        }
        case COLOR_SCREEN_SHOES: {
            ColorInfo color = _currentEntry.getShoesColor();
            if (!color.name.isEmpty()) {
                ColorPicker::setSelectedColor(colorPicker, color.name);
            }
            break;
        }
    }
}

void ColorScreen::setCurrentEntry(const LogEntry& entry) {
    _currentEntry = entry;
}

LogEntry ColorScreen::getCurrentEntry() {
    return _currentEntry;
}

void ColorScreen::_colorSelectedHandler(const ColorInfo& color) {
    // Update current entry based on screen type
    switch (_currentType) {
        case COLOR_SCREEN_SHIRT:
            _currentEntry.setShirtColor(color);
            DEBUG_PRINTF("Shirt color selected: %s (0x%06X)", color.name.c_str(), color.rgb);
            break;
        case COLOR_SCREEN_PANTS:
            _currentEntry.setPantsColor(color);
            DEBUG_PRINTF("Pants color selected: %s (0x%06X)", color.name.c_str(), color.rgb);
            break;
        case COLOR_SCREEN_SHOES:
            _currentEntry.setShoesColor(color);
            DEBUG_PRINTF("Shoes color selected: %s (0x%06X)", color.name.c_str(), color.rgb);
            break;
    }
}

void ColorScreen::_nextBtnClickHandler(lv_event_t* e) {
    // Check if color is selected
    bool colorSelected = false;
    
    switch (_currentType) {
        case COLOR_SCREEN_SHIRT:
            colorSelected = !_currentEntry.getShirtColor().name.isEmpty();
            break;
        case COLOR_SCREEN_PANTS:
            colorSelected = !_currentEntry.getPantsColor().name.isEmpty();
            break;
        case COLOR_SCREEN_SHOES:
            colorSelected = !_currentEntry.getShoesColor().name.isEmpty();
            break;
    }
    
    if (!colorSelected) {
        // Show error message
        lv_obj_t* screen = lv_scr_act();
        StatusBar::showNotification(lv_obj_get_child(screen, 0), "Please select a color");
        return;
    }
    
    // Proceed to next screen based on current screen type
    switch (_currentType) {
        case COLOR_SCREEN_SHIRT:
            UIManager::setScreen(SCREEN_PANTS_COLOR);
            break;
        case COLOR_SCREEN_PANTS:
            UIManager::setScreen(SCREEN_SHOES_COLOR);
            break;
        case COLOR_SCREEN_SHOES:
            UIManager::setScreen(SCREEN_ITEM_TYPE);
            break;
    }
}

void ColorScreen::_backBtnClickHandler(lv_event_t* e) {
    // Go back to previous screen based on current screen type
    switch (_currentType) {
        case COLOR_SCREEN_SHIRT:
            UIManager::setScreen(SCREEN_GENDER);
            break;
        case COLOR_SCREEN_PANTS:
            UIManager::setScreen(SCREEN_SHIRT_COLOR);
            break;
        case COLOR_SCREEN_SHOES:
            UIManager::setScreen(SCREEN_PANTS_COLOR);
            break;
    }
}
