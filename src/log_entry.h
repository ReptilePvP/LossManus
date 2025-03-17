/**
 * Enhanced Loss Prevention Log
 * Data Management Layer - Log Entry Structure
 * 
 * This file contains the structure and methods for log entries
 */

#ifndef DATA_LOG_ENTRY_H
#define DATA_LOG_ENTRY_H

#include <Arduino.h>
#include <time.h>
#include "../config.h"

enum Gender {
    GENDER_UNKNOWN = 0,
    GENDER_MALE = 1,
    GENDER_FEMALE = 2,
    GENDER_OTHER = 3
};

enum ItemType {
    ITEM_UNKNOWN = 0,
    ITEM_CLOTHING = 1,
    ITEM_ELECTRONICS = 2,
    ITEM_COSMETICS = 3,
    ITEM_ACCESSORIES = 4,
    ITEM_FOOD = 5,
    ITEM_OTHER = 6
};

struct Color {
    String name;
    uint32_t rgb;
    
    Color() : name("Unknown"), rgb(0) {}
    Color(const String& _name, uint32_t _rgb) : name(_name), rgb(_rgb) {}
};

class LogEntry {
public:
    // Constructors
    LogEntry();
    LogEntry(time_t timestamp);
    
    // Getters and setters
    time_t getTimestamp() const;
    void setTimestamp(time_t timestamp);
    
    Gender getGender() const;
    void setGender(Gender gender);
    
    Color getShirtColor() const;
    void setShirtColor(const Color& color);
    
    Color getPantsColor() const;
    void setPantsColor(const Color& color);
    
    Color getShoesColor() const;
    void setShoesColor(const Color& color);
    
    ItemType getItemType() const;
    void setItemType(ItemType type);
    
    String getItemDescription() const;
    void setItemDescription(const String& description);
    
    String getNotes() const;
    void setNotes(const String& notes);
    
    // Serialization methods
    String serialize() const;
    bool deserialize(const String& data);
    
    // Utility methods
    String getFormattedTimestamp(const char* format = "%Y-%m-%d %H:%M:%S") const;
    String getSummary() const;
    bool isValid() const;
    
private:
    time_t _timestamp;
    Gender _gender;
    Color _shirtColor;
    Color _pantsColor;
    Color _shoesColor;
    ItemType _itemType;
    String _itemDescription;
    String _notes;
};

#endif // DATA_LOG_ENTRY_H
