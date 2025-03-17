/**
 * Enhanced Loss Prevention Log
 * Data Management Layer - Export Utilities Implementation
 */

#include "export.h"

bool ExportUtil::exportToFile(const std::vector<LogEntry>& entries, const String& filename, ExportFormat format) {
    String content;
    
    switch (format) {
        case FORMAT_CSV:
            content = exportToCSV(entries);
            break;
        case FORMAT_JSON:
            content = exportToJSON(entries);
            break;
        case FORMAT_TEXT:
            content = exportToText(entries);
            break;
        default:
            DEBUG_PRINT("Invalid export format");
            return false;
    }
    
    // Write to file
    if (StorageHAL::writeFile(filename.c_str(), content.c_str(), content.length()) < 0) {
        DEBUG_PRINTF("Failed to export data to file: %s", filename.c_str());
        return false;
    }
    
    DEBUG_PRINTF("Exported %d entries to file: %s", entries.size(), filename.c_str());
    return true;
}

String ExportUtil::exportToCSV(const std::vector<LogEntry>& entries) {
    String csv = "Timestamp,Date,Time,Gender,Shirt Color,Shirt RGB,Pants Color,Pants RGB,Shoes Color,Shoes RGB,Item Type,Item Description,Notes\n";
    
    for (const auto& entry : entries) {
        // Format timestamp
        String date = entry.getFormattedTimestamp("%Y-%m-%d");
        String time = entry.getFormattedTimestamp("%H:%M:%S");
        
        // Format gender
        String gender;
        switch (entry.getGender()) {
            case GENDER_MALE: gender = "Male"; break;
            case GENDER_FEMALE: gender = "Female"; break;
            case GENDER_OTHER: gender = "Other"; break;
            default: gender = "Unknown"; break;
        }
        
        // Format item type
        String itemType;
        switch (entry.getItemType()) {
            case ITEM_CLOTHING: itemType = "Clothing"; break;
            case ITEM_ELECTRONICS: itemType = "Electronics"; break;
            case ITEM_COSMETICS: itemType = "Cosmetics"; break;
            case ITEM_ACCESSORIES: itemType = "Accessories"; break;
            case ITEM_FOOD: itemType = "Food"; break;
            case ITEM_OTHER: itemType = "Other"; break;
            default: itemType = "Unknown"; break;
        }
        
        // Escape CSV fields
        String itemDesc = entry.getItemDescription();
        itemDesc.replace("\"", "\"\""); // Escape quotes
        
        String notes = entry.getNotes();
        notes.replace("\"", "\"\""); // Escape quotes
        
        // Add CSV row
        csv += String(entry.getTimestamp()) + ",";
        csv += date + ",";
        csv += time + ",";
        csv += gender + ",";
        csv += "\"" + entry.getShirtColor().name + "\",";
        csv += "\"" + String(entry.getShirtColor().rgb, HEX) + "\",";
        csv += "\"" + entry.getPantsColor().name + "\",";
        csv += "\"" + String(entry.getPantsColor().rgb, HEX) + "\",";
        csv += "\"" + entry.getShoesColor().name + "\",";
        csv += "\"" + String(entry.getShoesColor().rgb, HEX) + "\",";
        csv += itemType + ",";
        csv += "\"" + itemDesc + "\",";
        csv += "\"" + notes + "\"\n";
    }
    
    return csv;
}

