/**
 * Enhanced Loss Prevention Log
 * Application Layer - App Controller Implementation
 * 
 * This file implements the main application controller
 */

#include "app_controller.h"

// Static member initialization
bool AppController::_initialized = false;
LogEntry AppController::_currentEntry;
ScreenType AppController::_currentScreen = SCREEN_MAIN_MENU;
uint32_t AppController::_lastUpdateTime = 0;
uint32_t AppController::_lastSyncTime = 0;
uint32_t AppController::_lastAutoSaveTime = 0;
uint32_t AppController::_lastPowerCheckTime = 0;

bool AppController::init() {
    if (_initialized) {
        return true;
    }
    
    DEBUG_PRINTLN("Initializing Loss Prevention Log application...");
    
    // Initialize hardware
    if (!_initHardware()) {
        DEBUG_PRINTLN("Failed to initialize hardware!");
        return false;
    }
    
    // Initialize data layer
    if (!_initData()) {
        DEBUG_PRINTLN("Failed to initialize data layer!");
        return false;
    }
    
    // Initialize connectivity
    if (!_initConnectivity()) {
        DEBUG_PRINTLN("Failed to initialize connectivity!");
        // Continue anyway, connectivity is not critical
    }
    
    // Initialize UI
    if (!_initUI()) {
        DEBUG_PRINTLN("Failed to initialize UI!");
        return false;
    }
    
    _initialized = true;
    DEBUG_PRINTLN("Application initialized successfully!");
    
    return true;
}

void AppController::run() {
    if (!_initialized) {
        if (!init()) {
            // Critical error, halt application
            while (true) {
                delay(1000);
            }
        }
    }
    
    // Run LVGL tasks
    lv_task_handler();
    
    // Run periodic tasks
    _runPeriodicTasks();
    
    // Small delay to prevent CPU hogging
    delay(5);
}

void AppController::handleScreenChange(ScreenType screenType) {
    _currentScreen = screenType;
    
    // Update current entry in the appropriate screen
    switch (screenType) {
        case SCREEN_GENDER:
            GenderScreen::setCurrentEntry(_currentEntry);
            break;
        case SCREEN_SHIRT_COLOR:
            ColorScreen::setCurrentEntry(_currentEntry);
            break;
        case SCREEN_PANTS_COLOR:
            ColorScreen::setCurrentEntry(_currentEntry);
            break;
        case SCREEN_SHOES_COLOR:
            ColorScreen::setCurrentEntry(_currentEntry);
            break;
        case SCREEN_ITEM_TYPE:
            ItemScreen::setCurrentEntry(_currentEntry);
            break;
        case SCREEN_ITEM_DETAILS:
            ItemDetailsScreen::setCurrentEntry(_currentEntry);
            break;
        case SCREEN_CONFIRM:
            ConfirmScreen::setCurrentEntry(_currentEntry);
            break;
        case SCREEN_MAIN_MENU:
            // Reset current entry when returning to main menu
            resetCurrentEntry();
            break;
        default:
            break;
    }
    
    // Get updated entry from the appropriate screen after navigation
    switch (screenType) {
        case SCREEN_GENDER:
            _currentEntry = GenderScreen::getCurrentEntry();
            break;
        case SCREEN_SHIRT_COLOR:
        case SCREEN_PANTS_COLOR:
        case SCREEN_SHOES_COLOR:
            _currentEntry = ColorScreen::getCurrentEntry();
            break;
        case SCREEN_ITEM_TYPE:
            _currentEntry = ItemScreen::getCurrentEntry();
            break;
        case SCREEN_ITEM_DETAILS:
            _currentEntry = ItemDetailsScreen::getCurrentEntry();
            break;
        case SCREEN_CONFIRM:
            _currentEntry = ConfirmScreen::getCurrentEntry();
            break;
        default:
            break;
    }
}

LogEntry AppController::getCurrentEntry() {
    return _currentEntry;
}

