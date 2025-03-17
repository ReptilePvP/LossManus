/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - UI Manager Implementation
 * 
 * Compatible with LVGL 8.4.0
 */

#include "ui_manager.h"
#include "screens/main_menu_screen.h"
#include "screens/gender_screen.h"
#include "screens/color_screen.h"
#include "screens/item_screen.h"
#include "screens/item_details_screen.h"
#include "screens/confirm_screen.h"
#include "../app/app_controller.h"

// Static member initialization
bool UIManager::_initialized = false;
lv_obj_t* UIManager::_currentScreen = nullptr;
ScreenType UIManager::_currentScreenType = SCREEN_MAIN_MENU;
lv_obj_t* UIManager::_loadingScreen = nullptr;
lv_obj_t* UIManager::_alertDialog = nullptr;
lv_obj_t* UIManager::_confirmDialog = nullptr;
lv_obj_t* UIManager::_keyboard = nullptr;

bool UIManager::init() {
    if (_initialized) {
        return true;
    }
    
    DEBUG_PRINTLN("Initializing UI Manager...");
    
    // Initialize LVGL (should be done by DisplayHAL)
    
    // Create keyboard
    _keyboard = lv_keyboard_create(lv_scr_act());
    lv_obj_add_flag(_keyboard, LV_OBJ_FLAG_HIDDEN);
    
    _initialized = true;
    DEBUG_PRINTLN("UI Manager initialized successfully!");
    
    return true;
}

void UIManager::update() {
    if (!_initialized) {
        return;
    }
    
    // Update current screen based on type
    if (_currentScreen) {
        switch (_currentScreenType) {
            case SCREEN_MAIN_MENU:
                MainMenuScreen::update(_currentScreen);
                break;
            case SCREEN_GENDER:
                GenderScreen::update(_currentScreen);
                break;
            case SCREEN_SHIRT_COLOR:
            case SCREEN_PANTS_COLOR:
            case SCREEN_SHOES_COLOR:
                ColorScreen::update(_currentScreen);
                break;
            case SCREEN_ITEM_TYPE:
                ItemScreen::update(_currentScreen);
                break;
            case SCREEN_ITEM_DETAILS:
                ItemDetailsScreen::update(_currentScreen);
                break;
            case SCREEN_CONFIRM:
                ConfirmScreen::update(_currentScreen);
                break;
            // Other screens will be implemented later
            default:
                break;
        }
    }
}

void UIManager::setScreen(ScreenType screenType) {
    if (!_initialized) {
        return;
    }
    
    // Create screen based on type
    lv_obj_t* screen = nullptr;
    
    switch (screenType) {
        case SCREEN_MAIN_MENU:
            screen = MainMenuScreen::create();
            break;
        case SCREEN_GENDER:
            screen = GenderScreen::create();
            break;
        case SCREEN_SHIRT_COLOR:
            screen = ColorScreen::create(COLOR_SCREEN_SHIRT);
            break;
        case SCREEN_PANTS_COLOR:
            screen = ColorScreen::create(COLOR_SCREEN_PANTS);
            break;
        case SCREEN_SHOES_COLOR:
            screen = ColorScreen::create(COLOR_SCREEN_SHOES);
            break;
        case SCREEN_ITEM_TYPE:
            screen = ItemScreen::create();
            break;
        case SCREEN_ITEM_DETAILS:
            screen = ItemDetailsScreen::create();
            break;
        case SCREEN_CONFIRM:
            screen = ConfirmScreen::create();
            break;
        // Other screens will be implemented later
        default:
            DEBUG_PRINTF("Screen type %d not implemented yet", screenType);
            return;
    }
    
    if (screen) {
        // Load screen
        lv_scr_load(screen);
        
        // Update current screen
        _currentScreen = screen;
        _currentScreenType = screenType;
        
        // Notify app controller about screen change
        AppController::handleScreenChange(screenType);
        
        DEBUG_PRINTF("Screen changed to %d", screenType);
    }
}

ScreenType UIManager::getCurrentScreenType() {
    return _currentScreenType;
}

