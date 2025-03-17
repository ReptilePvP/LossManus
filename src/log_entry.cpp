/**
 * Enhanced Loss Prevention Log
 * Data Management Layer - Log Entry Implementation
 */

#include "log_entry.h"
#include "../hal/rtc.h"

LogEntry::LogEntry() {
    _timestamp = RtcHAL::getTime();
    _gender = GENDER_UNKNOWN;
    _itemType = ITEM_UNKNOWN;
}

LogEntry::LogEntry(time_t timestamp) {
    _timestamp = timestamp;
    _gender = GENDER_UNKNOWN;
    _itemType = ITEM_UNKNOWN;
}

time_t LogEntry::getTimestamp() const {
    return _timestamp;
}

void LogEntry::setTimestamp(time_t timestamp) {
    _timestamp = timestamp;
}

Gender LogEntry::getGender() const {
    return _gender;
}

void LogEntry::setGender(Gender gender) {
    _gender = gender;
}

Color LogEntry::getShirtColor() const {
    return _shirtColor;
}

void LogEntry::setShirtColor(const Color& color) {
    _shirtColor = color;
}

Color LogEntry::getPantsColor() const {
    return _pantsColor;
}

void LogEntry::setPantsColor(const Color& color) {
    _pantsColor = color;
}

Color LogEntry::getShoesColor() const {
    return _shoesColor;
}

void LogEntry::setShoesColor(const Color& color) {
    _shoesColor = color;
}

ItemType LogEntry::getItemType() const {
    return _itemType;
}

void LogEntry::setItemType(ItemType type) {
    _itemType = type;
}

String LogEntry::getItemDescription() const {
    return _itemDescription;
}

void LogEntry::setItemDescription(const String& description) {
    _itemDescription = description;
}

String LogEntry::getNotes() const {
    return _notes;
}

void LogEntry::setNotes(const String& notes) {
    _notes = notes;
}

String LogEntry::serialize() const {
    // Format: timestamp|gender|shirtColor.name|shirtColor.rgb|pantsColor.name|pantsColor.rgb|shoesColor.name|shoesColor.rgb|itemType|itemDescription|notes
    String serialized = String(_timestamp) + "|";
    serialized += String((int)_gender) + "|";
    serialized += _shirtColor.name + "|";
    serialized += String(_shirtColor.rgb, HEX) + "|";
    serialized += _pantsColor.name + "|";
    serialized += String(_pantsColor.rgb, HEX) + "|";
    serialized += _shoesColor.name + "|";
    serialized += String(_shoesColor.rgb, HEX) + "|";
    serialized += String((int)_itemType) + "|";
    serialized += _itemDescription + "|";
    serialized += _notes;
    
    return serialized;
}

bool LogEntry::deserialize(const String& data) {
    // Split the serialized string by delimiter
    int delimiterPositions[10];
    int delimiterCount = 0;
    
    for (size_t i = 0; i < data.length() && delimiterCount < 10; i++) {
        if (data[i] == '|') {
            delimiterPositions[delimiterCount++] = i;
        }
    }
    
    if (delimiterCount != 10) {
        DEBUG_PRINT("Invalid log entry format");
        return false;
    }
    
    // Extract fields
    String timestampStr = data.substring(0, delimiterPositions[0]);
    String genderStr = data.substring(delimiterPositions[0] + 1, delimiterPositions[1]);
    String shirtColorName = data.substring(delimiterPositions[1] + 1, delimiterPositions[2]);
    String shirtColorRgbStr = data.substring(delimiterPositions[2] + 1, delimiterPositions[3]);
    String pantsColorName = data.substring(delimiterPositions[3] + 1, delimiterPositions[4]);
    String pantsColorRgbStr = data.substring(delimiterPositions[4] + 1, delimiterPositions[5]);
    String shoesColorName = data.substring(delimiterPositions[5] + 1, delimiterPositions[6]);
    String shoesColorRgbStr = data.substring(delimiterPositions[6] + 1, delimiterPositions[7]);
    String itemTypeStr = data.substring(delimiterPositions[7] + 1, delimiterPositions[8]);
    _itemDescription = data.substring(delimiterPositions[8] + 1, delimiterPositions[9]);
    _notes = data.substring(delimiterPositions[9] + 1);
    
    // Convert values
    _timestamp = (time_t)timestampStr.toInt();
    _gender = (Gender)genderStr.toInt();
    _shirtColor.name = shirtColorName;
    _shirtColor.rgb = (uint32_t)strtoul(shirtColorRgbStr.c_str(), NULL, 16);
    _pantsColor.name = pantsColorName;
    _pantsColor.rgb = (uint32_t)strtoul(pantsColorRgbStr.c_str(), NULL, 16);
    _shoesColor.name = shoesColorName;
    _shoesColor.rgb = (uint32_t)strtoul(shoesColorRgbStr.c_str(), NULL, 16);
    _itemType = (ItemType)itemTypeStr.toInt();
    
    return true;
}

String LogEntry::getFormattedTimestamp(const char* format) const {
    return RtcHAL::formatTime(_timestamp, format);
}

String LogEntry::getSummary() const {
    String genderStr;
    switch (_gender) {
        case GENDER_MALE: genderStr = "Male"; break;
        case GENDER_FEMALE: genderStr = "Female"; break;
        case GENDER_OTHER: genderStr = "Other"; break;
        default: genderStr = "Unknown"; break;
    }
    
    String itemTypeStr;
    switch (_itemType) {
        case ITEM_CLOTHING: itemTypeStr = "Clothing"; break;
        case ITEM_ELECTRONICS: itemTypeStr = "Electronics"; break;
        case ITEM_COSMETICS: itemTypeStr = "Cosmetics"; break;
        case ITEM_ACCESSORIES: itemTypeStr = "Accessories"; break;
        case ITEM_FOOD: itemTypeStr = "Food"; break;
        case ITEM_OTHER: itemTypeStr = "Other"; break;
        default: itemTypeStr = "Unknown"; break;
    }
    
    String summary = getFormattedTimestamp() + " - ";
    summary += genderStr + ", ";
    summary += "Shirt: " + _shirtColor.name + ", ";
    summary += "Pants: " + _pantsColor.name + ", ";
    summary += "Shoes: " + _shoesColor.name + ", ";
    summary += "Item: " + itemTypeStr;
    
    if (_itemDescription.length() > 0) {
        summary += " (" + _itemDescription + ")";
    }
    
    return summary;
}

bool LogEntry::isValid() const {
    // Basic validation
    if (_timestamp <= 0) return false;
    if (_gender == GENDER_UNKNOWN) return false;
    if (_itemType == ITEM_UNKNOWN) return false;
    
    return true;
}
