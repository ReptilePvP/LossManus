/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - WiFi Screen Implementation
 * 
 * Compatible with LVGL 8.4.0
 */

#include "wifi_screen.h"
#include "../ui_manager.h"
#include "../components/status_bar.h"
#include "../../connectivity/wifi_manager.h"
#include "../../app/app_controller.h"

// Static member initialization
lv_obj_t* WiFiScreen::_networksList = nullptr;
lv_obj_t* WiFiScreen::_statusLabel = nullptr;

lv_obj_t* WiFiScreen::create() {
    DEBUG_PRINTLN("Creating WiFi screen...");
    
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
    lv_label_set_text(title, "WiFi Settings");
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
    
    // Create status label
    _statusLabel = lv_label_create(screen);
    lv_obj_set_style_text_font(_statusLabel, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(_statusLabel, lv_color_hex(0xCCCCCC), 0);
    lv_label_set_text(_statusLabel, "Not connected");
    lv_obj_align(_statusLabel, LV_ALIGN_TOP_MID, 0, 80);
    
    // Create scan button
    lv_obj_t* scanBtn = lv_btn_create(screen);
    lv_obj_set_size(scanBtn, 120, 40);
    lv_obj_align(scanBtn, LV_ALIGN_TOP_RIGHT, -10, 80);
    lv_obj_set_style_bg_color(scanBtn, lv_color_hex(0x0078D7), 0);
    lv_obj_add_event_cb(scanBtn, _scanButtonClickHandler, LV_EVENT_CLICKED, nullptr);
    
    // Create scan button label
    lv_obj_t* scanLabel = lv_label_create(scanBtn);
    lv_label_set_text(scanLabel, LV_SYMBOL_REFRESH " Scan");
    lv_obj_center(scanLabel);
    
    // Create networks list
    _networksList = lv_list_create(screen);
    lv_obj_set_size(_networksList, DISPLAY_WIDTH - 20, DISPLAY_HEIGHT - 200);
    lv_obj_align(_networksList, LV_ALIGN_TOP_MID, 0, 130);
    lv_obj_set_style_bg_color(_networksList, lv_color_hex(0x303030), 0);
    lv_obj_set_style_border_width(_networksList, 0, 0);
    lv_obj_set_style_radius(_networksList, 10, 0);
    lv_obj_set_style_pad_all(_networksList, 10, 0);
    
    // Create disconnect button
    lv_obj_t* disconnectBtn = lv_btn_create(screen);
    lv_obj_set_size(disconnectBtn, 200, 50);
    lv_obj_align(disconnectBtn, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_set_style_bg_color(disconnectBtn, lv_color_hex(0xE81123), 0);
    lv_obj_add_event_cb(disconnectBtn, _disconnectButtonClickHandler, LV_EVENT_CLICKED, nullptr);
    
    // Create disconnect button label
    lv_obj_t* disconnectLabel = lv_label_create(disconnectBtn);
    lv_label_set_text(disconnectLabel, LV_SYMBOL_CLOSE " Disconnect");
    lv_obj_center(disconnectLabel);
    
    // Update connection status
    _updateConnectionStatus();
    
    // Refresh networks list
    refreshNetworks();
    
    DEBUG_PRINTLN("WiFi screen created successfully!");
    return screen;
}

void WiFiScreen::update(lv_obj_t* screen) {
    // Update status bar
    StatusBar::update();
    
    // Update connection status
    _updateConnectionStatus();
}

void WiFiScreen::refreshNetworks() {
    DEBUG_PRINTLN("Refreshing WiFi networks...");
    
    if (!_networksList) {
        return;
    }
    
    // Clear list
    lv_obj_clean(_networksList);
    
    // Add scanning message
    lv_obj_t* scanningLabel = lv_label_create(_networksList);
    lv_obj_set_style_text_font(scanningLabel, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(scanningLabel, lv_color_hex(0xCCCCCC), 0);
    lv_label_set_text(scanningLabel, "Scanning for networks...");
    lv_obj_center(scanningLabel);
    
    // Start WiFi scan
    // In a real implementation, this would be:
    // WiFiManager::scanNetworks([](std::vector<WiFiNetwork> networks) {
    //     // Clear list
    //     lv_obj_clean(_networksList);
    //     
    //     // Add networks to list
    //     for (const auto& network : networks) {
    //         _addNetworkToList(network.ssid, network.rssi, network.secure);
    //     }
    // });
    
    // For now, simulate WiFi scan with a delay
    delay(1000);
    
    // Clear list
    lv_obj_clean(_networksList);
    
    // Add sample networks
    const char* networks[] = {"Home Network", "Office WiFi", "Guest Network", "IoT Network", "Neighbor's WiFi"};
    const int rssi[] = {-45, -60, -70, -55, -80};
    const bool secure[] = {true, true, false, true, true};
    
    for (int i = 0; i < 5; i++) {
        // Create list button
        lv_obj_t* btn = lv_list_add_btn(_networksList, nullptr, "");
        lv_obj_set_style_bg_color(btn, lv_color_hex(0x404040), 0);
        lv_obj_set_style_bg_color(btn, lv_color_hex(0x505050), LV_STATE_PRESSED);
        lv_obj_set_style_bg_color(btn, lv_color_hex(0x606060), LV_STATE_FOCUSED);
        lv_obj_set_style_pad_all(btn, 10, 0);
        lv_obj_set_height(btn, 60);
        lv_obj_add_event_cb(btn, _networkItemClickHandler, LV_EVENT_CLICKED, nullptr);
        
        // Store SSID as user data
        lv_obj_set_user_data(btn, (void*)networks[i]);
        
        // Create container for content
        lv_obj_t* container = lv_obj_create(btn);
        lv_obj_set_size(container, lv_pct(100), lv_pct(100));
        lv_obj_set_style_bg_opa(container, LV_OPA_0, 0);
        lv_obj_set_style_border_width(container, 0, 0);
        lv_obj_set_style_pad_all(container, 0, 0);
        
        // Create SSID label
        lv_obj_t* ssidLabel = lv_label_create(container);
        lv_obj_set_style_text_font(ssidLabel, &lv_font_montserrat_16, 0);
        lv_obj_set_style_text_color(ssidLabel, lv_color_hex(0xFFFFFF), 0);
        lv_label_set_text(ssidLabel, networks[i]);
        lv_obj_align(ssidLabel, LV_ALIGN_TOP_LEFT, 0, 0);
        
        // Create signal strength label
        lv_obj_t* rssiLabel = lv_label_create(container);
        lv_obj_set_style_text_font(rssiLabel, &lv_font_montserrat_14, 0);
        lv_obj_set_style_text_color(rssiLabel, lv_color_hex(0xCCCCCC), 0);
        
        // Convert RSSI to signal strength
        const char* signalStrength;
        if (rssi[i] > -50) {
            signalStrength = "Excellent";
        } else if (rssi[i] > -60) {
            signalStrength = "Good";
        } else if (rssi[i] > -70) {
            signalStrength = "Fair";
        } else {
            signalStrength = "Poor";
        }
        
        char rssiText[32];
        snprintf(rssiText, sizeof(rssiText), "Signal: %s (%d dBm)", signalStrength, rssi[i]);
        lv_label_set_text(rssiLabel, rssiText);
        lv_obj_align(rssiLabel, LV_ALIGN_BOTTOM_LEFT, 0, 0);
        
        // Create security icon
        lv_obj_t* securityIcon = lv_label_create(container);
        lv_obj_set_style_text_font(securityIcon, &lv_font_montserrat_16, 0);
        
        if (secure[i]) {
            lv_obj_set_style_text_color(securityIcon, lv_color_hex(0x00CC00), 0);
            lv_label_set_text(securityIcon, LV_SYMBOL_LOCK);
        } else {
            lv_obj_set_style_text_color(securityIcon, lv_color_hex(0xFFCC00), 0);
            lv_label_set_text(securityIcon, LV_SYMBOL_WARNING);
        }
        
        lv_obj_align(securityIcon, LV_ALIGN_TOP_RIGHT, 0, 0);
    }
}

void WiFiScreen::_backButtonClickHandler(lv_event_t* e) {
    DEBUG_PRINTLN("Back button clicked");
    UIManager::setScreen(SCREEN_SETTINGS);
}

void WiFiScreen::_scanButtonClickHandler(lv_event_t* e) {
    DEBUG_PRINTLN("Scan button clicked");
    refreshNetworks();
}

void WiFiScreen::_networkItemClickHandler(lv_event_t* e) {
    lv_obj_t* btn = lv_event_get_target(e);
    const char* ssid = (const char*)lv_obj_get_user_data(btn);
    DEBUG_PRINTF("Network item clicked: %s\n", ssid);
    
    // Show password dialog
    _showPasswordDialog(ssid);
}

void WiFiScreen::_disconnectButtonClickHandler(lv_event_t* e) {
    DEBUG_PRINTLN("Disconnect button clicked");
    
    // Show loading
    UIManager::showLoading("Disconnecting...");
    
    // Disconnect from WiFi
    // In a real implementation, this would be:
    // WiFiManager::disconnect([](bool success) {
    //     UIManager::hideLoading();
    //     _updateConnectionStatus();
    // });
    
    // Simulate disconnect process
    delay(1000);
    
    // Hide loading
    UIManager::hideLoading();
    
    // Update connection status
    _updateConnectionStatus();
}

void WiFiScreen::_showPasswordDialog(const char* ssid) {
    // Create a custom dialog for password input
    lv_obj_t* dialog = lv_obj_create(lv_layer_top());
    lv_obj_set_size(dialog, DISPLAY_WIDTH - 40, 220);
    lv_obj_align(dialog, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(dialog, lv_color_hex(0x303030), 0);
    lv_obj_set_style_border_width(dialog, 1, 0);
    lv_obj_set_style_border_color(dialog, lv_color_hex(0x505050), 0);
    lv_obj_set_style_shadow_width(dialog, 20, 0);
    lv_obj_set_style_shadow_ofs_y(dialog, 5, 0);
    lv_obj_set_style_shadow_opa(dialog, LV_OPA_30, 0);
    lv_obj_set_style_pad_all(dialog, 15, 0);
    
    // Create title
    lv_obj_t* title = lv_label_create(dialog);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    
    char titleText[64];
    snprintf(titleText, sizeof(titleText), "Connect to %s", ssid);
    lv_label_set_text(title, titleText);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 0);
    
    // Create password label
    lv_obj_t* passLabel = lv_label_create(dialog);
    lv_obj_set_style_text_font(passLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(passLabel, lv_color_hex(0xCCCCCC), 0);
    lv_label_set_text(passLabel, "Password:");
    lv_obj_align(passLabel, LV_ALIGN_TOP_LEFT, 0, 40);
    
    // Create password text area
    lv_obj_t* passTextArea = lv_textarea_create(dialog);
    lv_obj_set_size(passTextArea, lv_pct(100), 40);
    lv_obj_align(passTextArea, LV_ALIGN_TOP_MID, 0, 70);
    lv_obj_set_style_bg_color(passTextArea, lv_color_hex(0x404040), 0);
    lv_textarea_set_password_mode(passTextArea, true);
    lv_textarea_set_placeholder_text(passTextArea, "Enter password");
    
    // Store SSID as user data
    lv_obj_set_user_data(passTextArea, (void*)ssid);
    
    // Create button container
    lv_obj_t* btnContainer = lv_obj_create(dialog);
    lv_obj_set_size(btnContainer, lv_pct(100), 50);
    lv_obj_align(btnContainer, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_set_style_bg_opa(btnContainer, LV_OPA_0, 0);
    lv_obj_set_style_border_width(btnContainer, 0, 0);
    lv_obj_set_style_pad_all(btnContainer, 0, 0);
    lv_obj_set_flex_flow(btnContainer, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(btnContainer, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    // Create connect button
    lv_obj_t* connectBtn = lv_btn_create(btnContainer);
    lv_obj_set_size(connectBtn, 120, 40);
    lv_obj_set_style_bg_color(connectBtn, lv_color_hex(0x0078D7), 0);
    
    // Add click event
    lv_obj_add_event_cb(connectBtn, [](lv_event_t* e) {
        lv_obj_t* btn = lv_event_get_target(e);
        lv_obj_t* dialog = lv_obj_get_parent(lv_obj_get_parent(btn));
        
        // Find password text area
        lv_obj_t* passTextArea = nullptr;
        for (int i = 0; i < lv_obj_get_child_cnt(dialog); i++) {
            lv_obj_t* child = lv_obj_get_child(dialog, i);
            if (lv_obj_check_type(child, &lv_textarea_class)) {
                passTextArea = child;
                break;
            }
        }
        
        if (passTextArea) {
            const char* ssid = (const char*)lv_obj_get_user_data(passTextArea);
            const char* password = lv_textarea_get_text(passTextArea);
            
            // Close dialog
            lv_obj_del(dialog);
            
            // Connect to network
            WiFiScreen::_connectToNetwork(ssid, password);
        }
    }, LV_EVENT_CLICKED, nullptr);
    
    // Create connect button label
    lv_obj_t* connectLabel = lv_label_create(connectBtn);
    lv_label_set_text(connectLabel, "Connect");
    lv_obj_center(connectLabel);
    
    // Create cancel button
    lv_obj_t* cancelBtn = lv_btn_create(btnContainer);
    lv_obj_set_size(cancelBtn, 120, 40);
    lv_obj_set_style_bg_color(cancelBtn, lv_color_hex(0x505050), 0);
    
    // Add click event
    lv_obj_add_event_cb(cancelBtn, [](lv_event_t* e) {
        lv_obj_t* btn = lv_event_get_target(e);
        lv_obj_t* dialog = lv_obj_get_parent(lv_obj_get_parent(btn));
        lv_obj_del(dialog);
    }, LV_EVENT_CLICKED, nullptr);
    
    // Create cancel button label
    lv_obj_t* cancelLabel = lv_label_create(cancelBtn);
    lv_label_set_text(cancelLabel, "Cancel");
    lv_obj_center(cancelLabel);
    
    // Show keyboard
    UIManager::showKeyboard(passTextArea);
}

void WiFiScreen::_connectToNetwork(const char* ssid, const char* password) {
    DEBUG_PRINTF("Connecting to network: %s\n", ssid);
    
    // Show loading
    UIManager::showLoading("Connecting...");
    
    // Connect to WiFi
    // In a real implementation, this would be:
    // WiFiManager::connect(ssid, password, [](bool success) {
    //     UIManager::hideLoading();
    //     
    //     if (success) {
    //         UIManager::showAlert("Connected", "Successfully connected to WiFi network.", "OK");
    //     } else {
    //         UIManager::showAlert("Connection Failed", "Failed to connect to WiFi network. Please check your password and try again.", "OK");
    //     }
    //     
    //     _updateConnectionStatus();
    // });
    
    // Simulate connection process
    delay(2000);
    
    // Hide loading
    UIManager::hideLoading();
    
    // Show success message
    UIManager::showAlert("Connected", "Successfully connected to WiFi network.", "OK");
    
    // Update connection status
    _updateConnectionStatus();
}

void WiFiScreen::_updateConnectionStatus() {
    if (!_statusLabel) {
        return;
    }
    
    // In a real implementation, this would check the actual WiFi status
    // For now, simulate a connected state
    bool connected = true;
    const char* ssid = "Home Network";
    
    if (connected) {
        char statusText[64];
        snprintf(statusText, sizeof(statusText), "Connected to %s", ssid);
        lv_label_set_text(_statusLabel, statusText);
        lv_obj_set_style_text_color(_statusLabel, lv_color_hex(0x00CC00), 0);
    } else {
        lv_label_set_text(_statusLabel, "Not connected");
        lv_obj_set_style_text_color(_statusLabel, lv_color_hex(0xCCCCCC), 0);
    }
}
