/**
 * Enhanced Loss Prevention Log
 * Hardware Abstraction Layer - RTC Implementation
 */

#include "rtc.h"
#include <WiFi.h>

// Static member initialization
bool RtcHAL::_initialized = false;
const char* RtcHAL::_timeZone = "UTC";

bool RtcHAL::init() {
    DEBUG_PRINT("Initializing RTC...");
    
    if (_initialized) {
        DEBUG_PRINT("RTC already initialized");
        return true;
    }
    
    // Configure timezone
    configTime(0, 0, "pool.ntp.org"); // UTC time
    setenv("TZ", _timeZone, 1);
    tzset();
    
    // Check if RTC has valid time
    struct tm timeinfo;
    if (!getTimeInfo(&timeinfo)) {
        DEBUG_PRINT("Failed to get time info");
        return false;
    }
    
    // If time is not set (year < 2023), set a default time
    if (timeinfo.tm_year < (2023 - 1900)) {
        DEBUG_PRINT("RTC time not set, setting default time");
        setTime(2023, 1, 1, 0, 0, 0);
    }
    
    _initialized = true;
    DEBUG_PRINT("RTC initialization complete");
    return true;
}

time_t RtcHAL::getTime() {
    time_t now;
    time(&now);
    return now;
}

bool RtcHAL::getTimeInfo(struct tm* timeinfo) {
    time_t now = getTime();
    struct tm* result = localtime(&now);
    if (result == nullptr) {
        return false;
    }
    
    *timeinfo = *result;
    return true;
}

bool RtcHAL::setTime(time_t time) {
    struct timeval tv = { .tv_sec = time, .tv_usec = 0 };
    if (settimeofday(&tv, nullptr) != 0) {
        DEBUG_PRINT("Failed to set system time");
        return false;
    }
    
    DEBUG_PRINTF("System time set to: %s", formatTime(time, "%Y-%m-%d %H:%M:%S").c_str());
    return true;
}

bool RtcHAL::setTime(int year, int month, int day, int hour, int minute, int second) {
    struct tm timeinfo = {0};
    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon = month - 1;
    timeinfo.tm_mday = day;
    timeinfo.tm_hour = hour;
    timeinfo.tm_min = minute;
    timeinfo.tm_sec = second;
    
    time_t time = mktime(&timeinfo);
    return setTime(time);
}

bool RtcHAL::syncWithNTP(const char* ntpServer) {
    if (WiFi.status() != WL_CONNECTED) {
        DEBUG_PRINT("WiFi not connected, cannot sync with NTP");
        return false;
    }
    
    DEBUG_PRINTF("Syncing time with NTP server: %s", ntpServer);
    
    configTime(0, 0, ntpServer);
    
    // Wait for time to be set
    int retry = 0;
    const int maxRetries = 10;
    struct tm timeinfo = {0};
    
    while (timeinfo.tm_year < (2023 - 1900) && retry < maxRetries) {
        DEBUG_PRINT("Waiting for NTP time sync...");
        delay(500);
        time_t now = getTime();
        localtime_r(&now, &timeinfo);
        retry++;
    }
    
    if (timeinfo.tm_year < (2023 - 1900)) {
        DEBUG_PRINT("Failed to sync time with NTP");
        return false;
    }
    
    DEBUG_PRINTF("Time synced with NTP: %s", formatTime(getTime(), "%Y-%m-%d %H:%M:%S").c_str());
    return true;
}

String RtcHAL::formatTime(time_t time, const char* format) {
    char buffer[64];
    struct tm* timeinfo = localtime(&time);
    strftime(buffer, sizeof(buffer), format, timeinfo);
    return String(buffer);
}

String RtcHAL::formatCurrentTime(const char* format) {
    return formatTime(getTime(), format);
}
