/**
 * Enhanced Loss Prevention Log
 * Connectivity Layer - Webhook Client Implementation
 */

#include "webhook.h"

// Static member initialization
bool WebhookClient::_initialized = false;
int WebhookClient::_lastStatusCode = 0;
String WebhookClient::_lastResponse = "";

bool WebhookClient::init() {
    DEBUG_PRINT("Initializing webhook client...");
    
    if (_initialized) {
        DEBUG_PRINT("Webhook client already initialized");
        return true;
    }
    
    _initialized = true;
    DEBUG_PRINT("Webhook client initialization complete");
    return true;
}

bool WebhookClient::sendData(const String& url, const String& data) {
    if (!_initialized) {
        if (!init()) {
            return false;
        }
    }
    
    if (url.isEmpty()) {
        DEBUG_PRINT("Webhook URL is empty");
        return false;
    }
    
    DEBUG_PRINTF("Sending data to webhook: %s", url.c_str());
    
    HTTPClient http;
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    
    _lastStatusCode = http.POST(data);
    _lastResponse = http.getString();
    
    DEBUG_PRINTF("Webhook response: %d - %s", _lastStatusCode, _lastResponse.c_str());
    
    http.end();
    
    return (_lastStatusCode >= 200 && _lastStatusCode < 300);
}

bool WebhookClient::sendDataWithRetry(const String& url, const String& data, 
                                     int maxRetries, int retryInterval) {
    if (!_initialized) {
        if (!init()) {
            return false;
        }
    }
    
    int retryCount = 0;
    bool success = false;
    
    while (retryCount <= maxRetries && !success) {
        if (retryCount > 0) {
            DEBUG_PRINTF("Retry attempt %d/%d after %d ms", 
                        retryCount, maxRetries, retryInterval);
            delay(retryInterval);
        }
        
        success = sendData(url, data);
        
        if (success) {
            DEBUG_PRINT("Webhook send successful");
            break;
        }
        
        retryCount++;
    }
    
    if (!success) {
        DEBUG_PRINTF("Webhook send failed after %d attempts", retryCount);
    }
    
    return success;
}

int WebhookClient::getLastStatusCode() {
    return _lastStatusCode;
}

String WebhookClient::getLastResponse() {
    return _lastResponse;
}
