/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Item Selector Component Implementation
 * 
 * Compatible with LVGL 8.4.0
 */

#include "item_selector.h"

// Static member initialization
lv_style_t ItemSelector::_styleItemBtn;
lv_style_t ItemSelector::_styleItemBtnSelected;
lv_style_t ItemSelector::_styleItemLabel;
bool ItemSelector::_stylesInitialized = false;

// Item type information
const ItemSelector::ItemTypeInfo ItemSelector::_itemTypes[] = {
    { ITEM_CLOTHING, "Clothing", LV_SYMBOL_IMAGE },
    { ITEM_ELECTRONICS, "Electronics", LV_SYMBOL_POWER },
    { ITEM_JEWELRY, "Jewelry", LV_SYMBOL_SETTINGS },
    { ITEM_COSMETICS, "Cosmetics", LV_SYMBOL_REFRESH },
    { ITEM_ACCESSORIES, "Accessories", LV_SYMBOL_PLUS },
    { ITEM_FOOD, "Food", LV_SYMBOL_LIST },
    { ITEM_TOYS, "Toys", LV_SYMBOL_PLAY },
    { ITEM_TOOLS, "Tools", LV_SYMBOL_SETTINGS },
    { ITEM_OTHER, "Other", LV_SYMBOL_CLOSE }
};
const int ItemSelector::_itemTypeCount = sizeof(ItemSelector::_itemTypes) / sizeof(ItemSelector::_itemTypes[0]);

void ItemSelector::_initStyles() {
    if (_stylesInitialized) {
        return;
    }
    
    // Item button style
    lv_style_init(&_styleItemBtn);
    lv_style_set_bg_color(&_styleItemBtn, lv_color_hex(0x303030)); // Dark background
    lv_style_set_bg_opa(&_styleItemBtn, LV_OPA_COVER);
    lv_style_set_border_width(&_styleItemBtn, 1);
    lv_style_set_border_color(&_styleItemBtn, lv_color_hex(0x505050));
    lv_style_set_radius(&_styleItemBtn, 5);
    lv_style_set_pad_all(&_styleItemBtn, 5);
    
    // Selected item button style
    lv_style_init(&_styleItemBtnSelected);
    lv_style_set_bg_color(&_styleItemBtnSelected, lv_color_hex(0x0078D7)); // Blue background when selected
    lv_style_set_border_width(&_styleItemBtnSelected, 2);
    lv_style_set_border_color(&_styleItemBtnSelected, lv_color_hex(0xFFFFFF));
    lv_style_set_shadow_width(&_styleItemBtnSelected, 10);
    lv_style_set_shadow_ofs_y(&_styleItemBtnSelected, 3);
    lv_style_set_shadow_opa(&_styleItemBtnSelected, LV_OPA_50);
    
    // Item label style
    lv_style_init(&_styleItemLabel);
    lv_style_set_text_color(&_styleItemLabel, lv_color_hex(0xFFFFFF)); // White text
    lv_style_set_text_font(&_styleItemLabel, &lv_font_montserrat_14);
    
    _stylesInitialized = true;
}

