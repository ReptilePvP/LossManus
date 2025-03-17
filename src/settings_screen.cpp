/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Settings Screen Implementation
 * 
 * Compatible with LVGL 8.4.0
 */

#include "settings_screen.h"
#include "../ui_manager.h"
#include "../components/status_bar.h"
#include "../../app/app_controller.h"
#include <Preferences.h>

lv_obj_t* SettingsScreen::create() {
    DEBUG_PRINTLN("Creating settings screen...");
    
    // Create screen
    lv_obj_t* screen = lv_obj_create(nullptr);
    lv_obj_set_size(screen, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x202020), 0);
    
    // Create status bar
    StatusBar::create(screen);
    
    // Create title
    lv_obj_t* title = lv_label_create(screen);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_22, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(title, "Settings");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 40);
    
    // Create back button
    lv_obj_t* backBtn = lv_btn_create(screen);
    lv_obj_set_size(backBtn, 50, 50);
    lv_obj_align(backBtn, LV_ALIGN_TOP_LEFT, 10, 30);
    lv_obj_set_style_bg_color(backBtn, lv_color_hex(0x404040), 0);
    lv_obj_set_style_bg_opa(backBtn, LV_OPA_50, 0);
    lv_obj_set_style_border_width(backBtn, 0, 0);
    lv_obj_set_style_radius(backBtn, 25, 0);
    lv_obj_add_event_cb(backBtn, _backButtonClickHandler, LV_EVENT_CLICKED, nullptr);
    
    // Create back button icon
    lv_obj_t* backIcon = lv_label_create(backBtn);
    lv_label_set_text(backIcon, LV_SYMBOL_LEFT);
    lv_obj_set_style_text_font(backIcon, &lv_font_montserrat_24, 0);
    lv_obj_center(backIcon);
    
    // Create settings container
    lv_obj_t* container = lv_obj_create(screen);
    lv_obj_set_size(container, DISPLAY_WIDTH - 20, DISPLAY_HEIGHT - 120);
    lv_obj_align(container, LV_ALIGN_TOP_MID, 0, 100);
    lv_obj_set_style_bg_color(container, lv_color_hex(0x303030), 0);
    lv_obj_set_style_border_width(container, 0, 0);
    lv_obj_set_style_radius(container, 10, 0);
    lv_obj_set_style_pad_all(container, 15, 0);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    
    // Load current settings
    Preferences prefs;
    prefs.begin("settings", false);
    uint8_t brightness = prefs.getUChar("brightness", 128);
    uint8_t volume = prefs.getUChar("volume", 128);
    bool soundEnabled = prefs.getBool("sound_enabled", true);
    prefs.end();
    
    // Create brightness section
    lv_obj_t* brightnessLabel = lv_label_create(container);
    lv_obj_set_style_text_font(brightnessLabel, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(brightnessLabel, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(brightnessLabel, "Display Brightness");
    
    lv_obj_t* brightnessSlider = lv_slider_create(container);
    lv_obj_set_width(brightnessSlider, lv_pct(100));
    lv_obj_set_style_bg_color(brightnessSlider, lv_color_hex(0x505050), LV_PART_MAIN);
    lv_obj_set_style_bg_color(brightnessSlider, lv_color_hex(0x0078D7), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(brightnessSlider, lv_color_hex(0x0078D7), LV_PART_KNOB);
    lv_slider_set_range(brightnessSlider, 10, 255);
    lv_slider_set_value(brightnessSlider, brightness, LV_ANIM_OFF);
    lv_obj_add_event_cb(brightnessSlider, _brightnessSliderEventHandler, LV_EVENT_VALUE_CHANGED, nullptr);
    
    // Add spacer
    lv_obj_t* spacer1 = lv_obj_create(container);
    lv_obj_set_size(spacer1, lv_pct(100), 20);
    lv_obj_set_style_bg_opa(spacer1, LV_OPA_0, 0);
    lv_obj_set_style_border_width(spacer1, 0, 0);
    
    // Create volume section
    lv_obj_t* volumeLabel = lv_label_create(container);
    lv_obj_set_style_text_font(volumeLabel, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(volumeLabel, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(volumeLabel, "Sound Volume");
    
    lv_obj_t* volumeSlider = lv_slider_create(container);
    lv_obj_set_width(volumeSlider, lv_pct(100));
    lv_obj_set_style_bg_color(volumeSlider, lv_color_hex(0x505050), LV_PART_MAIN);
    lv_obj_set_style_bg_color(volumeSlider, lv_color_hex(0x0078D7), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(volumeSlider, lv_color_hex(0x0078D7), LV_PART_KNOB);
    lv_slider_set_range(volumeSlider, 0, 255);
    lv_slider_set_value(volumeSlider, volume, LV_ANIM_OFF);
    lv_obj_add_event_cb(volumeSlider, _volumeSliderEventHandler, LV_EVENT_VALUE_CHANGED, nullptr);
    
    // Add spacer
    lv_obj_t* spacer2 = lv_obj_create(container);
    lv_obj_set_size(spacer2, lv_pct(100), 20);
    lv_obj_set_style_bg_opa(spacer2, LV_OPA_0, 0);
    lv_obj_set_style_border_width(spacer2, 0, 0);
    
    // Create sound enable section
    lv_obj_t* soundRow = lv_obj_create(container);
    lv_obj_set_size(soundRow, lv_pct(100), 40);
    lv_obj_set_style_bg_opa(soundRow, LV_OPA_0, 0);
    lv_obj_set_style_border_width(soundRow, 0, 0);
    lv_obj_set_style_pad_all(soundRow, 0, 0);
    lv_obj_set_flex_flow(soundRow, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(soundRow, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    lv_obj_t* soundLabel = lv_label_create(soundRow);
    lv_obj_set_style_text_font(soundLabel, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(soundLabel, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(soundLabel, "Sound Enabled");
    
    lv_obj_t* soundSwitch = lv_switch_create(soundRow);
    lv_obj_set_style_bg_color(soundSwitch, lv_color_hex(0x505050), LV_PART_MAIN);
    lv_obj_set_style_bg_color(soundSwitch, lv_color_hex(0x0078D7), LV_PART_INDICATOR);
    if (soundEnabled) {
        lv_obj_add_state(soundSwitch, LV_STATE_CHECKED);
    } else {
        lv_obj_clear_state(soundSwitch, LV_STATE_CHECKED);
    }
    lv_obj_add_event_cb(soundSwitch, _soundSwitchEventHandler, LV_EVENT_VALUE_CHANGED, nullptr);
    
    // Add spacer
    lv_obj_t* spacer3 = lv_obj_create(container);
    lv_obj_set_size(spacer3, lv_pct(100), 30);
    lv_obj_set_style_bg_opa(spacer3, LV_OPA_0, 0);
    lv_obj_set_style_border_width(spacer3, 0, 0);
    
    // Create WiFi button
    lv_obj_t* wifiBtn = lv_btn_create(container);
    lv_obj_set_size(wifiBtn, lv_pct(100), 50);
    lv_obj_set_style_bg_color(wifiBtn, lv_color_hex(0x0078D7), 0);
    lv_obj_add_event_cb(wifiBtn, _wifiButtonClickHandler, LV_EVENT_CLICKED, nullptr);
    
    lv_obj_t* wifiLabel = lv_label_create(wifiBtn);
    lv_label_set_text(wifiLabel, LV_SYMBOL_WIFI " WiFi Settings");
    lv_obj_center(wifiLabel);
    
    // Add spacer
    lv_obj_t* spacer4 = lv_obj_create(container);
    lv_obj_set_size(spacer4, lv_pct(100), 10);
    lv_obj_set_style_bg_opa(spacer4, LV_OPA_0, 0);
    lv_obj_set_style_border_width(spacer4, 0, 0);
    
    // Create About button
    lv_obj_t* aboutBtn = lv_btn_create(container);
    lv_obj_set_size(aboutBtn, lv_pct(100), 50);
    lv_obj_set_style_bg_color(aboutBtn, lv_color_hex(0x107C10), 0);
    lv_obj_add_event_cb(aboutBtn, _aboutButtonClickHandler, LV_EVENT_CLICKED, nullptr);
    
    lv_obj_t* aboutLabel = lv_label_create(aboutBtn);
    lv_label_set_text(aboutLabel, LV_SYMBOL_SETTINGS " About");
    lv_obj_center(aboutLabel);
    
    // Add spacer
    lv_obj_t* spacer5 = lv_obj_create(container);
    lv_obj_set_size(spacer5, lv_pct(100), 10);
    lv_obj_set_style_bg_opa(spacer5, LV_OPA_0, 0);
    lv_obj_set_style_border_width(spacer5, 0, 0);
    
    // Create Reset button
    lv_obj_t* resetBtn = lv_btn_create(container);
    lv_obj_set_size(resetBtn, lv_pct(100), 50);
    lv_obj_set_style_bg_color(resetBtn, lv_color_hex(0xE81123), 0);
    lv_obj_add_event_cb(resetBtn, _resetButtonClickHandler, LV_EVENT_CLICKED, nullptr);
    
    lv_obj_t* resetLabel = lv_label_create(resetBtn);
    lv_label_set_text(resetLabel, LV_SYMBOL_TRASH " Reset All Settings");
    lv_obj_center(resetLabel);
    
    DEBUG_PRINTLN("Settings screen created successfully!");
    return screen;
}

void SettingsScreen::update(lv_obj_t* screen) {
    // Update status bar
    StatusBar::update();
    
    // Any other dynamic updates can be added here
}

void SettingsScreen::_backButtonClickHandler(lv_event_t* e) {
    DEBUG_PRINTLN("Back button clicked");
    UIManager::setScreen(SCREEN_MAIN_MENU);
}

void SettingsScreen::_brightnessSliderEventHandler(lv_event_t* e) {
    lv_obj_t* slider = lv_event_get_target(e);
    int32_t value = lv_slider_get_value(slider);
    DEBUG_PRINTF("Brightness changed to %d\n", value);
    
    // Update display brightness
    M5.Display.setBrightness(value);
    
    // Save setting
    Preferences prefs;
    prefs.begin("settings", false);
    prefs.putUChar("brightness", value);
    prefs.end();
}

void SettingsScreen::_volumeSliderEventHandler(lv_event_t* e) {
    lv_obj_t* slider = lv_event_get_target(e);
    int32_t value = lv_slider_get_value(slider);
    DEBUG_PRINTF("Volume changed to %d\n", value);
    
    // Get sound enabled state
    Preferences prefs;
    prefs.begin("settings", false);
    bool soundEnabled = prefs.getBool("sound_enabled", true);
    
    // Update speaker volume
    if (soundEnabled) {
        M5.Speaker.setVolume(value);
    }
    
    // Save setting
    prefs.putUChar("volume", value);
    prefs.end();
}

void SettingsScreen::_soundSwitchEventHandler(lv_event_t* e) {
    lv_obj_t* sw = lv_event_get_target(e);
    bool enabled = lv_obj_has_state(sw, LV_STATE_CHECKED);
    DEBUG_PRINTF("Sound enabled changed to %d\n", enabled);
    
    // Load current volume
    Preferences prefs;
    prefs.begin("settings", false);
    uint8_t volume = prefs.getUChar("volume", 128);
    
    // Update speaker volume
    M5.Speaker.setVolume(enabled ? volume : 0);
    
    // Save setting
    prefs.putBool("sound_enabled", enabled);
    prefs.end();
}

void SettingsScreen::_wifiButtonClickHandler(lv_event_t* e) {
    DEBUG_PRINTLN("WiFi button clicked");
    UIManager::setScreen(SCREEN_WIFI);
}

void SettingsScreen::_aboutButtonClickHandler(lv_event_t* e) {
    DEBUG_PRINTLN("About button clicked");
    UIManager::setScreen(SCREEN_ABOUT);
}

void SettingsScreen::_resetButtonClickHandler(lv_event_t* e) {
    DEBUG_PRINTLN("Reset button clicked");
    _confirmReset();
}

void SettingsScreen::_saveSettings() {
    // This method would save all settings to persistent storage
    // Currently, each setting is saved individually when changed
}

void SettingsScreen::_loadSettings() {
    // This method would load all settings from persistent storage
    // Currently, each setting is loaded when the screen is created
}

void SettingsScreen::_confirmReset() {
    UIManager::showConfirm(
        "Reset Settings",
        "Are you sure you want to reset all settings to default values? This action cannot be undone.",
        "Reset",
        "Cancel",
        [](lv_event_t* e) {
            // Reset confirmed
            DEBUG_PRINTLN("Reset confirmed");
            
            // Reset settings to defaults
            Preferences prefs;
            prefs.begin("settings", false);
            prefs.putUChar("brightness", 128);
            prefs.putUChar("volume", 128);
            prefs.putBool("sound_enabled", true);
            prefs.end();
            
            // Apply default settings
            M5.Display.setBrightness(128);
            M5.Speaker.setVolume(128);
            
            // Show success message
            UIManager::showAlert("Settings Reset", "All settings have been reset to default values.", "OK", 
                [](lv_event_t* e) {
                    // Recreate settings screen to reflect changes
                    UIManager::setScreen(SCREEN_SETTINGS);
                }
            );
        }
    );
}
