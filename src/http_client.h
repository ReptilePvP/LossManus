/**
 * Enhanced Loss Prevention Log
 * Connectivity Layer - HTTP Client
 * 
 * This file contains the interface for HTTP operations
 */

#ifndef CONNECTIVITY_HTTP_CLIENT_H
#define CONNECTIVITY_HTTP_CLIENT_H

#include <Arduino.h>
#include <HTTPClient.h>
#include "../config.h"

class HttpClient {
public:
    /**
     * Initialize the HTTP client
     * @return true if successful, false otherwise
     */
    static bool init();
    
    /**
     * Perform GET request
     * @param url request URL
     * @param headers optional request headers
     * @return response body or empty string on error
     */
    static String get(const String& url, const std::map<String, String>& headers = {});
    
    /**
     * Perform POST request
     * @param url request URL
     * @param data request body
     * @param contentType content type header
     * @param headers optional request headers
     * @return response body or empty string on error
     */
    static String post(const String& url, const String& data, 
                      const String& contentType = "application/json",
                      const std::map<String, String>& headers = {});
    
    /**
     * Perform PUT request
     * @param url request URL
     * @param data request body
     * @param contentType content type header
     * @param headers optional request headers
     * @return response body or empty string on error
     */
    static String put(const String& url, const String& data, 
                     const String& contentType = "application/json",
                     const std::map<String, String>& headers = {});
    
    /**
     * Perform DELETE request
     * @param url request URL
     * @param headers optional request headers
     * @return response body or empty string on error
     */
    static String del(const String& url, const std::map<String, String>& headers = {});
    
    /**
     * Get last HTTP status code
     * @return last HTTP status code
     */
    static int getLastStatusCode();
    
    /**
     * Check if last request was successful
     * @return true if status code is 2xx, false otherwise
     */
    static bool isLastRequestSuccessful();
    
    /**
     * Set request timeout
     * @param timeout timeout in milliseconds
     */
    static void setTimeout(uint32_t timeout);
    
    /**
     * Get request timeout
     * @return timeout in milliseconds
     */
    static uint32_t getTimeout();

private:
    static bool _initialized;
    static int _lastStatusCode;
    static uint32_t _timeout;
};

#endif // CONNECTIVITY_HTTP_CLIENT_H