lv_obj_t* ItemSelector::create(lv_obj_t* parent, ItemSelectedCallback callback, uint8_t columns) {
    _initStyles();
    
    // Create item selector container
    lv_obj_t* itemSelector = lv_obj_create(parent);
    lv_obj_set_size(itemSelector, lv_pct(100), lv_pct(80));
    lv_obj_set_style_bg_opa(itemSelector, LV_OPA_0, 0);
    lv_obj_set_style_border_width(itemSelector, 0, 0);
    lv_obj_set_style_pad_all(itemSelector, 5, 0);
    lv_obj_set_flex_flow(itemSelector, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(itemSelector, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    
    // Store callback in user data
    lv_obj_set_user_data(itemSelector, (void*)callback);
    
    // Calculate button size based on columns
    int btnWidth = (lv_obj_get_width(itemSelector) - 20) / columns;
    int btnHeight = 60;
    
    // Create item buttons
    for (int i = 0; i < _itemTypeCount; i++) {
        // Create button
        lv_obj_t* btn = lv_btn_create(itemSelector);
        lv_obj_set_size(btn, btnWidth - 10, btnHeight);
        lv_obj_add_style(btn, &_styleItemBtn, 0);
        
        // Store item type in button user data
        lv_obj_set_user_data(btn, (void*)(intptr_t)_itemTypes[i].type);
        
        // Create button content container
        lv_obj_t* contentContainer = lv_obj_create(btn);
        lv_obj_set_size(contentContainer, lv_pct(100), lv_pct(100));
        lv_obj_set_style_bg_opa(contentContainer, LV_OPA_0, 0);
        lv_obj_set_style_border_width(contentContainer, 0, 0);
        lv_obj_set_style_pad_all(contentContainer, 0, 0);
        lv_obj_clear_flag(contentContainer, LV_OBJ_FLAG_SCROLLABLE);
        
        // Create icon label
        lv_obj_t* iconLabel = lv_label_create(contentContainer);
        lv_obj_add_style(iconLabel, &_styleItemLabel, 0);
        lv_label_set_text(iconLabel, _itemTypes[i].icon);
        lv_obj_align(iconLabel, LV_ALIGN_TOP_MID, 0, 5);
        
        // Create text label
        lv_obj_t* textLabel = lv_label_create(contentContainer);
        lv_obj_add_style(textLabel, &_styleItemLabel, 0);
        lv_label_set_text(textLabel, _itemTypes[i].name);
        lv_obj_align(textLabel, LV_ALIGN_BOTTOM_MID, 0, -5);
        
        // Add click event
        lv_obj_add_event_cb(btn, _itemButtonClickHandler, LV_EVENT_CLICKED, itemSelector);
    }
    
    // Add custom item button if needed
    addCustomItemButton(itemSelector);
    
    return itemSelector;
}

ItemType ItemSelector::getSelectedItemType(lv_obj_t* itemSelector) {
    if (!itemSelector) {
        return ITEM_UNKNOWN;
    }
    
    // Find selected button
    lv_obj_t* btn = lv_obj_get_child(itemSelector, 0);
    while (btn) {
        if (lv_obj_has_state(btn, LV_STATE_CHECKED)) {
            // Get item type from button user data
            return (ItemType)(intptr_t)lv_obj_get_user_data(btn);
        }
        btn = lv_obj_get_child(itemSelector, lv_obj_get_index(btn) + 1);
    }
    
    return ITEM_UNKNOWN;
}

bool ItemSelector::setSelectedItemType(lv_obj_t* itemSelector, ItemType itemType) {
    if (!itemSelector) {
        return false;
    }
    
    // Clear current selection
    lv_obj_t* btn = lv_obj_get_child(itemSelector, 0);
    while (btn) {
        lv_obj_clear_state(btn, LV_STATE_CHECKED);
        lv_obj_remove_style(btn, &_styleItemBtnSelected, LV_STATE_CHECKED);
        btn = lv_obj_get_child(itemSelector, lv_obj_get_index(btn) + 1);
    }
    
    // Find button with matching item type
    btn = lv_obj_get_child(itemSelector, 0);
    while (btn) {
        ItemType btnItemType = (ItemType)(intptr_t)lv_obj_get_user_data(btn);
        if (btnItemType == itemType) {
            // Select this button
            lv_obj_add_state(btn, LV_STATE_CHECKED);
            lv_obj_add_style(btn, &_styleItemBtnSelected, LV_STATE_CHECKED);
            return true;
        }
        btn = lv_obj_get_child(itemSelector, lv_obj_get_index(btn) + 1);
    }
    
    return false;
}

bool ItemSelector::addCustomItemButton(lv_obj_t* itemSelector) {
    if (!itemSelector) {
        return false;
    }
    
    // Calculate button size based on existing buttons
    lv_obj_t* firstBtn = lv_obj_get_child(itemSelector, 0);
    if (!firstBtn) {
        return false;
    }
    
    int btnWidth = lv_obj_get_width(firstBtn);
    int btnHeight = lv_obj_get_height(firstBtn);
    
    // Create custom item button
    lv_obj_t* btn = lv_btn_create(itemSelector);
    lv_obj_set_size(btn, btnWidth, btnHeight);
    lv_obj_add_style(btn, &_styleItemBtn, 0);
    
    // Store custom item type in button user data
    lv_obj_set_user_data(btn, (void*)(intptr_t)ITEM_CUSTOM);
    
    // Create button content container
    lv_obj_t* contentContainer = lv_obj_create(btn);
    lv_obj_set_size(contentContainer, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_opa(contentContainer, LV_OPA_0, 0);
    lv_obj_set_style_border_width(contentContainer, 0, 0);
    lv_obj_set_style_pad_all(contentContainer, 0, 0);
    lv_obj_clear_flag(contentContainer, LV_OBJ_FLAG_SCROLLABLE);
    
    // Create icon label
    lv_obj_t* iconLabel = lv_label_create(contentContainer);
    lv_obj_add_style(iconLabel, &_styleItemLabel, 0);
    lv_label_set_text(iconLabel, LV_SYMBOL_EDIT);
    lv_obj_align(iconLabel, LV_ALIGN_TOP_MID, 0, 5);
    
    // Create text label
    lv_obj_t* textLabel = lv_label_create(contentContainer);
    lv_obj_add_style(textLabel, &_styleItemLabel, 0);
    lv_label_set_text(textLabel, "Custom");
    lv_obj_align(textLabel, LV_ALIGN_BOTTOM_MID, 0, -5);
    
    // Add click event
    lv_obj_add_event_cb(btn, _customItemClickHandler, LV_EVENT_CLICKED, itemSelector);
    
    return true;
}

void ItemSelector::_itemButtonClickHandler(lv_event_t* e) {
    lv_obj_t* btn = lv_event_get_target(e);
    lv_obj_t* itemSelector = (lv_obj_t*)lv_event_get_user_data(e);
    
    if (!btn || !itemSelector) {
        return;
    }
    
    // Clear current selection
    lv_obj_t* otherBtn = lv_obj_get_child(itemSelector, 0);
    while (otherBtn) {
        if (otherBtn != btn) {
            lv_obj_clear_state(otherBtn, LV_STATE_CHECKED);
            lv_obj_remove_style(otherBtn, &_styleItemBtnSelected, LV_STATE_CHECKED);
        }
        otherBtn = lv_obj_get_child(itemSelector, lv_obj_get_index(otherBtn) + 1);
    }
    
    // Select this button
    lv_obj_add_state(btn, LV_STATE_CHECKED);
    lv_obj_add_style(btn, &_styleItemBtnSelected, LV_STATE_CHECKED);
    
    // Get item type from button user data
    ItemType itemType = (ItemType)(intptr_t)lv_obj_get_user_data(btn);
    
    // Call callback
    ItemSelectedCallback callback = (ItemSelectedCallback)lv_obj_get_user_data(itemSelector);
    if (callback) {
        callback(itemType);
    }
}

void ItemSelector::_customItemClickHandler(lv_event_t* e) {
    lv_obj_t* btn = lv_event_get_target(e);
    lv_obj_t* itemSelector = (lv_obj_t*)lv_event_get_user_data(e);
    
    if (!btn || !itemSelector) {
        return;
    }
    
    // Clear current selection
    lv_obj_t* otherBtn = lv_obj_get_child(itemSelector, 0);
    while (otherBtn) {
        if (otherBtn != btn) {
            lv_obj_clear_state(otherBtn, LV_STATE_CHECKED);
            lv_obj_remove_style(otherBtn, &_styleItemBtnSelected, LV_STATE_CHECKED);
        }
        otherBtn = lv_obj_get_child(itemSelector, lv_obj_get_index(otherBtn) + 1);
    }
    
    // Select this button
    lv_obj_add_state(btn, LV_STATE_CHECKED);
    lv_obj_add_style(btn, &_styleItemBtnSelected, LV_STATE_CHECKED);
    
    // Call callback with custom item type
    ItemSelectedCallback callback = (ItemSelectedCallback)lv_obj_get_user_data(itemSelector);
    if (callback) {
        callback(ITEM_CUSTOM);
    }
    
    // TODO: Implement custom item input dialog
}
