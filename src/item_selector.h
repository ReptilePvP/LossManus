/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Item Selector Component
 * 
 * This file contains the interface for the item selector component
 * Compatible with LVGL 8.4.0
 */

#ifndef UI_COMPONENTS_ITEM_SELECTOR_H
#define UI_COMPONENTS_ITEM_SELECTOR_H

#include <Arduino.h>
#include <lvgl.h>
#include <vector>
#include "../../config.h"
#include "../../data/log_entry.h"

// Item selection callback
typedef void (*ItemSelectedCallback)(ItemType itemType);

class ItemSelector {
public:
    /**
     * Create item selector grid
     * @param parent parent object
     * @param callback function to call when item is selected
     * @param columns number of columns in the grid
     * @return item selector object
     */
    static lv_obj_t* create(lv_obj_t* parent, ItemSelectedCallback callback, uint8_t columns = 2);
    
    /**
     * Get selected item type
     * @param itemSelector item selector object
     * @return selected item type or ITEM_UNKNOWN if none selected
     */
    static ItemType getSelectedItemType(lv_obj_t* itemSelector);
    
    /**
     * Set selected item type
     * @param itemSelector item selector object
     * @param itemType item type to select
     * @return true if item type was found and selected, false otherwise
     */
    static bool setSelectedItemType(lv_obj_t* itemSelector, ItemType itemType);
    
    /**
     * Add custom item button
     * @param itemSelector item selector object
     * @return true if successful, false otherwise
     */
    static bool addCustomItemButton(lv_obj_t* itemSelector);

private:
    static lv_style_t _styleItemBtn;
    static lv_style_t _styleItemBtnSelected;
    static lv_style_t _styleItemLabel;
    static bool _stylesInitialized;
    
    static void _initStyles();
    static void _itemButtonClickHandler(lv_event_t* e);
    static void _customItemClickHandler(lv_event_t* e);
    
    // Item type information
    struct ItemTypeInfo {
        ItemType type;
        const char* name;
        const char* icon;
    };
    
    static const ItemTypeInfo _itemTypes[];
    static const int _itemTypeCount;
};

#endif // UI_COMPONENTS_ITEM_SELECTOR_H