void AppController::setCurrentEntry(const LogEntry& entry) {
    _currentEntry = entry;
}

void AppController::resetCurrentEntry() {
    _currentEntry = LogEntry();
}

bool AppController::_initHardware() {
    DEBUG_PRINTLN("Initializing hardware...");
    
    // Initialize display
    if (!DisplayHAL::init()) {
        return false;
    }
    
    // Initialize storage
    if (!StorageHAL::init()) {
        return false;
    }
    
    // Initialize RTC
    if (!RtcHAL::init()) {
        return false;
    }
    
    // Initialize power management
    if (!PowerHAL::init()) {
        return false;
    }
    
    // Initialize touch
    if (!TouchHAL::init()) {
        return false;
    }
    
    // Initialize WiFi hardware
    WiFiHardware::init();
    
    return true;
}

bool AppController::_initData() {
    DEBUG_PRINTLN("Initializing data layer...");
    
    // Initialize database
    if (!Database::init()) {
        return false;
    }
    
    // Initialize search engine
    if (!SearchEngine::init()) {
        return false;
    }
    
    // Initialize export utilities
    if (!ExportManager::init()) {
        return false;
    }
    
    // Initialize sync manager
    if (!SyncManager::init()) {
        return false;
    }
    
    return true;
}

bool AppController::_initConnectivity() {
    DEBUG_PRINTLN("Initializing connectivity...");
    
    // Initialize WiFi manager
    if (!WiFiManager::init()) {
        return false;
    }
    
    // Initialize webhook client
    if (!WebhookClient::init()) {
        return false;
    }
    
    // Initialize API client
    if (!ApiClient::init()) {
        return false;
    }
    
    // Initialize offline queue
    if (!OfflineQueue::init()) {
        return false;
    }
    
    return true;
}

bool AppController::_initUI() {
    DEBUG_PRINTLN("Initializing UI...");
    
    // Initialize UI manager
    if (!UIManager::init()) {
        return false;
    }
    
    // Set initial screen
    UIManager::setScreen(SCREEN_MAIN_MENU);
    
    return true;
}

void AppController::_runPeriodicTasks() {
    uint32_t currentTime = millis();
    
    // Update UI every 50ms
    if (currentTime - _lastUpdateTime >= 50) {
        _lastUpdateTime = currentTime;
        
        // Update current screen
        UIManager::update();
    }
    
    // Check power status every 10 seconds
    if (currentTime - _lastPowerCheckTime >= 10000) {
        _lastPowerCheckTime = currentTime;
        _checkPower();
    }
    
    // Auto-save entry every 30 seconds if in entry flow
    if (currentTime - _lastAutoSaveTime >= 30000) {
        _lastAutoSaveTime = currentTime;
        _autoSaveEntry();
    }
    
    // Try to sync data every 5 minutes
    if (currentTime - _lastSyncTime >= 300000) {
        _lastSyncTime = currentTime;
        _syncData();
    }
}

void AppController::_checkPower() {
    // Check battery level
    int batteryLevel = PowerHAL::getBatteryLevel();
    
    // If battery level is critically low, show warning
    if (batteryLevel < 10 && !PowerHAL::isCharging()) {
        UIManager::showAlert("Low Battery Warning", 
                           "Battery level is critically low. Please connect charger soon.", 
                           "OK");
    }
}

void AppController::_autoSaveEntry() {
    // Only auto-save if in entry flow
    if (_currentScreen >= SCREEN_GENDER && _currentScreen <= SCREEN_CONFIRM) {
        // Save current entry state to temporary storage
        StorageHAL::saveTemporaryEntry(_currentEntry);
        DEBUG_PRINTLN("Auto-saved entry to temporary storage");
    }
}

void AppController::_syncData() {
    // Only sync if WiFi is connected
    if (WiFiManager::isConnected()) {
        // Process sync queue in background
        SyncManager::processSyncQueue();
    }
}
