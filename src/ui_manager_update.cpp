#include "ui_manager.h"
#include "screens/logs_screen.h"
#include "screens/log_detail_screen.h"
#include "screens/settings_screen.h"
#include "screens/wifi_screen.h"
#include "screens/about_screen.h"
#include "screens/search_screen.h"

// Update the UI manager to include the new screens
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
        case SCREEN_LOGS:
            screen = LogsScreen::create();
            break;
        case SCREEN_LOG_DETAIL:
            screen = LogDetailScreen::create();
            break;
        case SCREEN_SETTINGS:
            screen = SettingsScreen::create();
            break;
        case SCREEN_WIFI:
            screen = WiFiScreen::create();
            break;
        case SCREEN_ABOUT:
            screen = AboutScreen::create();
            break;
        case SCREEN_SEARCH:
            screen = SearchScreen::create();
            break;
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
