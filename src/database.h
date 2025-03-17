/**
 * Enhanced Loss Prevention Log
 * Data Management Layer - Database Interface
 * 
 * This file contains the interface for database operations
 */

#ifndef DATA_DATABASE_H
#define DATA_DATABASE_H

#include <Arduino.h>
#include <vector>
#include "log_entry.h"
#include "../hal/storage.h"
#include "../config.h"

class Database {
public:
    /**
     * Initialize the database
     * @return true if successful, false otherwise
     */
    static bool init();
    
    /**
     * Add a log entry to the database
     * @param entry log entry to add
     * @return true if successful, false otherwise
     */
    static bool addEntry(const LogEntry& entry);
    
    /**
     * Get a log entry by index
     * @param index entry index
     * @param entry reference to store the entry
     * @return true if successful, false otherwise
     */
    static bool getEntry(size_t index, LogEntry& entry);
    
    /**
     * Get all log entries
     * @return vector of log entries
     */
    static std::vector<LogEntry> getAllEntries();
    
    /**
     * Get entries by date range
     * @param startTime start of date range
     * @param endTime end of date range
     * @return vector of log entries
     */
    static std::vector<LogEntry> getEntriesByDateRange(time_t startTime, time_t endTime);
    
    /**
     * Get entries by gender
     * @param gender gender to filter by
     * @return vector of log entries
     */
    static std::vector<LogEntry> getEntriesByGender(Gender gender);
    
    /**
     * Get entries by item type
     * @param itemType item type to filter by
     * @return vector of log entries
     */
    static std::vector<LogEntry> getEntriesByItemType(ItemType itemType);
    
    /**
     * Search entries by text
     * @param searchText text to search for
     * @return vector of log entries
     */
    static std::vector<LogEntry> searchEntries(const String& searchText);
    
    /**
     * Get the number of entries in the database
     * @return number of entries
     */
    static size_t getEntryCount();
    
    /**
     * Delete an entry by index
     * @param index entry index
     * @return true if successful, false otherwise
     */
    static bool deleteEntry(size_t index);
    
    /**
     * Delete all entries
     * @return true if successful, false otherwise
     */
    static bool deleteAllEntries();
    
    /**
     * Export database to file
     * @param filename file to export to
     * @return true if successful, false otherwise
     */
    static bool exportToFile(const String& filename);
    
    /**
     * Import database from file
     * @param filename file to import from
     * @return true if successful, false otherwise
     */
    static bool importFromFile(const String& filename);
    
    /**
     * Backup database
     * @return true if successful, false otherwise
     */
    static bool backup();

private:
    static bool _initialized;
    static std::vector<LogEntry> _entries;
    static bool _dirty;
    
    static bool loadFromFile();
    static bool saveToFile();
};

#endif // DATA_DATABASE_H
