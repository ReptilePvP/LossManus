/**
 * Enhanced Loss Prevention Log
 * Connectivity Layer - HTTP Client Implementation
 */

#include "http_client.h"
#include <map>

// Static member initialization
bool HttpClient::_initialized = false;
int HttpClient::_lastStatusCode = 0;
uint32_t HttpClient::_timeout = 10000; // 10 seconds default timeout

bool HttpClient::init() {
    DEBUG_PRINT("Initializing HTTP client...");
    
    if (_initialized) {
        DEBUG_PRINT("HTTP client already initialized");
        return true;
    }
    
    _initialized = true;
    DEBUG_PRINT("HTTP client initialization complete");
    return true;
}

String HttpClient::get(const String& url, const std::map<String, String>& headers) {
    if (!_initialized) {
        if (!init()) {
            return "";
        }
    }
    
    DEBUG_PRINTF("HTTP GET: %s", url.c_str());
    
    HTTPClient http;
    http.begin(url);
    http.setTimeout(_timeout);
    
    // Add headers
    for (const auto& header : headers) {
        http.addHeader(header.first, header.second);
    }
    
    _lastStatusCode = http.GET();
    String response = "";
    
    if (_lastStatusCode > 0) {
        response = http.getString();
        DEBUG_PRINTF("HTTP response: %d, length: %d", _lastStatusCode, response.length());
    } else {
        DEBUG_PRINTF("HTTP GET failed, error: %s", http.errorToString(_lastStatusCode).c_str());
    }
    
    http.end();
    return response;
}

String HttpClient::post(const String& url, const String& data, 
                       const String& contentType, const std::map<String, String>& headers) {
    if (!_initialized) {
        if (!init()) {
            return "";
        }
    }
    
    DEBUG_PRINTF("HTTP POST: %s", url.c_str());
    
    HTTPClient http;
    http.begin(url);
    http.setTimeout(_timeout);
    
    // Add content type header
    http.addHeader("Content-Type", contentType);
    
    // Add other headers
    for (const auto& header : headers) {
        http.addHeader(header.first, header.second);
    }
    
    _lastStatusCode = http.POST(data);
    String response = "";
    
    if (_lastStatusCode > 0) {
        response = http.getString();
        DEBUG_PRINTF("HTTP response: %d, length: %d", _lastStatusCode, response.length());
    } else {
        DEBUG_PRINTF("HTTP POST failed, error: %s", http.errorToString(_lastStatusCode).c_str());
    }
    
    http.end();
    return response;
}

String HttpClient::put(const String& url, const String& data, 
                      const String& contentType, const std::map<String, String>& headers) {
    if (!_initialized) {
        if (!init()) {
            return "";
        }
    }
    
    DEBUG_PRINTF("HTTP PUT: %s", url.c_str());
    
    HTTPClient http;
    http.begin(url);
    http.setTimeout(_timeout);
    
    // Add content type header
    http.addHeader("Content-Type", contentType);
    
    // Add other headers
    for (const auto& header : headers) {
        http.addHeader(header.first, header.second);
    }
    
    _lastStatusCode = http.PUT(data);
    String response = "";
    
    if (_lastStatusCode > 0) {
        response = http.getString();
        DEBUG_PRINTF("HTTP response: %d, length: %d", _lastStatusCode, response.length());
    } else {
        DEBUG_PRINTF("HTTP PUT failed, error: %s", http.errorToString(_lastStatusCode).c_str());
    }
    
    http.end();
    return response;
}

String HttpClient::del(const String& url, const std::map<String, String>& headers) {
    if (!_initialized) {
        if (!init()) {
            return "";
        }
    }
    
    DEBUG_PRINTF("HTTP DELETE: %s", url.c_str());
    
    HTTPClient http;
    http.begin(url);
    http.setTimeout(_timeout);
    
    // Add headers
    for (const auto& header : headers) {
        http.addHeader(header.first, header.second);
    }
    
    _lastStatusCode = http.sendRequest("DELETE");
    String response = "";
    
    if (_lastStatusCode > 0) {
        response = http.getString();
        DEBUG_PRINTF("HTTP response: %d, length: %d", _lastStatusCode, response.length());
    } else {
        DEBUG_PRINTF("HTTP DELETE failed, error: %s", http.errorToString(_lastStatusCode).c_str());
    }
    
    http.end();
    return response;
}

int HttpClient::getLastStatusCode() {
    return _lastStatusCode;
}

bool HttpClient::isLastRequestSuccessful() {
    return (_lastStatusCode >= 200 && _lastStatusCode < 300);
}

void HttpClient::setTimeout(uint32_t timeout) {
    _timeout = timeout;
}

uint32_t HttpClient::getTimeout() {
    return _timeout;
}
