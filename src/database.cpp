/**
 * Enhanced Loss Prevention Log
 * Data Management Layer - Database Implementation
 */

#include "database.h"

// Static member initialization
bool Database::_initialized = false;
std::vector<LogEntry> Database::_entries;
bool Database::_dirty = false;

bool Database::init() {
    DEBUG_PRINT("Initializing database...");
    
    if (_initialized) {
        DEBUG_PRINT("Database already initialized");
        return true;
    }
    
    // Ensure storage is initialized
    if (!StorageHAL::isAvailable()) {
        if (!StorageHAL::init()) {
            DEBUG_PRINT("Failed to initialize storage for database");
            return false;
        }
    }
    
    // Load entries from file
    if (!loadFromFile()) {
        DEBUG_PRINT("No existing database found, creating new one");
    }
    
    _initialized = true;
    _dirty = false;
    
    DEBUG_PRINTF("Database initialized with %d entries", _entries.size());
    return true;
}

bool Database::addEntry(const LogEntry& entry) {
    if (!_initialized) {
        if (!init()) {
            return false;
        }
    }
    
    _entries.push_back(entry);
    _dirty = true;
    
    // Save to file
    if (!saveToFile()) {
        DEBUG_PRINT("Failed to save database after adding entry");
        return false;
    }
    
    DEBUG_PRINTF("Added entry, total entries: %d", _entries.size());
    return true;
}

bool Database::getEntry(size_t index, LogEntry& entry) {
    if (!_initialized || index >= _entries.size()) {
        return false;
    }
    
    entry = _entries[index];
    return true;
}

std::vector<LogEntry> Database::getAllEntries() {
    if (!_initialized) {
        if (!init()) {
            return std::vector<LogEntry>();
        }
    }
    
    return _entries;
}

std::vector<LogEntry> Database::getEntriesByDateRange(time_t startTime, time_t endTime) {
    std::vector<LogEntry> result;
    
    if (!_initialized) {
        if (!init()) {
            return result;
        }
    }
    
    for (const auto& entry : _entries) {
        time_t timestamp = entry.getTimestamp();
        if (timestamp >= startTime && timestamp <= endTime) {
            result.push_back(entry);
        }
    }
    
    return result;
}

std::vector<LogEntry> Database::getEntriesByGender(Gender gender) {
    std::vector<LogEntry> result;
    
    if (!_initialized) {
        if (!init()) {
            return result;
        }
    }
    
    for (const auto& entry : _entries) {
        if (entry.getGender() == gender) {
            result.push_back(entry);
        }
    }
    
    return result;
}

std::vector<LogEntry> Database::getEntriesByItemType(ItemType itemType) {
    std::vector<LogEntry> result;
    
    if (!_initialized) {
        if (!init()) {
            return result;
        }
    }
    
    for (const auto& entry : _entries) {
        if (entry.getItemType() == itemType) {
            result.push_back(entry);
        }
    }
    
    return result;
}

std::vector<LogEntry> Database::searchEntries(const String& searchText) {
    std::vector<LogEntry> result;
    
    if (!_initialized) {
        if (!init()) {
            return result;
        }
    }
    
    String searchLower = searchText;
    searchLower.toLowerCase();
    
    for (const auto& entry : _entries) {
        // Search in item description
        String desc = entry.getItemDescription();
        desc.toLowerCase();
        if (desc.indexOf(searchLower) >= 0) {
            result.push_back(entry);
            continue;
        }
        
        // Search in notes
        String notes = entry.getNotes();
        notes.toLowerCase();
        if (notes.indexOf(searchLower) >= 0) {
            result.push_back(entry);
            continue;
        }
        
        // Search in shirt color
        String shirtColor = entry.getShirtColor().name;
        shirtColor.toLowerCase();
        if (shirtColor.indexOf(searchLower) >= 0) {
            result.push_back(entry);
            continue;
        }
        
        // Search in pants color
        String pantsColor = entry.getPantsColor().name;
        pantsColor.toLowerCase();
        if (pantsColor.indexOf(searchLower) >= 0) {
            result.push_back(entry);
            continue;
        }
        
        // Search in shoes color
        String shoesColor = entry.getShoesColor().name;
        shoesColor.toLowerCase();
        if (shoesColor.indexOf(searchLower) >= 0) {
            result.push_back(entry);
            continue;
        }
    }
    
    return result;
}

size_t Database::getEntryCount() {
    if (!_initialized) {
        if (!init()) {
            return 0;
        }
    }
    
    return _entries.size();
}

bool Database::deleteEntry(size_t index) {
    if (!_initialized || index >= _entries.size()) {
        return false;
    }
    
    _entries.erase(_entries.begin() + index);
    _dirty = true;
    
    // Save to file
    if (!saveToFile()) {
        DEBUG_PRINT("Failed to save database after deleting entry");
        return false;
    }
    
    DEBUG_PRINTF("Deleted entry at index %d, total entries: %d", index, _entries.size());
    return true;
}

bool Database::deleteAllEntries() {
    if (!_initialized) {
        if (!init()) {
            return false;
        }
    }
    
    _entries.clear();
    _dirty = true;
    
    // Save to file
    if (!saveToFile()) {
        DEBUG_PRINT("Failed to save database after deleting all entries");
        return false;
    }
    
    DEBUG_PRINT("Deleted all entries");
    return true;
}

