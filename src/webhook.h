/**
 * Enhanced Loss Prevention Log
 * Connectivity Layer - Webhook Client
 * 
 * This file contains the interface for webhook operations
 */

#ifndef CONNECTIVITY_WEBHOOK_H
#define CONNECTIVITY_WEBHOOK_H

#include <Arduino.h>
#include <HTTPClient.h>
#include "../config.h"

class WebhookClient {
public:
    /**
     * Initialize the webhook client
     * @return true if successful, false otherwise
     */
    static bool init();
    
    /**
     * Send data to webhook URL
     * @param url webhook URL
     * @param data data to send
     * @return true if successful, false otherwise
     */
    static bool sendData(const String& url, const String& data);
    
    /**
     * Send data to webhook URL with retry
     * @param url webhook URL
     * @param data data to send
     * @param maxRetries maximum number of retry attempts
     * @param retryInterval interval between retries in milliseconds
     * @return true if successful, false otherwise
     */
    static bool sendDataWithRetry(const String& url, const String& data, 
                                 int maxRetries = WEBHOOK_MAX_RETRIES, 
                                 int retryInterval = WEBHOOK_RETRY_INTERVAL);
    
    /**
     * Get last HTTP status code
     * @return last HTTP status code
     */
    static int getLastStatusCode();
    
    /**
     * Get last response
     * @return last response string
     */
    static String getLastResponse();

private:
    static bool _initialized;
    static int _lastStatusCode;
    static String _lastResponse;
};

#endif // CONNECTIVITY_WEBHOOK_H
