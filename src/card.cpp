/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Card Component Implementation
 * 
 * Compatible with LVGL 8.4.0
 */

#include "card.h"

// Static member initialization
lv_style_t Card::_styleCard;
lv_style_t Card::_styleCardPressed;
lv_style_t Card::_styleTitle;
lv_style_t Card::_styleContent;
lv_style_t Card::_styleBtn;
lv_style_t Card::_styleBtnPressed;
bool Card::_stylesInitialized = false;

void Card::_initStyles() {
    if (_stylesInitialized) {
        return;
    }
    
    // Card style
    lv_style_init(&_styleCard);
    lv_style_set_bg_color(&_styleCard, lv_color_hex(0x303030)); // Dark background
    lv_style_set_bg_opa(&_styleCard, LV_OPA_COVER);
    lv_style_set_border_color(&_styleCard, lv_color_hex(0x404040)); // Border color
    lv_style_set_border_width(&_styleCard, 1);
    lv_style_set_border_opa(&_styleCard, LV_OPA_50);
    lv_style_set_radius(&_styleCard, 5);
    lv_style_set_shadow_width(&_styleCard, 10);
    lv_style_set_shadow_ofs_y(&_styleCard, 5);
    lv_style_set_shadow_opa(&_styleCard, LV_OPA_20);
    lv_style_set_pad_all(&_styleCard, 10);
    
    // Card pressed style
    lv_style_init(&_styleCardPressed);
    lv_style_set_bg_color(&_styleCardPressed, lv_color_hex(0x505050)); // Lighter background when pressed
    lv_style_set_transform_width(&_styleCardPressed, -5);
    lv_style_set_transform_height(&_styleCardPressed, -5);
    lv_style_set_shadow_ofs_y(&_styleCardPressed, 2);
    lv_style_set_shadow_opa(&_styleCardPressed, LV_OPA_10);
    
    // Title style
    lv_style_init(&_styleTitle);
    lv_style_set_text_color(&_styleTitle, lv_color_hex(0xFFFFFF)); // White text
    lv_style_set_text_font(&_styleTitle, &lv_font_montserrat_16); // Larger font for titles
    
    // Content style
    lv_style_init(&_styleContent);
    lv_style_set_text_color(&_styleContent, lv_color_hex(0xCCCCCC)); // Light gray text
    lv_style_set_text_font(&_styleContent, &lv_font_montserrat_14); // Normal font for content
    
    // Button style
    lv_style_init(&_styleBtn);
    lv_style_set_bg_color(&_styleBtn, lv_color_hex(0x0078D7)); // Blue button
    lv_style_set_bg_opa(&_styleBtn, LV_OPA_COVER);
    lv_style_set_border_width(&_styleBtn, 0);
    lv_style_set_radius(&_styleBtn, 5);
    lv_style_set_shadow_width(&_styleBtn, 5);
    lv_style_set_shadow_ofs_y(&_styleBtn, 3);
    lv_style_set_shadow_opa(&_styleBtn, LV_OPA_30);
    lv_style_set_pad_all(&_styleBtn, 10);
    lv_style_set_text_color(&_styleBtn, lv_color_hex(0xFFFFFF)); // White text
    
    // Button pressed style
    lv_style_init(&_styleBtnPressed);
    lv_style_set_bg_color(&_styleBtnPressed, lv_color_hex(0x0063B1)); // Darker blue when pressed
    lv_style_set_transform_width(&_styleBtnPressed, -2);
    lv_style_set_transform_height(&_styleBtnPressed, -2);
    lv_style_set_shadow_ofs_y(&_styleBtnPressed, 1);
    lv_style_set_shadow_opa(&_styleBtnPressed, LV_OPA_20);
    
    _stylesInitialized = true;
}

