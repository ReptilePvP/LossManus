/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Main Menu Screen Implementation
 * 
 * Compatible with LVGL 8.4.0
 */

#include "main_menu_screen.h"
#include "../../hal/power.h"
#include "../../data/database.h"
#include "../../data/sync.h"
#include "../../connectivity/wifi_manager.h"

lv_obj_t* MainMenuScreen::create() {
    // Create screen
    lv_obj_t* screen = lv_obj_create(NULL);
    lv_obj_set_size(screen, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x202020), 0); // Dark background
    
    // Add status bar
    StatusBar::create(screen);
    
    // Create title
    lv_obj_t* title = lv_label_create(screen);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_22, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(title, "Loss Prevention Log");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 30);
    
    // Create version label
    lv_obj_t* versionLabel = lv_label_create(screen);
    lv_obj_set_style_text_font(versionLabel, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(versionLabel, lv_color_hex(0xAAAAAA), 0);
    lv_label_set_text(versionLabel, "v" APP_VERSION);
    lv_obj_align(versionLabel, LV_ALIGN_TOP_MID, 0, 60);
    
    // Create menu container
    lv_obj_t* menuContainer = lv_obj_create(screen);
    lv_obj_set_size(menuContainer, DISPLAY_WIDTH - 40, DISPLAY_HEIGHT - 100);
    lv_obj_align(menuContainer, LV_ALIGN_CENTER, 0, 20);
    lv_obj_set_style_bg_opa(menuContainer, LV_OPA_0, 0);
    lv_obj_set_style_border_width(menuContainer, 0, 0);
    lv_obj_set_flex_flow(menuContainer, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(menuContainer, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_all(menuContainer, 5, 0);
    
    // Calculate card size
    int cardWidth = (lv_obj_get_width(menuContainer) - 20) / 2;
    int cardHeight = (lv_obj_get_height(menuContainer) - 20) / 3;
    
    // Create New Entry card
    lv_obj_t* newEntryCard = Card::create(menuContainer, cardWidth, cardHeight, "New Entry", LV_SYMBOL_PLUS);
    Card::setClickable(newEntryCard, true, _newEntryBtnClickHandler);
    
    // Create View Logs card
    lv_obj_t* viewLogsCard = Card::create(menuContainer, cardWidth, cardHeight, "View Logs", LV_SYMBOL_LIST);
    Card::setClickable(viewLogsCard, true, _viewLogsBtnClickHandler);
    
    // Create Export card
    lv_obj_t* exportCard = Card::create(menuContainer, cardWidth, cardHeight, "Export", LV_SYMBOL_DOWNLOAD);
    Card::setClickable(exportCard, true, _exportBtnClickHandler);
    
    // Create Sync card
    lv_obj_t* syncCard = Card::create(menuContainer, cardWidth, cardHeight, "Sync", LV_SYMBOL_REFRESH);
    Card::setClickable(syncCard, true, _syncBtnClickHandler);
    
    // Create Settings card
    lv_obj_t* settingsCard = Card::create(menuContainer, cardWidth, cardHeight, "Settings", LV_SYMBOL_SETTINGS);
    Card::setClickable(settingsCard, true, _settingsBtnClickHandler);
    
    // Create About card
    lv_obj_t* aboutCard = Card::create(menuContainer, cardWidth, cardHeight, "About", LV_SYMBOL_HELP);
    Card::setClickable(aboutCard, true, _aboutBtnClickHandler);
    
    // Create battery status
    lv_obj_t* batteryLabel = lv_label_create(screen);
    lv_obj_set_style_text_font(batteryLabel, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(batteryLabel, lv_color_hex(0xAAAAAA), 0);
    int batteryLevel = PowerHAL::getBatteryLevel();
    String batteryText = "Battery: " + String(batteryLevel) + "%";
    if (PowerHAL::isCharging()) {
        batteryText += " (Charging)";
    }
    lv_label_set_text(batteryLabel, batteryText.c_str());
    lv_obj_align(batteryLabel, LV_ALIGN_BOTTOM_MID, 0, -10);
    
    // Update screen
    update(screen);
    
    return screen;
}

void MainMenuScreen::update(lv_obj_t* screen) {
    if (!screen) {
        return;
    }
    
    // Update status bar if exists
    lv_obj_t* statusBar = lv_obj_get_child(screen, 0);
    if (statusBar) {
        StatusBar::update(statusBar);
    }
    
    // Find menu container
    lv_obj_t* menuContainer = NULL;
    for (int i = 0; i < lv_obj_get_child_cnt(screen); i++) {
        lv_obj_t* child = lv_obj_get_child(screen, i);
        if (lv_obj_get_child_cnt(child) >= 6) { // Menu container has at least 6 cards
            menuContainer = child;
            break;
        }
    }
    
    if (!menuContainer) {
        return;
    }
    
    // Update View Logs card with count
    lv_obj_t* viewLogsCard = lv_obj_get_child(menuContainer, 1);
    if (viewLogsCard) {
        int logCount = Database::getEntryCount();
        String content = String(logCount) + " entries";
        Card::setContent(viewLogsCard, content.c_str());
    }
    
    // Update Sync card with status
    lv_obj_t* syncCard = lv_obj_get_child(menuContainer, 3);
    if (syncCard) {
        String content;
        if (WiFiManager::isConnected()) {
            size_t pendingCount = SyncManager::getPendingSyncCount();
            if (pendingCount > 0) {
                content = String(pendingCount) + " pending";
            } else {
                content = "All synced";
            }
        } else {
            content = "WiFi disconnected";
        }
        Card::setContent(syncCard, content.c_str());
    }
    
    // Update battery label
    lv_obj_t* batteryLabel = NULL;
    for (int i = 0; i < lv_obj_get_child_cnt(screen); i++) {
        lv_obj_t* child = lv_obj_get_child(screen, i);
        const char* text = lv_label_get_text(child);
        if (text && strstr(text, "Battery:") != NULL) {
            batteryLabel = child;
            break;
        }
    }
    
    if (batteryLabel) {
        int batteryLevel = PowerHAL::getBatteryLevel();
        String batteryText = "Battery: " + String(batteryLevel) + "%";
        if (PowerHAL::isCharging()) {
            batteryText += " (Charging)";
        }
        lv_label_set_text(batteryLabel, batteryText.c_str());
    }
}

void MainMenuScreen::_newEntryBtnClickHandler(lv_event_t* e) {
    UIManager::setScreen(SCREEN_GENDER);
}

void MainMenuScreen::_viewLogsBtnClickHandler(lv_event_t* e) {
    UIManager::setScreen(SCREEN_LOGS);
}

void MainMenuScreen::_exportBtnClickHandler(lv_event_t* e) {
    // Show export options dialog
    // TODO: Implement export options dialog
    
    // For now, just show a message
    lv_obj_t* screen = lv_scr_act();
    StatusBar::showNotification(lv_obj_get_child(screen, 0), "Export feature coming soon");
}

void MainMenuScreen::_syncBtnClickHandler(lv_event_t* e) {
    if (!WiFiManager::isConnected()) {
        UIManager::setScreen(SCREEN_WIFI);
        return;
    }
    
    // Show sync dialog
    // TODO: Implement sync dialog
    
    // For now, just process sync queue
    UIManager::showLoading("Syncing data...");
    
    // Process sync queue in a separate task
    SyncManager::processSyncQueue();
    
    // Hide loading after a delay
    lv_timer_t* timer = lv_timer_create([](lv_timer_t* timer) {
        UIManager::hideLoading();
        lv_obj_t* screen = lv_scr_act();
        StatusBar::showNotification(lv_obj_get_child(screen, 0), "Sync completed");
    }, 2000, NULL);
    lv_timer_set_repeat_count(timer, 1);
}

void MainMenuScreen::_settingsBtnClickHandler(lv_event_t* e) {
    UIManager::setScreen(SCREEN_SETTINGS);
}

void MainMenuScreen::_aboutBtnClickHandler(lv_event_t* e) {
    UIManager::setScreen(SCREEN_ABOUT);
}
