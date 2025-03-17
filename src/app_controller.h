/**
 * Enhanced Loss Prevention Log
 * Application Layer - App Controller
 * 
 * This file contains the main application controller
 */

#ifndef APP_APP_CONTROLLER_H
#define APP_APP_CONTROLLER_H

#include <Arduino.h>
#include "../config.h"
#include "../hal/display.h"
#include "../hal/storage.h"
#include "../hal/rtc.h"
#include "../hal/power.h"
#include "../hal/touch.h"
#include "../hal/wifi_hardware.h"
#include "../data/database.h"
#include "../data/sync.h"
#include "../data/export.h"
#include "../data/search.h"
#include "../connectivity/wifi_manager.h"
#include "../connectivity/webhook.h"
#include "../connectivity/api_client.h"
#include "../connectivity/offline_queue.h"
#include "../ui/ui_manager.h"
#include "../ui/screens/main_menu_screen.h"
#include "../ui/screens/gender_screen.h"
#include "../ui/screens/color_screen.h"
#include "../ui/screens/item_screen.h"
#include "../ui/screens/item_details_screen.h"
#include "../ui/screens/confirm_screen.h"

class AppController {
public:
    /**
     * Initialize the application
     * @return true if successful, false otherwise
     */
    static bool init();
    
    /**
     * Run the application main loop
     */
    static void run();
    
    /**
     * Handle screen change
     * @param screenType screen type to change to
     */
    static void handleScreenChange(ScreenType screenType);
    
    /**
     * Get current log entry
     * @return current log entry
     */
    static LogEntry getCurrentEntry();
    
    /**
     * Set current log entry
     * @param entry log entry to set
     */
    static void setCurrentEntry(const LogEntry& entry);
    
    /**
     * Reset current log entry
     */
    static void resetCurrentEntry();

private:
    static bool _initialized;
    static LogEntry _currentEntry;
    static ScreenType _currentScreen;
    static uint32_t _lastUpdateTime;
    static uint32_t _lastSyncTime;
    static uint32_t _lastAutoSaveTime;
    static uint32_t _lastPowerCheckTime;
    
    // Initialize hardware
    static bool _initHardware();
    
    // Initialize data layer
    static bool _initData();
    
    // Initialize connectivity
    static bool _initConnectivity();
    
    // Initialize UI
    static bool _initUI();
    
    // Periodic tasks
    static void _runPeriodicTasks();
    
    // Power management
    static void _checkPower();
    
    // Auto save entry
    static void _autoSaveEntry();
    
    // Sync data
    static void _syncData();
};

#endif // APP_APP_CONTROLLER_H