lv_obj_t* Card::create(lv_obj_t* parent, int width, int height, 
                     const char* title, const char* icon) {
    _initStyles();
    
    // Create card container
    lv_obj_t* card = lv_obj_create(parent);
    lv_obj_set_size(card, width, height);
    lv_obj_add_style(card, &_styleCard, 0);
    lv_obj_clear_flag(card, LV_OBJ_FLAG_SCROLLABLE);
    
    // Store content area reference for later use
    lv_obj_t* contentArea = lv_obj_create(card);
    lv_obj_set_size(contentArea, width - 20, height - 50); // Adjust for padding and title
    lv_obj_align(contentArea, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_set_style_bg_opa(contentArea, LV_OPA_0, 0); // Transparent background
    lv_obj_set_style_border_width(contentArea, 0, 0); // No border
    lv_obj_set_style_pad_all(contentArea, 0, 0); // No padding
    lv_obj_set_user_data(contentArea, (void*)"content_area"); // Tag for identification
    
    // Set card title if provided
    if (title) {
        setTitle(card, title);
    }
    
    // Set card icon if provided
    if (icon) {
        setIcon(card, icon);
    }
    
    return card;
}

void Card::setTitle(lv_obj_t* card, const char* title) {
    if (!card || !title) {
        return;
    }
    
    // Find existing title label
    lv_obj_t* titleLabel = nullptr;
    lv_obj_t* child = lv_obj_get_child(card, 0);
    while (child) {
        if (lv_obj_get_user_data(child) == (void*)"title") {
            titleLabel = child;
            break;
        }
        child = lv_obj_get_child(card, lv_obj_get_index(child) + 1);
    }
    
    // Create title label if it doesn't exist
    if (!titleLabel) {
        titleLabel = lv_label_create(card);
        lv_obj_add_style(titleLabel, &_styleTitle, 0);
        lv_obj_align(titleLabel, LV_ALIGN_TOP_MID, 0, 5);
        lv_obj_set_user_data(titleLabel, (void*)"title"); // Tag for identification
    }
    
    // Set title text
    lv_label_set_text(titleLabel, title);
}

void Card::setIcon(lv_obj_t* card, const char* icon) {
    if (!card || !icon) {
        return;
    }
    
    // Find existing icon label
    lv_obj_t* iconLabel = nullptr;
    lv_obj_t* child = lv_obj_get_child(card, 0);
    while (child) {
        if (lv_obj_get_user_data(child) == (void*)"icon") {
            iconLabel = child;
            break;
        }
        child = lv_obj_get_child(card, lv_obj_get_index(child) + 1);
    }
    
    // Create icon label if it doesn't exist
    if (!iconLabel) {
        iconLabel = lv_label_create(card);
        lv_obj_add_style(iconLabel, &_styleTitle, 0);
        lv_obj_align(iconLabel, LV_ALIGN_TOP_LEFT, 10, 5);
        lv_obj_set_user_data(iconLabel, (void*)"icon"); // Tag for identification
    }
    
    // Set icon text
    lv_label_set_text(iconLabel, icon);
}

void Card::setContent(lv_obj_t* card, const char* content) {
    if (!card || !content) {
        return;
    }
    
    // Get content area
    lv_obj_t* contentArea = getContentArea(card);
    if (!contentArea) {
        return;
    }
    
    // Find existing content label
    lv_obj_t* contentLabel = nullptr;
    lv_obj_t* child = lv_obj_get_child(contentArea, 0);
    while (child) {
        if (lv_obj_get_user_data(child) == (void*)"content") {
            contentLabel = child;
            break;
        }
        child = lv_obj_get_child(contentArea, lv_obj_get_index(child) + 1);
    }
    
    // Create content label if it doesn't exist
    if (!contentLabel) {
        contentLabel = lv_label_create(contentArea);
        lv_obj_add_style(contentLabel, &_styleContent, 0);
        lv_obj_align(contentLabel, LV_ALIGN_TOP_LEFT, 0, 0);
        lv_obj_set_user_data(contentLabel, (void*)"content"); // Tag for identification
        lv_label_set_long_mode(contentLabel, LV_LABEL_LONG_WRAP);
        lv_obj_set_width(contentLabel, lv_obj_get_width(contentArea));
    }
    
    // Set content text
    lv_label_set_text(contentLabel, content);
}

lv_obj_t* Card::addButton(lv_obj_t* card, const char* text, 
                        lv_event_cb_t callback, void* userData) {
    if (!card || !text) {
        return nullptr;
    }
    
    // Get content area
    lv_obj_t* contentArea = getContentArea(card);
    if (!contentArea) {
        return nullptr;
    }
    
    // Create button
    lv_obj_t* btn = lv_btn_create(contentArea);
    lv_obj_add_style(btn, &_styleBtn, 0);
    lv_obj_add_style(btn, &_styleBtnPressed, LV_STATE_PRESSED);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, 0);
    
    // Create button label
    lv_obj_t* label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_center(label);
    
    // Add click event
    if (callback) {
        lv_obj_add_event_cb(btn, callback, LV_EVENT_CLICKED, userData);
    }
    
    return btn;
}

lv_obj_t* Card::addImage(lv_obj_t* card, const void* src) {
    if (!card || !src) {
        return nullptr;
    }
    
    // Get content area
    lv_obj_t* contentArea = getContentArea(card);
    if (!contentArea) {
        return nullptr;
    }
    
    // Create image
    lv_obj_t* img = lv_img_create(contentArea);
    lv_img_set_src(img, src);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
    
    return img;
}

lv_obj_t* Card::getContentArea(lv_obj_t* card) {
    if (!card) {
        return nullptr;
    }
    
    // Find content area
    lv_obj_t* contentArea = nullptr;
    lv_obj_t* child = lv_obj_get_child(card, 0);
    while (child) {
        if (lv_obj_get_user_data(child) == (void*)"content_area") {
            contentArea = child;
            break;
        }
        child = lv_obj_get_child(card, lv_obj_get_index(child) + 1);
    }
    
    return contentArea;
}

void Card::setClickable(lv_obj_t* card, bool clickable, 
                      lv_event_cb_t callback, void* userData) {
    if (!card) {
        return;
    }
    
    if (clickable) {
        lv_obj_add_style(card, &_styleCardPressed, LV_STATE_PRESSED);
        
        if (callback) {
            lv_obj_add_event_cb(card, callback, LV_EVENT_CLICKED, userData);
        }
        
        lv_obj_add_flag(card, LV_OBJ_FLAG_CLICKABLE);
    } else {
        lv_obj_clear_flag(card, LV_OBJ_FLAG_CLICKABLE);
    }
}
