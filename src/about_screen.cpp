/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - About Screen Implementation
 * 
 * Compatible with LVGL 8.4.0
 */

#include "about_screen.h"
#include "../ui_manager.h"
#include "../components/status_bar.h"
#include "../../version.h"

lv_obj_t* AboutScreen::create() {
    DEBUG_PRINTLN("Creating about screen...");
    
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
    lv_label_set_text(title, "About");
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
    lv_obj_set_size(container, DISPLAY_WIDTH - 40, DISPLAY_HEIGHT - 150);
    lv_obj_align(container, LV_ALIGN_TOP_MID, 0, 100);
    lv_obj_set_style_bg_color(container, lv_color_hex(0x303030), 0);
    lv_obj_set_style_border_width(container, 0, 0);
    lv_obj_set_style_radius(container, 10, 0);
    lv_obj_set_style_pad_all(container, 20, 0);
    
    // Create app name
    lv_obj_t* appName = lv_label_create(container);
    lv_obj_set_style_text_font(appName, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(appName, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(appName, "Loss Prevention Log");
    lv_obj_align(appName, LV_ALIGN_TOP_MID, 0, 0);
    
    // Create version
    lv_obj_t* version = lv_label_create(container);
    lv_obj_set_style_text_font(version, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(version, lv_color_hex(0xCCCCCC), 0);
    
    char versionText[32];
    snprintf(versionText, sizeof(versionText), "Version %s", APP_VERSION);
    lv_label_set_text(version, versionText);
    lv_obj_align(version, LV_ALIGN_TOP_MID, 0, 30);
    
    // Create description
    lv_obj_t* description = lv_label_create(container);
    lv_obj_set_style_text_font(description, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(description, lv_color_hex(0xAAAAAA), 0);
    lv_label_set_long_mode(description, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(description, lv_pct(100));
    lv_label_set_text(description, 
        "Enhanced Loss Prevention Log for M5Stack CoreS3\n\n"
        "This application helps retail loss prevention teams track and "
        "document potential theft incidents. It provides an easy-to-use "
        "interface for recording suspect descriptions, stolen items, "
        "and other relevant information."
    );
    lv_obj_align(description, LV_ALIGN_TOP_LEFT, 0, 70);
    
    // Create hardware info
    lv_obj_t* hardwareTitle = lv_label_create(container);
    lv_obj_set_style_text_font(hardwareTitle, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(hardwareTitle, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(hardwareTitle, "Hardware Information");
    lv_obj_align(hardwareTitle, LV_ALIGN_TOP_LEFT, 0, 170);
    
    lv_obj_t* hardwareInfo = lv_label_create(container);
    lv_obj_set_style_text_font(hardwareInfo, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(hardwareInfo, lv_color_hex(0xAAAAAA), 0);
    lv_label_set_long_mode(hardwareInfo, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(hardwareInfo, lv_pct(100));
    lv_label_set_text(hardwareInfo, 
        "Device: M5Stack CoreS3\n"
        "Display: 2.0\" IPS LCD (320x240)\n"
        "Processor: ESP32-S3\n"
        "Flash: 16MB\n"
        "RAM: 8MB PSRAM"
    );
    lv_obj_align(hardwareInfo, LV_ALIGN_TOP_LEFT, 0, 200);
    
    // Create credits
    lv_obj_t* creditsTitle = lv_label_create(container);
    lv_obj_set_style_text_font(creditsTitle, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(creditsTitle, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(creditsTitle, "Credits");
    lv_obj_align(creditsTitle, LV_ALIGN_TOP_LEFT, 0, 300);
    
    lv_obj_t* credits = lv_label_create(container);
    lv_obj_set_style_text_font(credits, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(credits, lv_color_hex(0xAAAAAA), 0);
    lv_label_set_long_mode(credits, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(credits, lv_pct(100));
    lv_label_set_text(credits, 
        "UI Framework: LVGL 8.4.0\n"
        "Hardware Framework: M5Unified\n"
        "Created by: Your Name\n"
        "© 2025 All Rights Reserved"
    );
    lv_obj_align(credits, LV_ALIGN_TOP_LEFT, 0, 330);
    
    DEBUG_PRINTLN("About screen created successfully!");
    return screen;
}

void AboutScreen::update(lv_obj_t* screen) {
    // Update status bar
    StatusBar::update();
    
    // Any other dynamic updates can be added here
}

void AboutScreen::_backButtonClickHandler(lv_event_t* e) {
    DEBUG_PRINTLN("Back button clicked");
    UIManager::setScreen(SCREEN_SETTINGS);
}
