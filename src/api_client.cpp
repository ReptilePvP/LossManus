/**
 * Enhanced Loss Prevention Log
 * Connectivity Layer - API Client Implementation
 */

#include "api_client.h"
#include <ArduinoJson.h>

// Static member initialization
bool ApiClient::_initialized = false;
String ApiClient::_baseUrl = "";
String ApiClient::_apiKey = "";
String ApiClient::_authToken = "";
uint32_t ApiClient::_tokenExpiry = 0;

bool ApiClient::init() {
    DEBUG_PRINT("Initializing API client...");
    
    if (_initialized) {
        DEBUG_PRINT("API client already initialized");
        return true;
    }
    
    // Initialize HTTP client
    if (!HttpClient::init()) {
        DEBUG_PRINT("Failed to initialize HTTP client");
        return false;
    }
    
    _initialized = true;
    DEBUG_PRINT("API client initialization complete");
    return true;
}

void ApiClient::setBaseUrl(const String& baseUrl) {
    _baseUrl = baseUrl;
    if (_baseUrl.endsWith("/")) {
        _baseUrl.remove(_baseUrl.length() - 1);
    }
    DEBUG_PRINTF("API base URL set to: %s", _baseUrl.c_str());
}

String ApiClient::getBaseUrl() {
    return _baseUrl;
}

void ApiClient::setApiKey(const String& apiKey) {
    _apiKey = apiKey;
    DEBUG_PRINT("API key set");
}

bool ApiClient::sendLogEntry(const String& entryJson) {
    if (!_initialized) {
        if (!init()) {
            return false;
        }
    }
    
    if (_baseUrl.isEmpty()) {
        DEBUG_PRINT("API base URL not set");
        return false;
    }
    
    // Check authentication if token is used
    if (!_apiKey.isEmpty() && !isAuthenticated()) {
        DEBUG_PRINT("Not authenticated, cannot send log entry");
        return false;
    }
    
    String url = buildUrl("/logs");
    
    DEBUG_PRINTF("Sending log entry to API: %s", url.c_str());
    
    String response = HttpClient::post(url, entryJson, "application/json", getDefaultHeaders());
    
    if (HttpClient::isLastRequestSuccessful()) {
        DEBUG_PRINT("Log entry sent successfully");
        return true;
    } else {
        DEBUG_PRINTF("Failed to send log entry, status: %d", HttpClient::getLastStatusCode());
        return false;
    }
}

String ApiClient::getStatistics() {
    if (!_initialized) {
        if (!init()) {
            return "{}";
        }
    }
    
    if (_baseUrl.isEmpty()) {
        DEBUG_PRINT("API base URL not set");
        return "{}";
    }
    
    // Check authentication if token is used
    if (!_apiKey.isEmpty() && !isAuthenticated()) {
        DEBUG_PRINT("Not authenticated, cannot get statistics");
        return "{}";
    }
    
    String url = buildUrl("/statistics");
    
    DEBUG_PRINTF("Getting statistics from API: %s", url.c_str());
    
    String response = HttpClient::get(url, getDefaultHeaders());
    
    if (HttpClient::isLastRequestSuccessful()) {
        DEBUG_PRINT("Statistics retrieved successfully");
        return response;
    } else {
        DEBUG_PRINTF("Failed to get statistics, status: %d", HttpClient::getLastStatusCode());
        return "{}";
    }
}