bool Database::exportToFile(const String& filename) {
    if (!_initialized) {
        if (!init()) {
            return false;
        }
    }
    
    String content;
    
    // Add header
    content = "timestamp|gender|shirt_color|shirt_rgb|pants_color|pants_rgb|shoes_color|shoes_rgb|item_type|item_description|notes\n";
    
    // Add entries
    for (const auto& entry : _entries) {
        content += entry.serialize() + "\n";
    }
    
    // Write to file
    if (StorageHAL::writeFile(filename.c_str(), content.c_str(), content.length()) < 0) {
        DEBUG_PRINTF("Failed to export database to file: %s", filename.c_str());
        return false;
    }
    
    DEBUG_PRINTF("Exported %d entries to file: %s", _entries.size(), filename.c_str());
    return true;
}

bool Database::importFromFile(const String& filename) {
    if (!_initialized) {
        if (!init()) {
            return false;
        }
    }
    
    // Check if file exists
    if (!StorageHAL::fileExists(filename.c_str())) {
        DEBUG_PRINTF("Import file not found: %s", filename.c_str());
        return false;
    }
    
    // Read file content
    int fileSize = StorageHAL::getFileSize(filename.c_str());
    if (fileSize <= 0) {
        DEBUG_PRINTF("Import file is empty: %s", filename.c_str());
        return false;
    }
    
    char* buffer = new char[fileSize + 1];
    if (StorageHAL::readFile(filename.c_str(), buffer, fileSize + 1) < 0) {
        DEBUG_PRINTF("Failed to read import file: %s", filename.c_str());
        delete[] buffer;
        return false;
    }
    
    // Parse file content
    String content = String(buffer);
    delete[] buffer;
    
    // Split into lines
    std::vector<String> lines;
    int start = 0;
    int end = content.indexOf('\n');
    
    while (end >= 0) {
        lines.push_back(content.substring(start, end));
        start = end + 1;
        end = content.indexOf('\n', start);
    }
    
    if (start < content.length()) {
        lines.push_back(content.substring(start));
    }
    
    // Skip header line
    if (lines.size() > 0) {
        lines.erase(lines.begin());
    }
    
    // Parse entries
    std::vector<LogEntry> importedEntries;
    for (const auto& line : lines) {
        if (line.length() > 0) {
            LogEntry entry;
            if (entry.deserialize(line)) {
                importedEntries.push_back(entry);
            } else {
                DEBUG_PRINT("Failed to parse entry from import file");
            }
        }
    }
    
    // Replace current entries
    _entries = importedEntries;
    _dirty = true;
    
    // Save to file
    if (!saveToFile()) {
        DEBUG_PRINT("Failed to save database after import");
        return false;
    }
    
    DEBUG_PRINTF("Imported %d entries from file: %s", _entries.size(), filename.c_str());
    return true;
}

bool Database::backup() {
    if (!_initialized) {
        if (!init()) {
            return false;
        }
    }
    
    // Create backup filename with timestamp
    char timestamp[20];
    time_t now;
    time(&now);
    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", localtime(&now));
    
    String backupFilename = "/backup/loss_prevention_" + String(timestamp) + ".db";
    
    // Export to backup file
    if (!exportToFile(backupFilename)) {
        DEBUG_PRINT("Failed to create database backup");
        return false;
    }
    
    DEBUG_PRINTF("Database backup created: %s", backupFilename.c_str());
    return true;
}

bool Database::loadFromFile() {
    // Clear current entries
    _entries.clear();
    
    // Check if database file exists
    if (!StorageHAL::fileExists(DATABASE_FILENAME)) {
        DEBUG_PRINT("Database file not found, starting with empty database");
        return false;
    }
    
    // Read file content
    int fileSize = StorageHAL::getFileSize(DATABASE_FILENAME);
    if (fileSize <= 0) {
        DEBUG_PRINT("Database file is empty");
        return false;
    }
    
    char* buffer = new char[fileSize + 1];
    if (StorageHAL::readFile(DATABASE_FILENAME, buffer, fileSize + 1) < 0) {
        DEBUG_PRINT("Failed to read database file");
        delete[] buffer;
        return false;
    }
    
    // Parse file content
    String content = String(buffer);
    delete[] buffer;
    
    // Split into lines
    int start = 0;
    int end = content.indexOf('\n');
    
    while (end >= 0) {
        String line = content.substring(start, end);
        if (line.length() > 0) {
            LogEntry entry;
            if (entry.deserialize(line)) {
                _entries.push_back(entry);
            } else {
                DEBUG_PRINT("Failed to parse entry from database file");
            }
        }
        start = end + 1;
        end = content.indexOf('\n', start);
    }
    
    if (start < content.length()) {
        String line = content.substring(start);
        if (line.length() > 0) {
            LogEntry entry;
            if (entry.deserialize(line)) {
                _entries.push_back(entry);
            }
        }
    }
    
    DEBUG_PRINTF("Loaded %d entries from database file", _entries.size());
    return true;
}

bool Database::saveToFile() {
    if (!_dirty) {
        DEBUG_PRINT("Database not modified, skipping save");
        return true;
    }
    
    String content;
    
    // Add entries
    for (const auto& entry : _entries) {
        content += entry.serialize() + "\n";
    }
    
    // Write to file
    if (StorageHAL::writeFile(DATABASE_FILENAME, content.c_str(), content.length()) < 0) {
        DEBUG_PRINT("Failed to save database to file");
        return false;
    }
    
    _dirty = false;
    DEBUG_PRINTF("Saved %d entries to database file", _entries.size());
    return true;
}