String ExportUtil::exportToJSON(const std::vector<LogEntry>& entries) {
    String json = "{\n  \"entries\": [\n";
    
    for (size_t i = 0; i < entries.size(); i++) {
        const auto& entry = entries[i];
        
        // Format gender
        String gender;
        switch (entry.getGender()) {
            case GENDER_MALE: gender = "Male"; break;
            case GENDER_FEMALE: gender = "Female"; break;
            case GENDER_OTHER: gender = "Other"; break;
            default: gender = "Unknown"; break;
        }
        
        // Format item type
        String itemType;
        switch (entry.getItemType()) {
            case ITEM_CLOTHING: itemType = "Clothing"; break;
            case ITEM_ELECTRONICS: itemType = "Electronics"; break;
            case ITEM_COSMETICS: itemType = "Cosmetics"; break;
            case ITEM_ACCESSORIES: itemType = "Accessories"; break;
            case ITEM_FOOD: itemType = "Food"; break;
            case ITEM_OTHER: itemType = "Other"; break;
            default: itemType = "Unknown"; break;
        }
        
        // Escape JSON strings
        String itemDesc = entry.getItemDescription();
        itemDesc.replace("\"", "\\\""); // Escape quotes
        
        String notes = entry.getNotes();
        notes.replace("\"", "\\\""); // Escape quotes
        
        // Add JSON object
        json += "    {\n";
        json += "      \"timestamp\": " + String(entry.getTimestamp()) + ",\n";
        json += "      \"date\": \"" + entry.getFormattedTimestamp("%Y-%m-%d") + "\",\n";
        json += "      \"time\": \"" + entry.getFormattedTimestamp("%H:%M:%S") + "\",\n";
        json += "      \"gender\": \"" + gender + "\",\n";
        json += "      \"shirt\": {\n";
        json += "        \"color\": \"" + entry.getShirtColor().name + "\",\n";
        json += "        \"rgb\": \"" + String(entry.getShirtColor().rgb, HEX) + "\"\n";
        json += "      },\n";
        json += "      \"pants\": {\n";
        json += "        \"color\": \"" + entry.getPantsColor().name + "\",\n";
        json += "        \"rgb\": \"" + String(entry.getPantsColor().rgb, HEX) + "\"\n";
        json += "      },\n";
        json += "      \"shoes\": {\n";
        json += "        \"color\": \"" + entry.getShoesColor().name + "\",\n";
        json += "        \"rgb\": \"" + String(entry.getShoesColor().rgb, HEX) + "\"\n";
        json += "      },\n";
        json += "      \"item\": {\n";
        json += "        \"type\": \"" + itemType + "\",\n";
        json += "        \"description\": \"" + itemDesc + "\"\n";
        json += "      },\n";
        json += "      \"notes\": \"" + notes + "\"\n";
        json += "    }";
        
        if (i < entries.size() - 1) {
            json += ",";
        }
        json += "\n";
    }
    
    json += "  ]\n}";
    return json;
}

String ExportUtil::exportToText(const std::vector<LogEntry>& entries) {
    String text = "Loss Prevention Log Export\n";
    text += "===========================\n\n";
    
    for (const auto& entry : entries) {
        text += "Date: " + entry.getFormattedTimestamp("%Y-%m-%d") + "\n";
        text += "Time: " + entry.getFormattedTimestamp("%H:%M:%S") + "\n";
        
        // Format gender
        String gender;
        switch (entry.getGender()) {
            case GENDER_MALE: gender = "Male"; break;
            case GENDER_FEMALE: gender = "Female"; break;
            case GENDER_OTHER: gender = "Other"; break;
            default: gender = "Unknown"; break;
        }
        text += "Gender: " + gender + "\n";
        
        // Format clothing
        text += "Clothing:\n";
        text += "  Shirt: " + entry.getShirtColor().name + " (" + String(entry.getShirtColor().rgb, HEX) + ")\n";
        text += "  Pants: " + entry.getPantsColor().name + " (" + String(entry.getPantsColor().rgb, HEX) + ")\n";
        text += "  Shoes: " + entry.getShoesColor().name + " (" + String(entry.getShoesColor().rgb, HEX) + ")\n";
        
        // Format item
        String itemType;
        switch (entry.getItemType()) {
            case ITEM_CLOTHING: itemType = "Clothing"; break;
            case ITEM_ELECTRONICS: itemType = "Electronics"; break;
            case ITEM_COSMETICS: itemType = "Cosmetics"; break;
            case ITEM_ACCESSORIES: itemType = "Accessories"; break;
            case ITEM_FOOD: itemType = "Food"; break;
            case ITEM_OTHER: itemType = "Other"; break;
            default: itemType = "Unknown"; break;
        }
        text += "Item Type: " + itemType + "\n";
        text += "Item Description: " + entry.getItemDescription() + "\n";
        
        // Add notes if present
        if (entry.getNotes().length() > 0) {
            text += "Notes: " + entry.getNotes() + "\n";
        }
        
        text += "----------------------------\n\n";
    }
    
    return text;
}