void UIManager::showLoading(const char* message) {
    if (!_initialized) {
        return;
    }
    
    // Hide existing loading screen if any
    hideLoading();
    
    // Create loading screen
    _loadingScreen = lv_obj_create(lv_layer_top());
    lv_obj_set_size(_loadingScreen, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    lv_obj_set_style_bg_color(_loadingScreen, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(_loadingScreen, LV_OPA_50, 0);
    
    // Create spinner
    lv_obj_t* spinner = lv_spinner_create(_loadingScreen, 1000, 60);
    lv_obj_set_size(spinner, 80, 80);
    lv_obj_align(spinner, LV_ALIGN_CENTER, 0, -20);
    
    // Create message label
    lv_obj_t* label = lv_label_create(_loadingScreen);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(label, message ? message : "Loading...");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 50);
}

void UIManager::hideLoading() {
    if (!_initialized || !_loadingScreen) {
        return;
    }
    
    // Delete loading screen
    lv_obj_del(_loadingScreen);
    _loadingScreen = nullptr;
}

void UIManager::showAlert(const char* title, const char* message, const char* buttonText, 
                        lv_event_cb_t callback, void* userData) {
    if (!_initialized) {
        return;
    }
    
    // Hide existing alert dialog if any
    if (_alertDialog) {
        lv_obj_del(_alertDialog);
        _alertDialog = nullptr;
    }
    
    // Create alert dialog
    _alertDialog = lv_obj_create(lv_layer_top());
    lv_obj_set_size(_alertDialog, DISPLAY_WIDTH - 40, 200);
    lv_obj_align(_alertDialog, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(_alertDialog, lv_color_hex(0x303030), 0);
    lv_obj_set_style_border_width(_alertDialog, 1, 0);
    lv_obj_set_style_border_color(_alertDialog, lv_color_hex(0x505050), 0);
    lv_obj_set_style_shadow_width(_alertDialog, 20, 0);
    lv_obj_set_style_shadow_ofs_y(_alertDialog, 5, 0);
    lv_obj_set_style_shadow_opa(_alertDialog, LV_OPA_30, 0);
    lv_obj_set_style_pad_all(_alertDialog, 15, 0);
    
    // Create title label
    lv_obj_t* titleLabel = lv_label_create(_alertDialog);
    lv_obj_set_style_text_font(titleLabel, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(titleLabel, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(titleLabel, title);
    lv_obj_align(titleLabel, LV_ALIGN_TOP_MID, 0, 0);
    
    // Create message label
    lv_obj_t* messageLabel = lv_label_create(_alertDialog);
    lv_obj_set_style_text_font(messageLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(messageLabel, lv_color_hex(0xCCCCCC), 0);
    lv_label_set_long_mode(messageLabel, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(messageLabel, lv_pct(100));
    lv_label_set_text(messageLabel, message);
    lv_obj_align(messageLabel, LV_ALIGN_TOP_MID, 0, 30);
    
    // Create button
    lv_obj_t* button = lv_btn_create(_alertDialog);
    lv_obj_set_size(button, 120, 40);
    lv_obj_align(button, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_set_style_bg_color(button, lv_color_hex(0x0078D7), 0); // Blue button
    
    // Store callback and user data
    lv_obj_set_user_data(button, userData);
    
    // Add click event
    if (callback) {
        lv_obj_add_event_cb(button, callback, LV_EVENT_CLICKED, NULL);
    } else {
        lv_obj_add_event_cb(button, _alertButtonClickHandler, LV_EVENT_CLICKED, NULL);
    }
    
    // Create button label
    lv_obj_t* buttonLabel = lv_label_create(button);
    lv_label_set_text(buttonLabel, buttonText ? buttonText : "OK");
    lv_obj_center(buttonLabel);
}

void UIManager::showConfirm(const char* title, const char* message, 
                          const char* confirmText, const char* cancelText,
                          lv_event_cb_t confirmCallback, lv_event_cb_t cancelCallback, 
                          void* userData) {
    if (!_initialized) {
        return;
    }
    
    // Hide existing confirm dialog if any
    if (_confirmDialog) {
        lv_obj_del(_confirmDialog);
        _confirmDialog = nullptr;
    }
    
    // Create confirm dialog
    _confirmDialog = lv_obj_create(lv_layer_top());
    lv_obj_set_size(_confirmDialog, DISPLAY_WIDTH - 40, 200);
    lv_obj_align(_confirmDialog, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(_confirmDialog, lv_color_hex(0x303030), 0);
    lv_obj_set_style_border_width(_confirmDialog, 1, 0);
    lv_obj_set_style_border_color(_confirmDialog, lv_color_hex(0x505050), 0);
    lv_obj_set_style_shadow_width(_confirmDialog, 20, 0);
    lv_obj_set_style_shadow_ofs_y(_confirmDialog, 5, 0);
    lv_obj_set_style_shadow_opa(_confirmDialog, LV_OPA_30, 0);
    lv_obj_set_style_pad_all(_confirmDialog, 15, 0);
    
    // Create title label
    lv_obj_t* titleLabel = lv_label_create(_confirmDialog);
    lv_obj_set_style_text_font(titleLabel, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(titleLabel, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(titleLabel, title);
    lv_obj_align(titleLabel, LV_ALIGN_TOP_MID, 0, 0);
    
    // Create message label
    lv_obj_t* messageLabel = lv_label_create(_confirmDialog);
    lv_obj_set_style_text_font(messageLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(messageLabel, lv_color_hex(0xCCCCCC), 0);
    lv_label_set_long_mode(messageLabel, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(messageLabel, lv_pct(100));
    lv_label_set_text(messageLabel, message);
    lv_obj_align(messageLabel, LV_ALIGN_TOP_MID, 0, 30);
    
    // Create button container
    lv_obj_t* btnContainer = lv_obj_create(_confirmDialog);
    lv_obj_set_size(btnContainer, lv_pct(100), 50);
    lv_obj_align(btnContainer, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_set_style_bg_opa(btnContainer, LV_OPA_0, 0);
    lv_obj_set_style_border_width(btnContainer, 0, 0);
    lv_obj_set_style_pad_all(btnContainer, 0, 0);
    lv_obj_set_flex_flow(btnContainer, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(btnContainer, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    // Create confirm button
    lv_obj_t* confirmBtn = lv_btn_create(btnContainer);
    lv_obj_set_size(confirmBtn, 120, 40);
    lv_obj_set_style_bg_color(confirmBtn, lv_color_hex(0x0078D7), 0); // Blue button
    
    // Store callback and user data
    lv_obj_set_user_data(confirmBtn, userData);
    
    // Add click event
    if (confirmCallback) {
        lv_obj_add_event_cb(confirmBtn, confirmCallback, LV_EVENT_CLICKED, NULL);
    } else {
        lv_obj_add_event_cb(confirmBtn, _confirmButtonClickHandler, LV_EVENT_CLICKED, NULL);
    }
    
    // Create confirm button label
    lv_obj_t* confirmBtnLabel = lv_label_create(confirmBtn);
    lv_label_set_text(confirmBtnLabel, confirmText ? confirmText : "OK");
    lv_obj_center(confirmBtnLabel);
    
    // Create cancel button
    lv_obj_t* cancelBtn = lv_btn_create(btnContainer);
    lv_obj_set_size(cancelBtn, 120, 40);
    lv_obj_set_style_bg_color(cancelBtn, lv_color_hex(0x505050), 0); // Gray button
    
    // Store callback and user data
    lv_obj_set_user_data(cancelBtn, userData);
    
    // Add click event
    if (cancelCallback) {
        lv_obj_add_event_cb(cancelBtn, cancelCallback, LV_EVENT_CLICKED, NULL);
    } else {
        lv_obj_add_event_cb(cancelBtn, _cancelButtonClickHandler, LV_EVENT_CLICKED, NULL);
    }
    
    // Create cancel button label
    lv_obj_t* cancelBtnLabel = lv_label_create(cancelBtn);
    lv_label_set_text(cancelBtnLabel, cancelText ? cancelText : "Cancel");
    lv_obj_center(cancelBtnLabel);
}

void UIManager::showKeyboard(lv_obj_t* textArea) {
    if (!_initialized || !_keyboard) {
        return;
    }
    
    // Show keyboard
    lv_keyboard_set_textarea(_keyboard, textArea);
    lv_obj_clear_flag(_keyboard, LV_OBJ_FLAG_HIDDEN);
    
    // Add event handler to text area
    lv_obj_add_event_cb(textArea, _keyboardEventHandler, LV_EVENT_DEFOCUSED, NULL);
}

void UIManager::hideKeyboard() {
    if (!_initialized || !_keyboard) {
        return;
    }
    
    // Hide keyboard
    lv_obj_add_flag(_keyboard, LV_OBJ_FLAG_HIDDEN);
    lv_keyboard_set_textarea(_keyboard, NULL);
}

void UIManager::_alertButtonClickHandler(lv_event_t* e) {
    // Close alert dialog
    if (_alertDialog) {
        lv_obj_del(_alertDialog);
        _alertDialog = nullptr;
    }
}

void UIManager::_confirmButtonClickHandler(lv_event_t* e) {
    // Close confirm dialog
    if (_confirmDialog) {
        lv_obj_del(_confirmDialog);
        _confirmDialog = nullptr;
    }
}

void UIManager::_cancelButtonClickHandler(lv_event_t* e) {
    // Close confirm dialog
    if (_confirmDialog) {
        lv_obj_del(_confirmDialog);
        _confirmDialog = nullptr;
    }
}

void UIManager::_keyboardEventHandler(lv_event_t* e) {
    lv_obj_t* textArea = lv_event_get_target(e);
    
    // Hide keyboard when text area loses focus
    hideKeyboard();
}