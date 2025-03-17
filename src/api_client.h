/**
 * Enhanced Loss Prevention Log
 * Connectivity Layer - API Client
 * 
 * This file contains the interface for API operations
 */

#ifndef CONNECTIVITY_API_CLIENT_H
#define CONNECTIVITY_API_CLIENT_H

#include <Arduino.h>
#include "http_client.h"
#include "../config.h"

class ApiClient {
public:
    /**
     * Initialize the API client
     * @return true if successful, false otherwise
     */
    static bool init();
    
    /**
     * Set API base URL
     * @param baseUrl API base URL
     */
    static void setBaseUrl(const String& baseUrl);
    
    /**
     * Get API base URL
     * @return current API base URL
     */
    static String getBaseUrl();
    
    /**
     * Set API key
     * @param apiKey API key
     */
    static void setApiKey(const String& apiKey);
    
    /**
     * Send log entry to API
     * @param entryJson log entry in JSON format
     * @return true if successful, false otherwise
     */
    static bool sendLogEntry(const String& entryJson);
    
    /**
     * Get statistics from API
     * @return statistics in JSON format
     */
    static String getStatistics();
    
    /**
     * Authenticate with API
     * @param username username
     * @param password password
     * @return true if successful, false otherwise
     */
    static bool authenticate(const String& username, const String& password);
    
    /**
     * Check if authenticated
     * @return true if authenticated, false otherwise
     */
    static bool isAuthenticated();
    
    /**
     * Refresh authentication token
     * @return true if successful, false otherwise
     */
    static bool refreshToken();

private:
    static bool _initialized;
    static String _baseUrl;
    static String _apiKey;
    static String _authToken;
    static uint32_t _tokenExpiry;
    
    static String buildUrl(const String& endpoint);
    static std::map<String, String> getDefaultHeaders();
};

#endif // CONNECTIVITY_API_CLIENT_H
