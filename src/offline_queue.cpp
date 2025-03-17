/**
 * Enhanced Loss Prevention Log
 * Connectivity Layer - Offline Queue Manager Implementation
 */

#include "offline_queue.h"
#include "webhook.h"
#include "api_client.h"
#include "wifi_manager.h"
#include "../hal/storage.h"
#include <ArduinoJson.h>

// Static member initialization
bool OfflineQueueManager::_initialized = false;
std::vector<QueueItem> OfflineQueueManager::_queue;
bool OfflineQueueManager::_autoProcessingEnabled = true;
uint32_t OfflineQueueManager::_lastProcessTime = 0;
const char* OfflineQueueManager::QUEUE_FILENAME = "/offline_queue.json";

bool OfflineQueueManager::init() {
    DEBUG_PRINT("Initializing offline queue manager...");
    
    if (_initialized) {
        DEBUG_PRINT("Offline queue manager already initialized");
        return true;
    }
    
    // Load queue from storage
    loadQueue();
    
    _initialized = true;
    _autoProcessingEnabled = true;
    _lastProcessTime = millis();
    
    DEBUG_PRINTF("Offline queue manager initialized with %d items", _queue.size());
    return true;
}

bool OfflineQueueManager::queueLogEntry(const LogEntry& entry) {
    if (!_initialized) {
        if (!init()) {
            return false;
        }
    }
    
    QueueItem item;
    item.type = QUEUE_LOG_ENTRY;
    item.data = entry.serialize();
    item.target = "";
    item.timestamp = millis();
    item.retryCount = 0;
    
    _queue.push_back(item);
    
    DEBUG_PRINTF("Log entry queued for offline processing, queue size: %d", _queue.size());
    return saveQueue();
}

bool OfflineQueueManager::queueApiRequest(const String& endpoint, const String& data) {
    if (!_initialized) {
        if (!init()) {
            return false;
        }
    }
    
    QueueItem item;
    item.type = QUEUE_API_REQUEST;
    item.data = data;
    item.target = endpoint;
    item.timestamp = millis();
    item.retryCount = 0;
    
    _queue.push_back(item);
    
    DEBUG_PRINTF("API request queued for offline processing, queue size: %d", _queue.size());
    return saveQueue();
}

bool OfflineQueueManager::queueWebhook(const String& url, const String& data) {
    if (!_initialized) {
        if (!init()) {
            return false;
        }
    }
    
    QueueItem item;
    item.type = QUEUE_WEBHOOK;
    item.data = data;
    item.target = url;
    item.timestamp = millis();
    item.retryCount = 0;
    
    _queue.push_back(item);
    
    DEBUG_PRINTF("Webhook queued for offline processing, queue size: %d", _queue.size());
    return saveQueue();
}

int OfflineQueueManager::processQueue() {
    if (!_initialized) {
        if (!init()) {
            return 0;
        }
    }
    
    if (_queue.empty()) {
        return 0;
    }
    
    if (!WiFiManager::isConnected()) {
        DEBUG_PRINT("Cannot process offline queue, not connected to WiFi");
        return 0;
    }
    
    DEBUG_PRINTF("Processing offline queue with %d items...", _queue.size());
    
    int successCount = 0;
    std::vector<size_t> itemsToRemove;
    
    for (size_t i = 0; i < _queue.size(); i++) {
        QueueItem& item = _queue[i];
        bool success = false;
        
        switch (item.type) {
            case QUEUE_LOG_ENTRY: {
                LogEntry entry;
                if (entry.deserialize(item.data)) {
                    // Convert to JSON for API
                    StaticJsonDocument<512> doc;
                    doc["timestamp"] = entry.getTimestamp();
                    doc["gender"] = (int)entry.getGender();
                    doc["shirt_color"] = entry.getShirtColor().name;
                    doc["shirt_rgb"] = String(entry.getShirtColor().rgb, HEX);
                    doc["pants_color"] = entry.getPantsColor().name;
                    doc["pants_rgb"] = String(entry.getPantsColor().rgb, HEX);
                    doc["shoes_color"] = entry.getShoesColor().name;
                    doc["shoes_rgb"] = String(entry.getShoesColor().rgb, HEX);
                    doc["item_type"] = (int)entry.getItemType();
                    doc["item_description"] = entry.getItemDescription();
                    doc["notes"] = entry.getNotes();
                    
                    String json;
                    serializeJson(doc, json);
                    
                    success = ApiClient::sendLogEntry(json);
                }
                break;
            }
            case QUEUE_API_REQUEST:
                success = ApiClient::sendLogEntry(item.data);
                break;
            case QUEUE_WEBHOOK:
                success = WebhookClient::sendData(item.target, item.data);
                break;
        }
        
        if (success) {
            successCount++;
            itemsToRemove.push_back(i);
            DEBUG_PRINTF("Successfully processed queue item %d", i);
        } else {
            item.retryCount++;
            DEBUG_PRINTF("Failed to process queue item %d, retry count: %d", i, item.retryCount);
            
            // Remove item if max retries reached
            if (item.retryCount >= WEBHOOK_MAX_RETRIES) {
                DEBUG_PRINTF("Max retries reached for queue item %d, removing", i);
                itemsToRemove.push_back(i);
            }
        }
    }
    
    // Remove processed items (in reverse order to maintain indices)
    std::sort(itemsToRemove.begin(), itemsToRemove.end(), std::greater<size_t>());
    for (size_t index : itemsToRemove) {
        _queue.erase(_queue.begin() + index);
    }
    
    // Save updated queue
    saveQueue();
    
    DEBUG_PRINTF("Processed %d items from offline queue, %d successful", itemsToRemove.size(), successCount);
    return successCount;
}

