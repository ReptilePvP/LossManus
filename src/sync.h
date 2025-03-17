/**
 * Enhanced Loss Prevention Log
 * Data Management Layer - Data Synchronization
 * 
 * This file contains the interface for data synchronization operations
 */

#ifndef DATA_SYNC_H
#define DATA_SYNC_H

#include <Arduino.h>
#include <vector>
#include "log_entry.h"
#include "../connectivity/webhook.h"
#include "../config.h"

enum SyncStatus {
    SYNC_NONE,
    SYNC_IN_PROGRESS,
    SYNC_COMPLETED,
    SYNC_FAILED
};

class SyncManager {
public:
    /**
     * Initialize the sync manager
     * @return true if successful, false otherwise
     */
    static bool init();
    
    /**
     * Synchronize log entries with server
     * @param entries vector of entries to synchronize
     * @return true if successful, false otherwise
     */
    static bool syncEntries(const std::vector<LogEntry>& entries);
    
    /**
     * Queue entry for synchronization
     * @param entry entry to queue
     * @return true if successful, false otherwise
     */
    static bool queueEntry(const LogEntry& entry);
    
    /**
     * Process sync queue
     * @return true if all entries were synchronized, false otherwise
     */
    static bool processSyncQueue();
    
    /**
     * Get sync status
     * @return current sync status
     */
    static SyncStatus getSyncStatus();
    
    /**
     * Get last sync time
     * @return timestamp of last successful sync
     */
    static time_t getLastSyncTime();
    
    /**
     * Get pending sync count
     * @return number of entries pending synchronization
     */
    static size_t getPendingSyncCount();
    
    /**
     * Set webhook URL
     * @param url webhook URL
     */
    static void setWebhookUrl(const String& url);
    
    /**
     * Get webhook URL
     * @return current webhook URL
     */
    static String getWebhookUrl();
    
    /**
     * Enable/disable automatic synchronization
     * @param enabled true to enable, false to disable
     */
    static void setAutoSync(bool enabled);
    
    /**
     * Check if automatic synchronization is enabled
     * @return true if enabled, false otherwise
     */
    static bool isAutoSyncEnabled();
    
    /**
     * Update sync manager (call in loop)
     */
    static void update();

private:
    static bool _initialized;
    static SyncStatus _syncStatus;
    static time_t _lastSyncTime;
    static std::vector<LogEntry> _syncQueue;
    static String _webhookUrl;
    static bool _autoSyncEnabled;
    static uint32_t _lastSyncAttempt;
};

#endif // DATA_SYNC_H