bool ApiClient::authenticate(const String& username, const String& password) {
    if (!_initialized) {
        if (!init()) {
            return false;
        }
    }
    
    if (_baseUrl.isEmpty()) {
        DEBUG_PRINT("API base URL not set");
        return false;
    }
    
    String url = buildUrl("/auth/login");
    
    // Create authentication payload
    StaticJsonDocument<256> doc;
    doc["username"] = username;
    doc["password"] = password;
    
    String payload;
    serializeJson(doc, payload);
    
    DEBUG_PRINTF("Authenticating with API: %s", url.c_str());
    
    String response = HttpClient::post(url, payload, "application/json");
    
    if (HttpClient::isLastRequestSuccessful()) {
        // Parse response to get token
        StaticJsonDocument<512> respDoc;
        DeserializationError error = deserializeJson(respDoc, response);
        
        if (error) {
            DEBUG_PRINTF("Failed to parse authentication response: %s", error.c_str());
            return false;
        }
        
        if (respDoc.containsKey("token")) {
            _authToken = respDoc["token"].as<String>();
            
            // Set token expiry (default to 1 hour if not provided)
            if (respDoc.containsKey("expiresIn")) {
                uint32_t expiresIn = respDoc["expiresIn"].as<uint32_t>();
                _tokenExpiry = millis() + (expiresIn * 1000);
            } else {
                _tokenExpiry = millis() + (3600 * 1000); // 1 hour
            }
            
            DEBUG_PRINT("Authentication successful");
            return true;
        } else {
            DEBUG_PRINT("Authentication response does not contain token");
            return false;
        }
    } else {
        DEBUG_PRINTF("Authentication failed, status: %d", HttpClient::getLastStatusCode());
        return false;
    }
}

bool ApiClient::isAuthenticated() {
    // If using API key, no token authentication needed
    if (!_apiKey.isEmpty()) {
        return true;
    }
    
    // Check if token exists and is not expired
    if (_authToken.isEmpty()) {
        return false;
    }
    
    if (millis() > _tokenExpiry) {
        DEBUG_PRINT("Authentication token expired");
        return refreshToken();
    }
    
    return true;
}

bool ApiClient::refreshToken() {
    if (!_initialized) {
        if (!init()) {
            return false;
        }
    }
    
    if (_baseUrl.isEmpty() || _authToken.isEmpty()) {
        DEBUG_PRINT("API base URL or auth token not set");
        return false;
    }
    
    String url = buildUrl("/auth/refresh");
    
    // Create refresh payload
    StaticJsonDocument<256> doc;
    doc["token"] = _authToken;
    
    String payload;
    serializeJson(doc, payload);
    
    DEBUG_PRINTF("Refreshing authentication token: %s", url.c_str());
    
    String response = HttpClient::post(url, payload, "application/json");
    
    if (HttpClient::isLastRequestSuccessful()) {
        // Parse response to get new token
        StaticJsonDocument<512> respDoc;
        DeserializationError error = deserializeJson(respDoc, response);
        
        if (error) {
            DEBUG_PRINTF("Failed to parse token refresh response: %s", error.c_str());
            return false;
        }
        
        if (respDoc.containsKey("token")) {
            _authToken = respDoc["token"].as<String>();
            
            // Set token expiry (default to 1 hour if not provided)
            if (respDoc.containsKey("expiresIn")) {
                uint32_t expiresIn = respDoc["expiresIn"].as<uint32_t>();
                _tokenExpiry = millis() + (expiresIn * 1000);
            } else {
                _tokenExpiry = millis() + (3600 * 1000); // 1 hour
            }
            
            DEBUG_PRINT("Token refresh successful");
            return true;
        } else {
            DEBUG_PRINT("Token refresh response does not contain token");
            return false;
        }
    } else {
        DEBUG_PRINTF("Token refresh failed, status: %d", HttpClient::getLastStatusCode());
        return false;
    }
}

String ApiClient::buildUrl(const String& endpoint) {
    if (_baseUrl.isEmpty()) {
        return endpoint;
    }
    
    if (endpoint.startsWith("/")) {
        return _baseUrl + endpoint;
    } else {
        return _baseUrl + "/" + endpoint;
    }
}

std::map<String, String> ApiClient::getDefaultHeaders() {
    std::map<String, String> headers;
    
    // Add API key if available
    if (!_apiKey.isEmpty()) {
        headers["X-API-Key"] = _apiKey;
    }
    
    // Add auth token if available
    if (!_authToken.isEmpty()) {
        headers["Authorization"] = "Bearer " + _authToken;
    }
    
    // Add common headers
    headers["Accept"] = "application/json";
    headers["User-Agent"] = "LossPreventionLog/" + String(APP_VERSION);
    
    return headers;
}
