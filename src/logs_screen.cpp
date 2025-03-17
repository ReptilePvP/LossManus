/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Logs Screen Implementation
 * 
 * Compatible with LVGL 8.4.0
 */

#include "logs_screen.h"
#include "../ui_manager.h"
#include "../components/status_bar.h"
#include "../../data/database.h"
#include "../../data/log_entry.h"
#include "../../data/export.h"
#include "../../data/sync.h"
#include "../../app/app_controller.h"

// Static member initialization
LogScreenMode LogsScreen::_currentMode = LOG_SCREEN_ALL;
lv_obj_t* LogsScreen::_logsList = nullptr;
lv_obj_t* LogsScreen::_modeSelector = nullptr;
int LogsScreen::_selectedLogIndex = -1;

lv_obj_t* LogsScreen::create() {
    DEBUG_PRINTLN("Creating logs screen...");
    
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
    lv_label_set_text(title, "Loss Prevention Logs");
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
    
    // Create mode selector
    _modeSelector = lv_dropdown_create(screen);
    lv_dropdown_set_options(_modeSelector, "All Logs\nToday's Logs\nPending Sync");
    lv_obj_set_size(_modeSelector, 150, 40);
    lv_obj_align(_modeSelector, LV_ALIGN_TOP_RIGHT, -10, 80);
    lv_obj_add_event_cb(_modeSelector, _modeSelectorEventHandler, LV_EVENT_VALUE_CHANGED, nullptr);
    
    // Create logs list
    _logsList = lv_list_create(screen);
    lv_obj_set_size(_logsList, DISPLAY_WIDTH - 20, DISPLAY_HEIGHT - 180);
    lv_obj_align(_logsList, LV_ALIGN_TOP_MID, 0, 130);
    lv_obj_set_style_bg_color(_logsList, lv_color_hex(0x303030), 0);
    lv_obj_set_style_border_width(_logsList, 0, 0);
    lv_obj_set_style_radius(_logsList, 10, 0);
    lv_obj_set_style_pad_all(_logsList, 10, 0);
    
    // Create button container
    lv_obj_t* btnContainer = lv_obj_create(screen);
    lv_obj_set_size(btnContainer, DISPLAY_WIDTH, 60);
    lv_obj_align(btnContainer, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_set_style_bg_opa(btnContainer, LV_OPA_0, 0);
    lv_obj_set_style_border_width(btnContainer, 0, 0);
    lv_obj_set_style_pad_all(btnContainer, 0, 0);
    lv_obj_set_flex_flow(btnContainer, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(btnContainer, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    // Create sync button
    lv_obj_t* syncBtn = lv_btn_create(btnContainer);
    lv_obj_set_size(syncBtn, 140, 50);
    lv_obj_set_style_bg_color(syncBtn, lv_color_hex(0x0078D7), 0);
    lv_obj_add_event_cb(syncBtn, _syncButtonClickHandler, LV_EVENT_CLICKED, nullptr);
    
    // Create sync button label
    lv_obj_t* syncLabel = lv_label_create(syncBtn);
    lv_label_set_text(syncLabel, LV_SYMBOL_REFRESH " Sync");
    lv_obj_center(syncLabel);
    
    // Create export button
    lv_obj_t* exportBtn = lv_btn_create(btnContainer);
    lv_obj_set_size(exportBtn, 140, 50);
    lv_obj_set_style_bg_color(exportBtn, lv_color_hex(0x107C10), 0);
    lv_obj_add_event_cb(exportBtn, _exportButtonClickHandler, LV_EVENT_CLICKED, nullptr);
    
    // Create export button label
    lv_obj_t* exportLabel = lv_label_create(exportBtn);
    lv_label_set_text(exportLabel, LV_SYMBOL_DOWNLOAD " Export");
    lv_obj_center(exportLabel);
    
    // Load logs
    _loadLogs(_logsList, _currentMode);
    
    DEBUG_PRINTLN("Logs screen created successfully!");
    return screen;
}

void LogsScreen::update(lv_obj_t* screen) {
    // Update status bar
    StatusBar::update();
    
    // Any other dynamic updates can be added here
}

void LogsScreen::setMode(LogScreenMode mode) {
    _currentMode = mode;
    
    // Update dropdown selection
    if (_modeSelector) {
        lv_dropdown_set_selected(_modeSelector, (uint16_t)mode);
    }
    
    // Refresh logs
    refreshLogs();
}

void LogsScreen::refreshLogs() {
    if (_logsList) {
        // Clear list
        lv_obj_clean(_logsList);
        
        // Load logs
        _loadLogs(_logsList, _currentMode);
    }
}

void LogsScreen::_backButtonClickHandler(lv_event_t* e) {
    DEBUG_PRINTLN("Back button clicked");
    UIManager::setScreen(SCREEN_MAIN_MENU);
}

void LogsScreen::_modeSelectorEventHandler(lv_event_t* e) {
    uint16_t selected = lv_dropdown_get_selected(lv_event_get_target(e));
    DEBUG_PRINTF("Mode selector changed to %d\n", selected);
    
    // Set mode
    _currentMode = (LogScreenMode)selected;
    
    // Refresh logs
    refreshLogs();
}

void LogsScreen::_logItemClickHandler(lv_event_t* e) {
    lv_obj_t* btn = lv_event_get_target(e);
    int index = (int)lv_obj_get_user_data(btn);
    DEBUG_PRINTF("Log item clicked: %d\n", index);
    
    // Store selected log index
    _selectedLogIndex = index;
    
    // Navigate to log detail screen
    UIManager::setScreen(SCREEN_LOG_DETAIL);
}

void LogsScreen::_syncButtonClickHandler(lv_event_t* e) {
    DEBUG_PRINTLN("Sync button clicked");
    
    // Show loading
    UIManager::showLoading("Syncing logs...");
    
    // Start sync process
    // This would typically be done asynchronously
    // For now, we'll simulate it with a delay
    
    // In a real implementation, this would be:
    // SyncManager::syncLogs([](bool success) {
    //     UIManager::hideLoading();
    //     if (success) {
    //         UIManager::showAlert("Sync Complete", "All logs have been synchronized.", "OK");
    //     } else {
    //         UIManager::showAlert("Sync Failed", "Failed to synchronize logs. Please try again.", "OK");
    //     }
    // });
    
    // Simulate sync process
    delay(2000);
    
    // Hide loading
    UIManager::hideLoading();
    
    // Show success message
    UIManager::showAlert("Sync Complete", "All logs have been synchronized.", "OK");
    
    // Refresh logs
    refreshLogs();
}

void LogsScreen::_exportButtonClickHandler(lv_event_t* e) {
    DEBUG_PRINTLN("Export button clicked");
    
    // Show loading
    UIManager::showLoading("Exporting logs...");
    
    // Start export process
    // This would typically be done asynchronously
    // For now, we'll simulate it with a delay
    
    // In a real implementation, this would be:
    // ExportManager::exportLogs([](bool success, const char* path) {
    //     UIManager::hideLoading();
    //     if (success) {
    //         UIManager::showAlert("Export Complete", String("Logs exported to: ") + path, "OK");
    //     } else {
    //         UIManager::showAlert("Export Failed", "Failed to export logs. Please try again.", "OK");
    //     }
    // });
    
    // Simulate export process
    delay(2000);
    
    // Hide loading
    UIManager::hideLoading();
    
    // Show success message
    UIManager::showAlert("Export Complete", "Logs exported to: /logs.csv", "OK");
}

void LogsScreen::_loadLogs(lv_obj_t* list, LogScreenMode mode) {
    DEBUG_PRINTF("Loading logs for mode %d\n", mode);
    
    // In a real implementation, this would load logs from the database
    // For now, we'll create some sample logs
    
    // Sample logs
    const int numLogs = 10;
    
    for (int i = 0; i < numLogs; i++) {
        char title[32];
        char description[64];
        char timestamp[32];
        bool synced = (i % 3 != 0); // Every third log is not synced
        
        // Generate sample data
        snprintf(title, sizeof(title), "Log #%d", i + 1);
        
        const char* genders[] = {"Male", "Female"};
        const char* colors[] = {"Red", "Blue", "Green", "Black", "White"};
        const char* items[] = {"Shirt", "Pants", "Shoes", "Hat", "Jacket"};
        
        int genderIndex = i % 2;
        int shirtColorIndex = i % 5;
        int pantsColorIndex = (i + 2) % 5;
        int itemIndex = i % 5;
        
        snprintf(description, sizeof(description), "%s, %s shirt, %s pants, %s", 
                genders[genderIndex], 
                colors[shirtColorIndex], 
                colors[pantsColorIndex], 
                items[itemIndex]);
        
        // Generate timestamp (today - i days)
        time_t now = time(nullptr);
        struct tm* timeinfo = localtime(&now);
        timeinfo->tm_mday -= i;
        mktime(timeinfo);
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M", timeinfo);
        
        // Skip logs that don't match the current mode
        if (mode == LOG_SCREEN_TODAY && i > 0) {
            continue; // Only show today's logs
        } else if (mode == LOG_SCREEN_PENDING && synced) {
            continue; // Only show pending logs
        }
        
        // Create log item
        _createLogItem(list, title, description, timestamp, synced, i);
    }
}

void LogsScreen::_createLogItem(lv_obj_t* parent, const char* title, const char* description, 
                             const char* timestamp, bool synced, int index) {
    // Create list button
    lv_obj_t* btn = lv_list_add_btn(parent, nullptr, "");
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x404040), 0);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x505050), LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x606060), LV_STATE_FOCUSED);
    lv_obj_set_style_pad_all(btn, 10, 0);
    lv_obj_set_height(btn, 80);
    lv_obj_add_event_cb(btn, _logItemClickHandler, LV_EVENT_CLICKED, nullptr);
    
    // Store index as user data
    lv_obj_set_user_data(btn, (void*)(intptr_t)index);
    
    // Create container for content
    lv_obj_t* container = lv_obj_create(btn);
    lv_obj_set_size(container, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_opa(container, LV_OPA_0, 0);
    lv_obj_set_style_border_width(container, 0, 0);
    lv_obj_set_style_pad_all(container, 0, 0);
    
    // Create title label
    lv_obj_t* titleLabel = lv_label_create(container);
    lv_obj_set_style_text_font(titleLabel, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(titleLabel, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(titleLabel, title);
    lv_obj_align(titleLabel, LV_ALIGN_TOP_LEFT, 0, 0);
    
    // Create description label
    lv_obj_t* descLabel = lv_label_create(container);
    lv_obj_set_style_text_font(descLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(descLabel, lv_color_hex(0xCCCCCC), 0);
    lv_label_set_text(descLabel, description);
    lv_obj_align(descLabel, LV_ALIGN_TOP_LEFT, 0, 25);
    
    // Create timestamp label
    lv_obj_t* timeLabel = lv_label_create(container);
    lv_obj_set_style_text_font(timeLabel, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(timeLabel, lv_color_hex(0x999999), 0);
    lv_label_set_text(timeLabel, timestamp);
    lv_obj_align(timeLabel, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    
    // Create sync status icon
    lv_obj_t* syncIcon = lv_label_create(container);
    lv_obj_set_style_text_font(syncIcon, &lv_font_montserrat_16, 0);
    
    if (synced) {
        lv_obj_set_style_text_color(syncIcon, lv_color_hex(0x00CC00), 0);
        lv_label_set_text(syncIcon, LV_SYMBOL_OK);
    } else {
        lv_obj_set_style_text_color(syncIcon, lv_color_hex(0xFFCC00), 0);
        lv_label_set_text(syncIcon, LV_SYMBOL_WARNING);
    }
    
    lv_obj_align(syncIcon, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
}
