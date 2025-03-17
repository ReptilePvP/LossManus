/**
 * Enhanced Loss Prevention Log
 * Data Management Layer - Data Synchronization Implementation
 */

#include "sync.h"
#include "../hal/wifi_hardware.h"

// Static member initialization
bool SyncManager::_initialized = false;
SyncStatus SyncManager::_syncStatus = SYNC_NONE;
time_t SyncManager::_lastSyncTime = 0;
std::vector<LogEntry> SyncManager::_syncQueue;
String SyncManager::_webhookUrl = "";
bool SyncManager::_autoSyncEnabled = true;
uint32_t SyncManager::_lastSyncAttempt = 0;

bool SyncManager::init() {
    DEBUG_PRINT("Initializing sync manager...");
    
    if (_initialized) {
        DEBUG_PRINT("Sync manager already initialized");
        return true;
    }
    
    _initialized = true;
    _syncStatus = SYNC_NONE;
    
    DEBUG_PRINT("Sync manager initialization complete");
    return true;
}

bool SyncManager::syncEntries(const std::vector<LogEntry>& entries) {
    if (!_initialized) {
        if (!init()) {
            return false;
        }
    }
    
    if (_webhookUrl.isEmpty()) {
        DEBUG_PRINT("Webhook URL not set, cannot sync entries");
        _syncStatus = SYNC_FAILED;
        return false;
    }
    
    if (!WiFiHAL::isEnabled() || WiFi.status() != WL_CONNECTED) {
        DEBUG_PRINT("WiFi not connected, queueing entries for later sync");
        
        // Queue entries for later sync
        for (const auto& entry : entries) {
            _syncQueue.push_back(entry);
        }
        
        _syncStatus = SYNC_FAILED;
        return false;
    }
    
    DEBUG_PRINTF("Syncing %d entries...", entries.size());
    _syncStatus = SYNC_IN_PROGRESS;
    
    bool success = true;
    for (const auto& entry : entries) {
        // Prepare JSON payload
        String json = "{";
        json += "\"timestamp\":" + String(entry.getTimestamp()) + ",";
        json += "\"gender\":" + String((int)entry.getGender()) + ",";
        json += "\"shirt_color\":\"" + entry.getShirtColor().name + "\",";
        json += "\"shirt_rgb\":\"" + String(entry.getShirtColor().rgb, HEX) + "\",";
        json += "\"pants_color\":\"" + entry.getPantsColor().name + "\",";
        json += "\"pants_rgb\":\"" + String(entry.getPantsColor().rgb, HEX) + "\",";
        json += "\"shoes_color\":\"" + entry.getShoesColor().name + "\",";
        json += "\"shoes_rgb\":\"" + String(entry.getShoesColor().rgb, HEX) + "\",";
        json += "\"item_type\":" + String((int)entry.getItemType()) + ",";
        
        // Escape JSON strings
        String itemDesc = entry.getItemDescription();
        itemDesc.replace("\"", "\\\"");
        json += "\"item_description\":\"" + itemDesc + "\",";
        
        String notes = entry.getNotes();
        notes.replace("\"", "\\\"");
        json += "\"notes\":\"" + notes + "\"";
        
        json += "}";
        
        // Send to webhook
        if (!WebhookClient::sendData(_webhookUrl, json)) {
            DEBUG_PRINT("Failed to sync entry, queueing for later");
            _syncQueue.push_back(entry);
            success = false;
        }
    }
    
    if (success) {
        _syncStatus = SYNC_COMPLETED;
        _lastSyncTime = time(nullptr);
        DEBUG_PRINT("Sync completed successfully");
    } else {
        _syncStatus = SYNC_FAILED;
        DEBUG_PRINT("Sync failed for some entries");
    }
    
    _lastSyncAttempt = millis();
    return success;
}

bool SyncManager::queueEntry(const LogEntry& entry) {
    if (!_initialized) {
        if (!init()) {
            return false;
        }
    }
    
    _syncQueue.push_back(entry);
    DEBUG_PRINTF("Entry queued for sync, queue size: %d", _syncQueue.size());
    return true;
}

bool SyncManager::processSyncQueue() {
    if (!_initialized) {
        if (!init()) {
            return false;
        }
    }
    
    if (_syncQueue.empty()) {
        DEBUG_PRINT("Sync queue is empty");
        return true;
    }
    
    if (!WiFiHAL::isEnabled() || WiFi.status() != WL_CONNECTED) {
        DEBUG_PRINT("WiFi not connected, cannot process sync queue");
        return false;
    }
    
    DEBUG_PRINTF("Processing sync queue with %d entries...", _syncQueue.size());
    
    std::vector<LogEntry> queueCopy = _syncQueue;
    _syncQueue.clear();
    
    bool success = syncEntries(queueCopy);
    
    if (!success) {
        DEBUG_PRINT("Failed to process sync queue");
    }
    
    return success;
}

SyncStatus SyncManager::getSyncStatus() {
    return _syncStatus;
}

time_t SyncManager::getLastSyncTime() {
    return _lastSyncTime;
}

size_t SyncManager::getPendingSyncCount() {
    return _syncQueue.size();
}

void SyncManager::setWebhookUrl(const String& url) {
    _webhookUrl = url;
    DEBUG_PRINTF("Webhook URL set to: %s", url.c_str());
}

String SyncManager::getWebhookUrl() {
    return _webhookUrl;
}

void SyncManager::setAutoSync(bool enabled) {
    _autoSyncEnabled = enabled;
    DEBUG_PRINTF("Auto sync %s", enabled ? "enabled" : "disabled");
}

bool SyncManager::isAutoSyncEnabled() {
    return _autoSyncEnabled;
}

void SyncManager::update() {
    if (!_initialized || !_autoSyncEnabled) {
        return;
    }
    
    // Check if there are entries in the queue and it's time to retry
    if (!_syncQueue.empty() && (millis() - _lastSyncAttempt > WEBHOOK_RETRY_INTERVAL)) {
        DEBUG_PRINT("Auto sync: Processing sync queue");
        processSyncQueue();
    }
}
