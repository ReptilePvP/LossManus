/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Log Detail Screen Implementation
 * 
 * Compatible with LVGL 8.4.0
 */

#include "log_detail_screen.h"
#include "../ui_manager.h"
#include "../components/status_bar.h"
#include "../../data/database.h"
#include "../../data/log_entry.h"
#include "../../data/sync.h"
#include "../../app/app_controller.h"

// Static member initialization
int LogDetailScreen::_logIndex = -1;

lv_obj_t* LogDetailScreen::create() {
    DEBUG_PRINTLN("Creating log detail screen...");
    
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
    lv_label_set_text(title, "Log Details");
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
    
    // Create content container
    lv_obj_t* container = lv_obj_create(screen);
    lv_obj_set_size(container, DISPLAY_WIDTH - 20, DISPLAY_HEIGHT - 180);
    lv_obj_align(container, LV_ALIGN_TOP_MID, 0, 100);
    lv_obj_set_style_bg_color(container, lv_color_hex(0x303030), 0);
    lv_obj_set_style_border_width(container, 0, 0);
    lv_obj_set_style_radius(container, 10, 0);
    lv_obj_set_style_pad_all(container, 15, 0);
    
    // Load log details
    _loadLogDetails(container);
    
    // Create button container
    lv_obj_t* btnContainer = lv_obj_create(screen);
    lv_obj_set_size(btnContainer, DISPLAY_WIDTH, 60);
    lv_obj_align(btnContainer, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_set_style_bg_opa(btnContainer, LV_OPA_0, 0);
    lv_obj_set_style_border_width(btnContainer, 0, 0);
    lv_obj_set_style_pad_all(btnContainer, 0, 0);
    lv_obj_set_flex_flow(btnContainer, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(btnContainer, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    // Create edit button
    lv_obj_t* editBtn = lv_btn_create(btnContainer);
    lv_obj_set_size(editBtn, 90, 50);
    lv_obj_set_style_bg_color(editBtn, lv_color_hex(0x0078D7), 0);
    lv_obj_add_event_cb(editBtn, _editButtonClickHandler, LV_EVENT_CLICKED, nullptr);
    
    // Create edit button label
    lv_obj_t* editLabel = lv_label_create(editBtn);
    lv_label_set_text(editLabel, LV_SYMBOL_EDIT " Edit");
    lv_obj_center(editLabel);
    
    // Create sync button
    lv_obj_t* syncBtn = lv_btn_create(btnContainer);
    lv_obj_set_size(syncBtn, 90, 50);
    lv_obj_set_style_bg_color(syncBtn, lv_color_hex(0x107C10), 0);
    lv_obj_add_event_cb(syncBtn, _syncButtonClickHandler, LV_EVENT_CLICKED, nullptr);
    
    // Create sync button label
    lv_obj_t* syncLabel = lv_label_create(syncBtn);
    lv_label_set_text(syncLabel, LV_SYMBOL_UPLOAD " Sync");
    lv_obj_center(syncLabel);
    
    // Create delete button
    lv_obj_t* deleteBtn = lv_btn_create(btnContainer);
    lv_obj_set_size(deleteBtn, 90, 50);
    lv_obj_set_style_bg_color(deleteBtn, lv_color_hex(0xE81123), 0);
    lv_obj_add_event_cb(deleteBtn, _deleteButtonClickHandler, LV_EVENT_CLICKED, nullptr);
    
    // Create delete button label
    lv_obj_t* deleteLabel = lv_label_create(deleteBtn);
    lv_label_set_text(deleteLabel, LV_SYMBOL_TRASH " Delete");
    lv_obj_center(deleteLabel);
    
    DEBUG_PRINTLN("Log detail screen created successfully!");
    return screen;
}

void LogDetailScreen::update(lv_obj_t* screen) {
    // Update status bar
    StatusBar::update();
    
    // Any other dynamic updates can be added here
}

void LogDetailScreen::setLogIndex(int index) {
    _logIndex = index;
}

void LogDetailScreen::_backButtonClickHandler(lv_event_t* e) {
    DEBUG_PRINTLN("Back button clicked");
    UIManager::setScreen(SCREEN_LOGS);
}

void LogDetailScreen::_editButtonClickHandler(lv_event_t* e) {
    DEBUG_PRINTLN("Edit button clicked");
    
    // In a real implementation, this would navigate to edit mode
    // For now, just show a message
    UIManager::showAlert("Edit Log", "Log editing functionality will be implemented in a future update.", "OK");
}

void LogDetailScreen::_deleteButtonClickHandler(lv_event_t* e) {
    DEBUG_PRINTLN("Delete button clicked");
    
    // Show confirmation dialog
    _confirmDelete();
}

void LogDetailScreen::_syncButtonClickHandler(lv_event_t* e) {
    DEBUG_PRINTLN("Sync button clicked");
    
    // Show loading
    UIManager::showLoading("Syncing log...");
    
    // Start sync process
    // This would typically be done asynchronously
    // For now, we'll simulate it with a delay
    
    // In a real implementation, this would be:
    // SyncManager::syncLog(_logIndex, [](bool success) {
    //     UIManager::hideLoading();
    //     if (success) {
    //         UIManager::showAlert("Sync Complete", "Log has been synchronized.", "OK");
    //     } else {
    //         UIManager::showAlert("Sync Failed", "Failed to synchronize log. Please try again.", "OK");
    //     }
    // });
    
    // Simulate sync process
    delay(1500);
    
    // Hide loading
    UIManager::hideLoading();
    
    // Show success message
    UIManager::showAlert("Sync Complete", "Log has been synchronized.", "OK");
}

void LogDetailScreen::_loadLogDetails(lv_obj_t* container) {
    DEBUG_PRINTF("Loading log details for index %d\n", _logIndex);
    
    // In a real implementation, this would load log details from the database
    // For now, we'll create a sample log
    
    // Sample log data
    const char* title = "Log #1";
    const char* gender = "Male";
    const char* shirtColor = "Red";
    const char* pantsColor = "Blue";
    const char* item = "Shirt";
    const char* timestamp = "2025-03-17 12:30";
    const char* location = "Store #123";
    const char* notes = "Customer was observed concealing merchandise in a shopping bag.";
    bool synced = false;
    
    // Create title
    lv_obj_t* titleLabel = lv_label_create(container);
    lv_obj_set_style_text_font(titleLabel, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(titleLabel, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(titleLabel, title);
    lv_obj_align(titleLabel, LV_ALIGN_TOP_MID, 0, 0);
    
    // Create timestamp
    lv_obj_t* timeLabel = lv_label_create(container);
    lv_obj_set_style_text_font(timeLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(timeLabel, lv_color_hex(0xCCCCCC), 0);
    lv_label_set_text(timeLabel, timestamp);
    lv_obj_align(timeLabel, LV_ALIGN_TOP_MID, 0, 30);
    
    // Create sync status
    lv_obj_t* syncLabel = lv_label_create(container);
    lv_obj_set_style_text_font(syncLabel, &lv_font_montserrat_14, 0);
    
    if (synced) {
        lv_obj_set_style_text_color(syncLabel, lv_color_hex(0x00CC00), 0);
        lv_label_set_text(syncLabel, "Synced " LV_SYMBOL_OK);
    } else {
        lv_obj_set_style_text_color(syncLabel, lv_color_hex(0xFFCC00), 0);
        lv_label_set_text(syncLabel, "Not Synced " LV_SYMBOL_WARNING);
    }
    
    lv_obj_align(syncLabel, LV_ALIGN_TOP_MID, 0, 55);
    
    // Create details grid
    lv_obj_t* grid = lv_obj_create(container);
    lv_obj_set_size(grid, lv_pct(100), 200);
    lv_obj_align(grid, LV_ALIGN_TOP_MID, 0, 90);
    lv_obj_set_style_bg_opa(grid, LV_OPA_0, 0);
    lv_obj_set_style_border_width(grid, 0, 0);
    lv_obj_set_style_pad_all(grid, 0, 0);
    lv_obj_set_flex_flow(grid, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(grid, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    
    // Create detail rows
    const char* labels[] = {"Gender:", "Shirt Color:", "Pants Color:", "Item:", "Location:"};
    const char* values[] = {gender, shirtColor, pantsColor, item, location};
    
    for (int i = 0; i < 5; i++) {
        // Create row container
        lv_obj_t* row = lv_obj_create(grid);
        lv_obj_set_size(row, lv_pct(100), 30);
        lv_obj_set_style_bg_opa(row, LV_OPA_0, 0);
        lv_obj_set_style_border_width(row, 0, 0);
        lv_obj_set_style_pad_all(row, 0, 0);
        lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(row, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        
        // Create label
        lv_obj_t* label = lv_label_create(row);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_16, 0);
        lv_obj_set_style_text_color(label, lv_color_hex(0xAAAAAA), 0);
        lv_label_set_text(label, labels[i]);
        lv_obj_set_width(label, 120);
        
        // Create value
        lv_obj_t* value = lv_label_create(row);
        lv_obj_set_style_text_font(value, &lv_font_montserrat_16, 0);
        lv_obj_set_style_text_color(value, lv_color_hex(0xFFFFFF), 0);
        lv_label_set_text(value, values[i]);
    }
    
    // Create notes section
    lv_obj_t* notesLabel = lv_label_create(container);
    lv_obj_set_style_text_font(notesLabel, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(notesLabel, lv_color_hex(0xAAAAAA), 0);
    lv_label_set_text(notesLabel, "Notes:");
    lv_obj_align(notesLabel, LV_ALIGN_TOP_LEFT, 0, 300);
    
    lv_obj_t* notesValue = lv_label_create(container);
    lv_obj_set_style_text_font(notesValue, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(notesValue, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_long_mode(notesValue, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(notesValue, lv_pct(100));
    lv_label_set_text(notesValue, notes);
    lv_obj_align(notesValue, LV_ALIGN_TOP_LEFT, 0, 330);
}

void LogDetailScreen::_confirmDelete() {
    UIManager::showConfirm(
        "Delete Log",
        "Are you sure you want to delete this log? This action cannot be undone.",
        "Delete",
        "Cancel",
        [](lv_event_t* e) {
            // Delete confirmed
            DEBUG_PRINTLN("Delete confirmed");
            
            // In a real implementation, this would delete the log from the database
            // For now, just show a message and go back to logs screen
            
            // Show loading
            UIManager::hideLoading();
            UIManager::showLoading("Deleting log...");
            
            // Simulate delete process
            delay(1000);
            
            // Hide loading
            UIManager::hideLoading();
            
            // Show success message
            UIManager::showAlert("Log Deleted", "The log has been deleted successfully.", "OK", 
                [](lv_event_t* e) {
                    // Navigate back to logs screen
                    UIManager::setScreen(SCREEN_LOGS);
                }
            );
        }
    );
}
