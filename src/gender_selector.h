/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Gender Selector Component
 * 
 * This file contains the interface for the gender selector component
 * Compatible with LVGL 8.4.0
 */

#ifndef UI_COMPONENTS_GENDER_SELECTOR_H
#define UI_COMPONENTS_GENDER_SELECTOR_H

#include <Arduino.h>
#include <lvgl.h>
#include "../../config.h"
#include "../../data/log_entry.h"

// Gender selection callback
typedef void (*GenderSelectedCallback)(Gender gender);

class GenderSelector {
public:
    /**
     * Create gender selector
     * @param parent parent object
     * @param callback function to call when gender is selected
     * @return gender selector object
     */
    static lv_obj_t* create(lv_obj_t* parent, GenderSelectedCallback callback);
    
    /**
     * Get selected gender
     * @param genderSelector gender selector object
     * @return selected gender or GENDER_UNKNOWN if none selected
     */
    static Gender getSelectedGender(lv_obj_t* genderSelector);
    
    /**
     * Set selected gender
     * @param genderSelector gender selector object
     * @param gender gender to select
     * @return true if gender was found and selected, false otherwise
     */
    static bool setSelectedGender(lv_obj_t* genderSelector, Gender gender);

private:
    static lv_style_t _styleGenderBtn;
    static lv_style_t _styleGenderBtnSelected;
    static lv_style_t _styleGenderLabel;
    static bool _stylesInitialized;
    
    static void _initStyles();
    static void _genderButtonClickHandler(lv_event_t* e);
    
    // Gender information
    struct GenderInfo {
        Gender gender;
        const char* name;
        const char* icon;
    };
    
    static const GenderInfo _genders[];
    static const int _genderCount;
};

#endif // UI_COMPONENTS_GENDER_SELECTOR_H
