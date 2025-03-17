/**
 * Enhanced Loss Prevention Log
 * Data Management Layer - Search Engine
 * 
 * This file contains the interface for search and filter operations
 */

#ifndef DATA_SEARCH_H
#define DATA_SEARCH_H

#include <Arduino.h>
#include <vector>
#include <functional>
#include "log_entry.h"
#include "../config.h"

// Search filter type
enum FilterType {
    FILTER_DATE_RANGE,
    FILTER_GENDER,
    FILTER_SHIRT_COLOR,
    FILTER_PANTS_COLOR,
    FILTER_SHOES_COLOR,
    FILTER_ITEM_TYPE,
    FILTER_TEXT
};

// Search filter class
class SearchFilter {
public:
    FilterType type;
    
    // Filter parameters
    union {
        struct {
            time_t startTime;
            time_t endTime;
        } dateRange;
        
        Gender gender;
        
        struct {
            String colorName;
        } color;
        
        ItemType itemType;
        
        struct {
            String text;
        } textSearch;
    };
    
    // Constructors for different filter types
    static SearchFilter createDateRangeFilter(time_t startTime, time_t endTime);
    static SearchFilter createGenderFilter(Gender gender);
    static SearchFilter createShirtColorFilter(const String& colorName);
    static SearchFilter createPantsColorFilter(const String& colorName);
    static SearchFilter createShoesColorFilter(const String& colorName);
    static SearchFilter createItemTypeFilter(ItemType itemType);
    static SearchFilter createTextFilter(const String& text);
};

class SearchEngine {
public:
    /**
     * Initialize the search engine
     * @return true if successful, false otherwise
     */
    static bool init();
    
    /**
     * Search entries with a single filter
     * @param entries vector of entries to search in
     * @param filter search filter to apply
     * @return filtered vector of entries
     */
    static std::vector<LogEntry> search(const std::vector<LogEntry>& entries, const SearchFilter& filter);
    
    /**
     * Search entries with multiple filters (AND logic)
     * @param entries vector of entries to search in
     * @param filters vector of search filters to apply
     * @return filtered vector of entries
     */
    static std::vector<LogEntry> searchMultiple(const std::vector<LogEntry>& entries, const std::vector<SearchFilter>& filters);
    
    /**
     * Sort entries by timestamp (newest first)
     * @param entries vector of entries to sort
     * @return sorted vector of entries
     */
    static std::vector<LogEntry> sortByTimestampDesc(const std::vector<LogEntry>& entries);
    
    /**
     * Sort entries by timestamp (oldest first)
     * @param entries vector of entries to sort
     * @return sorted vector of entries
     */
    static std::vector<LogEntry> sortByTimestampAsc(const std::vector<LogEntry>& entries);
    
    /**
     * Sort entries by custom comparator
     * @param entries vector of entries to sort
     * @param comparator function to compare entries
     * @return sorted vector of entries
     */
    static std::vector<LogEntry> sortCustom(const std::vector<LogEntry>& entries, 
                                           std::function<bool(const LogEntry&, const LogEntry&)> comparator);

private:
    static bool _initialized;
    
    // Filter functions
    static bool filterByDateRange(const LogEntry& entry, time_t startTime, time_t endTime);
    static bool filterByGender(const LogEntry& entry, Gender gender);
    static bool filterByShirtColor(const LogEntry& entry, const String& colorName);
    static bool filterByPantsColor(const LogEntry& entry, const String& colorName);
    static bool filterByShoesColor(const LogEntry& entry, const String& colorName);
    static bool filterByItemType(const LogEntry& entry, ItemType itemType);
    static bool filterByText(const LogEntry& entry, const String& text);
};

#endif // DATA_SEARCH_H
