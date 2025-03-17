/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - UI Manager
 * 
 * This file contains the interface for the UI manager
 * Compatible with LVGL 8.4.0
 */

#ifndef UI_UI_MANAGER_H
#define UI_UI_MANAGER_H

#include <Arduino.h>
#include <lvgl.h>
#include "../config.h"

// Screen types
enum ScreenType {
    SCREEN_MAIN_MENU,
    SCREEN_GENDER,
    SCREEN_SHIRT_COLOR,
    SCREEN_PANTS_COLOR,
    SCREEN_SHOES_COLOR,
    SCREEN_ITEM_TYPE,
    SCREEN_ITEM_DETAILS,
    SCREEN_CONFIRM,
    SCREEN_LOGS,
    SCREEN_LOG_DETAIL,
    SCREEN_SETTINGS,
    SCREEN_WIFI,
    SCREEN_ABOUT,
    SCREEN_SEARCH
};

class UIManager {
public:
    /**
     * Initialize UI manager
     * @return true if successful, false otherwise
     */
    static bool init();
    
    /**
     * Update UI
     */
    static void update();
    
    /**
     * Set current screen
     * @param screenType screen type to display
     */
    static void setScreen(ScreenType screenType);
    
    /**
     * Get current screen type
     * @return current screen type
     */
    static ScreenType getCurrentScreenType();
    
    /**
     * Show loading overlay
     * @param message loading message to display
     */
    static void showLoading(const char* message);
    
    /**
     * Hide loading overlay
     */
    static void hideLoading();
    
    /**
     * Show alert dialog
     * @param title alert title
     * @param message alert message
     * @param buttonText button text
     * @param callback button click callback
     * @param userData user data to pass to callback
     */
    static void showAlert(const char* title, const char* message, const char* buttonText, 
                        lv_event_cb_t callback = nullptr, void* userData = nullptr);
    
    /**
     * Show confirmation dialog
     * @param title confirmation title
     * @param message confirmation message
     * @param confirmText confirm button text
     * @param cancelText cancel button text
     * @param confirmCallback confirm button click callback
     * @param cancelCallback cancel button click callback
     * @param userData user data to pass to callbacks
     */
    static void showConfirm(const char* title, const char* message, 
                          const char* confirmText, const char* cancelText,
                          lv_event_cb_t confirmCallback, lv_event_cb_t cancelCallback = nullptr, 
                          void* userData = nullptr);
    
    /**
     * Show keyboard
     * @param textArea text area to attach keyboard to
     */
    static void showKeyboard(lv_obj_t* textArea);
    
    /**
     * Hide keyboard
     */
    static void hideKeyboard();

private:
    static bool _initialized;
    static lv_obj_t* _currentScreen;
    static ScreenType _currentScreenType;
    static lv_obj_t* _loadingScreen;
    static lv_obj_t* _alertDialog;
    static lv_obj_t* _confirmDialog;
    static lv_obj_t* _keyboard;
    
    // Create screens
    static lv_obj_t* _createMainMenuScreen();
    static lv_obj_t* _createGenderScreen();
    static lv_obj_t* _createColorScreen(ScreenType screenType);
    static lv_obj_t* _createItemScreen();
    static lv_obj_t* _createItemDetailsScreen();
    static lv_obj_t* _createConfirmScreen();
    static lv_obj_t* _createLogsScreen();
    static lv_obj_t* _createLogDetailScreen();
    static lv_obj_t* _createSettingsScreen();
    static lv_obj_t* _createWifiScreen();
    static lv_obj_t* _createAboutScreen();
    static lv_obj_t* _createSearchScreen();
    
    // Event handlers
    static void _alertButtonClickHandler(lv_event_t* e);
    static void _confirmButtonClickHandler(lv_event_t* e);
    static void _cancelButtonClickHandler(lv_event_t* e);
    static void _keyboardEventHandler(lv_event_t* e);
};

#endif // UI_UI_MANAGER_H