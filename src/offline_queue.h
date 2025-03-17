/**
 * Enhanced Loss Prevention Log
 * Connectivity Layer - Offline Queue Manager
 * 
 * This file contains the interface for offline operation queue
 */

#ifndef CONNECTIVITY_OFFLINE_QUEUE_H
#define CONNECTIVITY_OFFLINE_QUEUE_H

#include <Arduino.h>
#include <vector>
#include "../data/log_entry.h"
#include "../config.h"

enum QueueItemType {
    QUEUE_LOG_ENTRY,
    QUEUE_API_REQUEST,
    QUEUE_WEBHOOK
};

struct QueueItem {
    QueueItemType type;
    String data;
    String target;
    uint32_t timestamp;
    int retryCount;
};

class OfflineQueueManager {
public:
    /**
     * Initialize the offline queue manager
     * @return true if successful, false otherwise
     */
    static bool init();
    
    /**
     * Queue a log entry for synchronization
     * @param entry log entry to queue
     * @return true if successful, false otherwise
     */
    static bool queueLogEntry(const LogEntry& entry);
    
    /**
     * Queue an API request
     * @param endpoint API endpoint
     * @param data request data
     * @return true if successful, false otherwise
     */
    static bool queueApiRequest(const String& endpoint, const String& data);
    
    /**
     * Queue a webhook request
     * @param url webhook URL
     * @param data request data
     * @return true if successful, false otherwise
     */
    static bool queueWebhook(const String& url, const String& data);
    
    /**
     * Process the queue
     * @return number of items processed successfully
     */
    static int processQueue();
    
    /**
     * Get queue size
     * @return number of items in the queue
     */
    static size_t getQueueSize();
    
    /**
     * Clear the queue
     */
    static void clearQueue();
    
    /**
     * Save queue to storage
     * @return true if successful, false otherwise
     */
    static bool saveQueue();
    
    /**
     * Load queue from storage
     * @return true if successful, false otherwise
     */
    static bool loadQueue();
    
    /**
     * Enable/disable automatic queue processing
     * @param enabled true to enable, false to disable
     */
    static void setAutoProcessing(bool enabled);
    
    /**
     * Check if automatic queue processing is enabled
     * @return true if enabled, false otherwise
     */
    static bool isAutoProcessingEnabled();
    
    /**
     * Update offline queue manager (call in loop)
     */
    static void update();

private:
    static bool _initialized;
    static std::vector<QueueItem> _queue;
    static bool _autoProcessingEnabled;
    static uint32_t _lastProcessTime;
    static const char* QUEUE_FILENAME;
};

#endif // CONNECTIVITY_OFFLINE_QUEUE_H
