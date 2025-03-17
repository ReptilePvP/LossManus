/**
 * Enhanced Loss Prevention Log
 * User Interface Layer - Search Screen Implementation
 * 
 * Compatible with LVGL 8.4.0
 */

#include "search_screen.h"
#include "../ui_manager.h"
#include "../components/status_bar.h"
#include "../../data/database.h"
#include "../../data/log_entry.h"
#include "../../app/app_controller.h"

// Static member initialization
lv_obj_t* SearchScreen::_searchInput = nullptr;
lv_obj_t* SearchScreen::_resultsList = nullptr;
lv_obj_t* SearchScreen::_dateFilterDropdown = nullptr;
lv_obj_t* SearchScreen::_itemFilterDropdown = nullptr;

lv_obj_t* SearchScreen::create() {
    DEBUG_PRINTLN("Creating search screen...");
    
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
    lv_label_set_text(title, "Search Logs");
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
    
    // Create search container
    lv_obj_t* searchContainer = lv_obj_create(screen);
    lv_obj_set_size(searchContainer, DISPLAY_WIDTH - 20, 60);
    lv_obj_align(searchContainer, LV_ALIGN_TOP_MID, 0, 90);
    lv_obj_set_style_bg_color(searchContainer, lv_color_hex(0x303030), 0);
    lv_obj_set_style_border_width(searchContainer, 0, 0);
    lv_obj_set_style_radius(searchContainer, 10, 0);
    lv_obj_set_style_pad_all(searchContainer, 10, 0);
    
    // Create search input
    _searchInput = lv_textarea_create(searchContainer);
    lv_obj_set_size(_searchInput, DISPLAY_WIDTH - 140, 40);
    lv_obj_align(_searchInput, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_bg_color(_searchInput, lv_color_hex(0x404040), 0);
    lv_textarea_set_placeholder_text(_searchInput, "Search...");
    lv_obj_add_event_cb(_searchInput, [](lv_event_t* e) {
        if (lv_event_get_code(e) == LV_EVENT_FOCUSED) {
            UIManager::showKeyboard(lv_event_get_target(e));
        }
    }, LV_EVENT_FOCUSED, nullptr);
    
    // Create search button
    lv_obj_t* searchBtn = lv_btn_create(searchContainer);
    lv_obj_set_size(searchBtn, 40, 40);
    lv_obj_align(searchBtn, LV_ALIGN_RIGHT_MID, -50, 0);
    lv_obj_set_style_bg_color(searchBtn, lv_color_hex(0x0078D7), 0);
    lv_obj_add_event_cb(searchBtn, _searchButtonClickHandler, LV_EVENT_CLICKED, nullptr);
    
    // Create search button icon
    lv_obj_t* searchIcon = lv_label_create(searchBtn);
    lv_label_set_text(searchIcon, LV_SYMBOL_SEARCH);
    lv_obj_center(searchIcon);
    
    // Create clear button
    lv_obj_t* clearBtn = lv_btn_create(searchContainer);
    lv_obj_set_size(clearBtn, 40, 40);
    lv_obj_align(clearBtn, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_set_style_bg_color(clearBtn, lv_color_hex(0x505050), 0);
    lv_obj_add_event_cb(clearBtn, _clearButtonClickHandler, LV_EVENT_CLICKED, nullptr);
    
    // Create clear button icon
    lv_obj_t* clearIcon = lv_label_create(clearBtn);
    lv_label_set_text(clearIcon, LV_SYMBOL_CLOSE);
    lv_obj_center(clearIcon);
    
    // Create filter container
    lv_obj_t* filterContainer = lv_obj_create(screen);
    lv_obj_set_size(filterContainer, DISPLAY_WIDTH - 20, 60);
    lv_obj_align(filterContainer, LV_ALIGN_TOP_MID, 0, 160);
    lv_obj_set_style_bg_color(filterContainer, lv_color_hex(0x303030), 0);
    lv_obj_set_style_border_width(filterContainer, 0, 0);
    lv_obj_set_style_radius(filterContainer, 10, 0);
    lv_obj_set_style_pad_all(filterContainer, 10, 0);
    lv_obj_set_flex_flow(filterContainer, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(filterContainer, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    // Create date filter
    lv_obj_t* dateLabel = lv_label_create(filterContainer);
    lv_obj_set_style_text_font(dateLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(dateLabel, lv_color_hex(0xCCCCCC), 0);
    lv_label_set_text(dateLabel, "Date:");
    
    _dateFilterDropdown = lv_dropdown_create(filterContainer);
    lv_dropdown_set_options(_dateFilterDropdown, "All Time\nToday\nLast 7 Days\nLast 30 Days");
    lv_obj_set_size(_dateFilterDropdown, 120, 40);
    lv_obj_add_event_cb(_dateFilterDropdown, _dateFilterEventHandler, LV_EVENT_VALUE_CHANGED, nullptr);
    
    // Create item filter
    lv_obj_t* itemLabel = lv_label_create(filterContainer);
    lv_obj_set_style_text_font(itemLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(itemLabel, lv_color_hex(0xCCCCCC), 0);
    lv_label_set_text(itemLabel, "Item:");
    
    _itemFilterDropdown = lv_dropdown_create(filterContainer);
    lv_dropdown_set_options(_itemFilterDropdown, "All Items\nShirt\nPants\nShoes\nHat\nJacket");
    lv_obj_set_size(_itemFilterDropdown, 120, 40);
    lv_obj_add_event_cb(_itemFilterDropdown, _itemFilterEventHandler, LV_EVENT_VALUE_CHANGED, nullptr);
    
    // Create results list
    _resultsList = lv_list_create(screen);
    lv_obj_set_size(_resultsList, DISPLAY_WIDTH - 20, DISPLAY_HEIGHT - 230);
    lv_obj_align(_resultsList, LV_ALIGN_TOP_MID, 0, 230);
    lv_obj_set_style_bg_color(_resultsList, lv_color_hex(0x303030), 0);
    lv_obj_set_style_border_width(_resultsList, 0, 0);
    lv_obj_set_style_radius(_resultsList, 10, 0);
    lv_obj_set_style_pad_all(_resultsList, 10, 0);
    
    // Add initial message
    lv_obj_t* initialMsg = lv_label_create(_resultsList);
    lv_obj_set_style_text_font(initialMsg, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(initialMsg, lv_color_hex(0x999999), 0);
    lv_label_set_text(initialMsg, "Enter search terms and press search button");
    lv_obj_center(initialMsg);
    
    DEBUG_PRINTLN("Search screen created successfully!");
    return screen;
}

void SearchScreen::update(lv_obj_t* screen) {
    // Update status bar
    StatusBar::update();
    
    // Any other dynamic updates can be added here
}

void SearchScreen::performSearch() {
    if (!_searchInput || !_resultsList || !_dateFilterDropdown || !_itemFilterDropdown) {
        return;
    }
    
    // Get search query
    const char* query = lv_textarea_get_text(_searchInput);
    
    // Get filters
    int dateFilter = lv_dropdown_get_selected(_dateFilterDropdown);
    int itemFilter = lv_dropdown_get_selected(_itemFilterDropdown);
    
    // Display search results
    _displaySearchResults(query, dateFilter, itemFilter);
}

void SearchScreen::_backButtonClickHandler(lv_event_t* e) {
    DEBUG_PRINTLN("Back button clicked");
    UIManager::setScreen(SCREEN_MAIN_MENU);
}

void SearchScreen::_searchButtonClickHandler(lv_event_t* e) {
    DEBUG_PRINTLN("Search button clicked");
    
    // Hide keyboard if visible
    UIManager::hideKeyboard();
    
    // Perform search
    performSearch();
}

void SearchScreen::_clearButtonClickHandler(lv_event_t* e) {
    DEBUG_PRINTLN("Clear button clicked");
    
    // Clear search input
    if (_searchInput) {
        lv_textarea_set_text(_searchInput, "");
    }
    
    // Reset filters
    if (_dateFilterDropdown) {
        lv_dropdown_set_selected(_dateFilterDropdown, 0);
    }
    
    if (_itemFilterDropdown) {
        lv_dropdown_set_selected(_itemFilterDropdown, 0);
    }
    
    // Clear results
    if (_resultsList) {
        lv_obj_clean(_resultsList);
        
        // Add initial message
        lv_obj_t* initialMsg = lv_label_create(_resultsList);
        lv_obj_set_style_text_font(initialMsg, &lv_font_montserrat_16, 0);
        lv_obj_set_style_text_color(initialMsg, lv_color_hex(0x999999), 0);
        lv_label_set_text(initialMsg, "Enter search terms and press search button");
        lv_obj_center(initialMsg);
    }
}

void SearchScreen::_dateFilterEventHandler(lv_event_t* e) {
    DEBUG_PRINTLN("Date filter changed");
    
    // Perform search if there's a query
    if (_searchInput && strlen(lv_textarea_get_text(_searchInput)) > 0) {
        performSearch();
    }
}

void SearchScreen::_itemFilterEventHandler(lv_event_t* e) {
    DEBUG_PRINTLN("Item filter changed");
    
    // Perform search if there's a query
    if (_searchInput && strlen(lv_textarea_get_text(_searchInput)) > 0) {
        performSearch();
    }
}

void SearchScreen::_resultItemClickHandler(lv_event_t* e) {
    lv_obj_t* btn = lv_event_get_target(e);
    int index = (int)lv_obj_get_user_data(btn);
    DEBUG_PRINTF("Result item clicked: %d\n", index);
    
    // Navigate to log detail screen
    LogDetailScreen::setLogIndex(index);
    UIManager::setScreen(SCREEN_LOG_DETAIL);
}

void SearchScreen::_displaySearchResults(const char* query, int dateFilter, int itemFilter) {
    DEBUG_PRINTF("Searching for: %s (Date filter: %d, Item filter: %d)\n", query, dateFilter, itemFilter);
    
    if (!_resultsList) {
        return;
    }
    
    // Clear list
    lv_obj_clean(_resultsList);
    
    // Check if query is empty
    if (strlen(query) == 0) {
        lv_obj_t* noQueryMsg = lv_label_create(_resultsList);
        lv_obj_set_style_text_font(noQueryMsg, &lv_font_montserrat_16, 0);
        lv_obj_set_style_text_color(noQueryMsg, lv_color_hex(0x999999), 0);
        lv_label_set_text(noQueryMsg, "Please enter search terms");
        lv_obj_center(noQueryMsg);
        return;
    }
    
    // In a real implementation, this would search the database
    // For now, we'll create some sample results
    
    // Sample search results
    const int maxResults = 10;
    int numResults = 0;
    
    // Convert query to lowercase for case-insensitive search
    String queryLower = String(query);
    queryLower.toLowerCase();
    
    // Sample data
    const char* genders[] = {"Male", "Female"};
    const char* colors[] = {"Red", "Blue", "Green", "Black", "White"};
    const char* items[] = {"Shirt", "Pants", "Shoes", "Hat", "Jacket"};
    
    for (int i = 0; i < maxResults; i++) {
        // Generate sample data
        int genderIndex = i % 2;
        int shirtColorIndex = i % 5;
        int pantsColorIndex = (i + 2) % 5;
        int itemIndex = i % 5;
        
        // Skip if item filter doesn't match
        if (itemFilter > 0 && itemFilter - 1 != itemIndex) {
            continue;
        }
        
        char title[32];
        char description[64];
        char timestamp[32];
        
        snprintf(title, sizeof(title), "Log #%d", i + 1);
        
        snprintf(description, sizeof(description), "%s, %s shirt, %s pants, %s", 
                genders[genderIndex], 
                colors[shirtColorIndex], 
                colors[pantsColorIndex], 
                items[itemIndex]);
        
        // Generate timestamp (today - i days)
        time_t now = time(nullptr);
        struct tm* timeinfo = localtime(&now);
        timeinfo->tm_mday -= i;
        mktime(timeinfo);
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M", timeinfo);
        
        // Skip if date filter doesn't match
        if (dateFilter == 1 && i > 0) { // Today
            continue;
        } else if (dateFilter == 2 && i >= 7) { // Last 7 days
            continue;
        } else if (dateFilter == 3 && i >= 30) { // Last 30 days
            continue;
        }
        
        // Check if this result matches the search query
        String descLower = String(description);
        descLower.toLowerCase();
        
        if (descLower.indexOf(queryLower) >= 0) {
            // Create result item
            _createResultItem(_resultsList, title, description, timestamp, (i % 3 != 0), i);
            numResults++;
        }
    }
    
    // Show no results message if needed
    if (numResults == 0) {
        lv_obj_t* noResultsMsg = lv_label_create(_resultsList);
        lv_obj_set_style_text_font(noResultsMsg, &lv_font_montserrat_16, 0);
        lv_obj_set_style_text_color(noResultsMsg, lv_color_hex(0x999999), 0);
        lv_label_set_text(noResultsMsg, "No results found");
        lv_obj_center(noResultsMsg);
    }
}

void SearchScreen::_createResultItem(lv_obj_t* parent, const char* title, const char* description, 
                                const char* timestamp, bool synced, int index) {
    // Create list button
    lv_obj_t* btn = lv_list_add_btn(parent, nullptr, "");
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x404040), 0);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x505050), LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x606060), LV_STATE_FOCUSED);
    lv_obj_set_style_pad_all(btn, 10, 0);
    lv_obj_set_height(btn, 80);
    lv_obj_add_event_cb(btn, _resultItemClickHandler, LV_EVENT_CLICKED, nullptr);
    
    // Store index as user data
    lv_obj_set_user_data(btn, (void*)(intptr_t)index);
    
    // Create container for content
    lv_obj_t* container = lv_obj_create(btn);
    lv_obj_set_size(container, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_opa(container, LV_OPA_0, 0);
    lv_obj_set_style_border_width(container, 0, 0);
    lv_obj_set_style_pad_all(container, 0, 0);
    
    // Create title label
    lv_obj_t* titleLabel = lv_label_create(container);
    lv_obj_set_style_text_font(titleLabel, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(titleLabel, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(titleLabel, title);
    lv_obj_align(titleLabel, LV_ALIGN_TOP_LEFT, 0, 0);
    
    // Create description label
    lv_obj_t* descLabel = lv_label_create(container);
    lv_obj_set_style_text_font(descLabel, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(descLabel, lv_color_hex(0xCCCCCC), 0);
    lv_label_set_text(descLabel, description);
    lv_obj_align(descLabel, LV_ALIGN_TOP_LEFT, 0, 25);
    
    // Create timestamp label
    lv_obj_t* timeLabel = lv_label_create(container);
    lv_obj_set_style_text_font(timeLabel, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(timeLabel, lv_color_hex(0x999999), 0);
    lv_label_set_text(timeLabel, timestamp);
    lv_obj_align(timeLabel, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    
    // Create sync status icon
    lv_obj_t* syncIcon = lv_label_create(container);
    lv_obj_set_style_text_font(syncIcon, &lv_font_montserrat_16, 0);
    
    if (synced) {
        lv_obj_set_style_text_color(syncIcon, lv_color_hex(0x00CC00), 0);
        lv_label_set_text(syncIcon, LV_SYMBOL_OK);
    } else {
        lv_obj_set_style_text_color(syncIcon, lv_color_hex(0xFFCC00), 0);
        lv_label_set_text(syncIcon, LV_SYMBOL_WARNING);
    }
    
    lv_obj_align(syncIcon, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
}