size_t OfflineQueueManager::getQueueSize() {
    return _queue.size();
}

void OfflineQueueManager::clearQueue() {
    _queue.clear();
    saveQueue();
    DEBUG_PRINT("Offline queue cleared");
}

bool OfflineQueueManager::saveQueue() {
    if (!StorageHAL::isAvailable()) {
        DEBUG_PRINT("Storage not available, cannot save offline queue");
        return false;
    }
    
    // Create JSON document
    DynamicJsonDocument doc(16384); // Adjust size as needed
    JsonArray queueArray = doc.createNestedArray("queue");
    
    for (const auto& item : _queue) {
        JsonObject itemObj = queueArray.createNestedObject();
        itemObj["type"] = (int)item.type;
        itemObj["data"] = item.data;
        itemObj["target"] = item.target;
        itemObj["timestamp"] = item.timestamp;
        itemObj["retryCount"] = item.retryCount;
    }
    
    // Serialize JSON to string
    String jsonStr;
    serializeJson(doc, jsonStr);
    
    // Write to file
    if (StorageHAL::writeFile(QUEUE_FILENAME, jsonStr.c_str(), jsonStr.length()) < 0) {
        DEBUG_PRINT("Failed to save offline queue to file");
        return false;
    }
    
    DEBUG_PRINTF("Saved offline queue with %d items", _queue.size());
    return true;
}

bool OfflineQueueManager::loadQueue() {
    _queue.clear();
    
    if (!StorageHAL::isAvailable()) {
        DEBUG_PRINT("Storage not available, cannot load offline queue");
        return false;
    }
    
    if (!StorageHAL::fileExists(QUEUE_FILENAME)) {
        DEBUG_PRINT("Offline queue file not found, starting with empty queue");
        return true;
    }
    
    // Read file content
    int fileSize = StorageHAL::getFileSize(QUEUE_FILENAME);
    if (fileSize <= 0) {
        DEBUG_PRINT("Offline queue file is empty");
        return true;
    }
    
    char* buffer = new char[fileSize + 1];
    if (StorageHAL::readFile(QUEUE_FILENAME, buffer, fileSize + 1) < 0) {
        DEBUG_PRINT("Failed to read offline queue file");
        delete[] buffer;
        return false;
    }
    
    // Parse JSON
    DynamicJsonDocument doc(16384); // Adjust size as needed
    DeserializationError error = deserializeJson(doc, buffer);
    delete[] buffer;
    
    if (error) {
        DEBUG_PRINTF("Failed to parse offline queue file: %s", error.c_str());
        return false;
    }
    
    // Extract queue items
    JsonArray queueArray = doc["queue"].as<JsonArray>();
    for (JsonObject itemObj : queueArray) {
        QueueItem item;
        item.type = (QueueItemType)itemObj["type"].as<int>();
        item.data = itemObj["data"].as<String>();
        item.target = itemObj["target"].as<String>();
        item.timestamp = itemObj["timestamp"].as<uint32_t>();
        item.retryCount = itemObj["retryCount"].as<int>();
        
        _queue.push_back(item);
    }
    
    DEBUG_PRINTF("Loaded offline queue with %d items", _queue.size());
    return true;
}

void OfflineQueueManager::setAutoProcessing(bool enabled) {
    _autoProcessingEnabled = enabled;
    DEBUG_PRINTF("Auto processing %s", enabled ? "enabled" : "disabled");
}

bool OfflineQueueManager::isAutoProcessingEnabled() {
    return _autoProcessingEnabled;
}

void OfflineQueueManager::update() {
    if (!_initialized || !_autoProcessingEnabled) {
        return;
    }
    
    // Process queue periodically
    if (!_queue.empty() && millis() - _lastProcessTime > WEBHOOK_RETRY_INTERVAL) {
        _lastProcessTime = millis();
        
        if (WiFiManager::isConnected()) {
            DEBUG_PRINT("Auto processing offline queue");
            processQueue();
        }
    }
}
