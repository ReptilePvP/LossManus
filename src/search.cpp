/**
 * Enhanced Loss Prevention Log
 * Data Management Layer - Search Engine Implementation
 */

#include "search.h"
#include <algorithm>

// Static member initialization
bool SearchEngine::_initialized = false;

// SearchFilter static constructors
SearchFilter SearchFilter::createDateRangeFilter(time_t startTime, time_t endTime) {
    SearchFilter filter;
    filter.type = FILTER_DATE_RANGE;
    filter.dateRange.startTime = startTime;
    filter.dateRange.endTime = endTime;
    return filter;
}

SearchFilter SearchFilter::createGenderFilter(Gender gender) {
    SearchFilter filter;
    filter.type = FILTER_GENDER;
    filter.gender = gender;
    return filter;
}

SearchFilter SearchFilter::createShirtColorFilter(const String& colorName) {
    SearchFilter filter;
    filter.type = FILTER_SHIRT_COLOR;
    filter.color.colorName = colorName;
    return filter;
}

SearchFilter SearchFilter::createPantsColorFilter(const String& colorName) {
    SearchFilter filter;
    filter.type = FILTER_PANTS_COLOR;
    filter.color.colorName = colorName;
    return filter;
}

SearchFilter SearchFilter::createShoesColorFilter(const String& colorName) {
    SearchFilter filter;
    filter.type = FILTER_SHOES_COLOR;
    filter.color.colorName = colorName;
    return filter;
}

SearchFilter SearchFilter::createItemTypeFilter(ItemType itemType) {
    SearchFilter filter;
    filter.type = FILTER_ITEM_TYPE;
    filter.itemType = itemType;
    return filter;
}

SearchFilter SearchFilter::createTextFilter(const String& text) {
    SearchFilter filter;
    filter.type = FILTER_TEXT;
    filter.textSearch.text = text;
    return filter;
}

// SearchEngine implementation
bool SearchEngine::init() {
    DEBUG_PRINT("Initializing search engine...");
    
    if (_initialized) {
        DEBUG_PRINT("Search engine already initialized");
        return true;
    }
    
    _initialized = true;
    DEBUG_PRINT("Search engine initialization complete");
    return true;
}

std::vector<LogEntry> SearchEngine::search(const std::vector<LogEntry>& entries, const SearchFilter& filter) {
    std::vector<LogEntry> result;
    
    for (const auto& entry : entries) {
        bool match = false;
        
        switch (filter.type) {
            case FILTER_DATE_RANGE:
                match = filterByDateRange(entry, filter.dateRange.startTime, filter.dateRange.endTime);
                break;
            case FILTER_GENDER:
                match = filterByGender(entry, filter.gender);
                break;
            case FILTER_SHIRT_COLOR:
                match = filterByShirtColor(entry, filter.color.colorName);
                break;
            case FILTER_PANTS_COLOR:
                match = filterByPantsColor(entry, filter.color.colorName);
                break;
            case FILTER_SHOES_COLOR:
                match = filterByShoesColor(entry, filter.color.colorName);
                break;
            case FILTER_ITEM_TYPE:
                match = filterByItemType(entry, filter.itemType);
                break;
            case FILTER_TEXT:
                match = filterByText(entry, filter.textSearch.text);
                break;
        }
        
        if (match) {
            result.push_back(entry);
        }
    }
    
    return result;
}

std::vector<LogEntry> SearchEngine::searchMultiple(const std::vector<LogEntry>& entries, const std::vector<SearchFilter>& filters) {
    if (filters.empty()) {
        return entries;
    }
    
    std::vector<LogEntry> result = entries;
    
    for (const auto& filter : filters) {
        result = search(result, filter);
    }
    
    return result;
}

std::vector<LogEntry> SearchEngine::sortByTimestampDesc(const std::vector<LogEntry>& entries) {
    std::vector<LogEntry> sorted = entries;
    std::sort(sorted.begin(), sorted.end(), [](const LogEntry& a, const LogEntry& b) {
        return a.getTimestamp() > b.getTimestamp();
    });
    return sorted;
}

std::vector<LogEntry> SearchEngine::sortByTimestampAsc(const std::vector<LogEntry>& entries) {
    std::vector<LogEntry> sorted = entries;
    std::sort(sorted.begin(), sorted.end(), [](const LogEntry& a, const LogEntry& b) {
        return a.getTimestamp() < b.getTimestamp();
    });
    return sorted;
}

std::vector<LogEntry> SearchEngine::sortCustom(const std::vector<LogEntry>& entries, 
                                             std::function<bool(const LogEntry&, const LogEntry&)> comparator) {
    std::vector<LogEntry> sorted = entries;
    std::sort(sorted.begin(), sorted.end(), comparator);
    return sorted;
}

// Filter functions
bool SearchEngine::filterByDateRange(const LogEntry& entry, time_t startTime, time_t endTime) {
    time_t timestamp = entry.getTimestamp();
    return (timestamp >= startTime && timestamp <= endTime);
}

bool SearchEngine::filterByGender(const LogEntry& entry, Gender gender) {
    return entry.getGender() == gender;
}

bool SearchEngine::filterByShirtColor(const LogEntry& entry, const String& colorName) {
    String entryColor = entry.getShirtColor().name;
    entryColor.toLowerCase();
    
    String searchColor = colorName;
    searchColor.toLowerCase();
    
    return entryColor.indexOf(searchColor) >= 0;
}

bool SearchEngine::filterByPantsColor(const LogEntry& entry, const String& colorName) {
    String entryColor = entry.getPantsColor().name;
    entryColor.toLowerCase();
    
    String searchColor = colorName;
    searchColor.toLowerCase();
    
    return entryColor.indexOf(searchColor) >= 0;
}

bool SearchEngine::filterByShoesColor(const LogEntry& entry, const String& colorName) {
    String entryColor = entry.getShoesColor().name;
    entryColor.toLowerCase();
    
    String searchColor = colorName;
    searchColor.toLowerCase();
    
    return entryColor.indexOf(searchColor) >= 0;
}

bool SearchEngine::filterByItemType(const LogEntry& entry, ItemType itemType) {
    return entry.getItemType() == itemType;
}

bool SearchEngine::filterByText(const LogEntry& entry, const String& text) {
    if (text.isEmpty()) {
        return true;
    }
    
    String searchText = text;
    searchText.toLowerCase();
    
    // Search in item description
    String desc = entry.getItemDescription();
    desc.toLowerCase();
    if (desc.indexOf(searchText) >= 0) {
        return true;
    }
    
    // Search in notes
    String notes = entry.getNotes();
    notes.toLowerCase();
    if (notes.indexOf(searchText) >= 0) {
        return true;
    }
    
    // Search in shirt color
    String shirtColor = entry.getShirtColor().name;
    shirtColor.toLowerCase();
    if (shirtColor.indexOf(searchText) >= 0) {
        return true;
    }
    
    // Search in pants color
    String pantsColor = entry.getPantsColor().name;
    pantsColor.toLowerCase();
    if (pantsColor.indexOf(searchText) >= 0) {
        return true;
    }
    
    // Search in shoes color
    String shoesColor = entry.getShoesColor().name;
    shoesColor.toLowerCase();
    if (shoesColor.indexOf(searchText) >= 0) {
        return true;
    }
    
    return false;
}
